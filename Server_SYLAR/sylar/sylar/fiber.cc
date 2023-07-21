#include "fiber.h"
#include "config.h"
#include <atomic>
#include "scheduler.h"
#include "macro.h"
#include "log.h"
namespace sylar{

static Logger::ptr g_logger = SYLAR_LOG_NAME("system");

//原子变量
//在多线程中如果使用了原子变量，其本身就保证了数据访问的互斥性，
//所以不需要使用互斥量来保护该变量了。
static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count {0};

//该变量在各自线程中各有各的值且互不影响
//副协程
static thread_local Fiber* t_fiber = nullptr;
//主协程
static thread_local Fiber::ptr t_threadFiber = nullptr;

static ConfigVar<uint32_t>::ptr g_fiber_statck_size = 
    Config::Lookup<uint32_t>("fiber.stack_size",1024 * 1024,"fiber stack size");

class MallocStackAllocator{
public:
    static void* Alloc(size_t size){
        return malloc(size);
    }

    static void Dealloc(void * vp,size_t size){
        return free(vp);
    }
};

using StackAllocator = MallocStackAllocator;

Fiber::Fiber(){
    m_state = EXEC;
    SetThis(this);

    if(getcontext(&m_ctx)){
        SYLAR_ASSERT2(false,"getcontext");
    }
    ++s_fiber_count;
    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber";
}

uint64_t Fiber::GetFiberId(){
    if(t_fiber){
        return t_fiber->getId();
    }
    return 0;
}


Fiber::Fiber(std::function<void()>cb ,size_t stacksize, bool use_caller)
    :m_id(++s_fiber_id)
    ,m_cb(cb) {
    ++s_fiber_count;
    m_stacksize = stacksize ? stacksize : g_fiber_statck_size->getValue();

    m_stack = StackAllocator::Alloc(m_stacksize);
    if(getcontext(&m_ctx)){
        SYLAR_ASSERT2(false,"getcontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    if(use_caller){
        //会执行MainFunc函数
        makecontext(&m_ctx, &Fiber::MainFunc,0);
    }else{
        SYLAR_LOG_INFO(g_logger) << "makecontext!!";
        makecontext(&m_ctx, &Fiber::CallerMainFunc,0);
    }

    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber id= " << m_id;

}
Fiber::~Fiber(){
    --s_fiber_count;
    if(m_stack){
        SYLAR_ASSERT(m_state == TERM || m_state == INIT || m_state == EXCEPT);
        StackAllocator::Dealloc(m_stack,m_stacksize);
    }else{
        //主协程
        SYLAR_ASSERT(!m_cb);
        SYLAR_ASSERT(m_state == EXEC);

        Fiber * cur  = t_fiber;
        if(cur == this){
            SetThis(nullptr);
        }
    }
    SYLAR_LOG_DEBUG(g_logger) << "Fiber::~Fiber id= " << m_id;

}
//重置协程函数与状态
//init,term状态使用
void Fiber::reset(std::function<void()> cb){
    SYLAR_ASSERT(m_stack);//主协程是没有栈的
    SYLAR_ASSERT(m_state == TERM || m_state == INIT || m_state == EXCEPT);
    m_cb = cb;
    if(getcontext(&m_ctx)){
        SYLAR_ASSERT2(false,"getcontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    makecontext(&m_ctx, &Fiber::MainFunc,0);
    m_state = INIT;
}
// 切换到当前协程执行到后台，自己进入执行状态
void Fiber::call(){
    SetThis(this);
    m_state = EXEC;
    //SYLAR_ASSERT(GetThis() == t_threadFiber);
    if(swapcontext(&t_threadFiber->m_ctx,&m_ctx)){
        SYLAR_ASSERT2(false,"swapcontext");
    }
}

void Fiber::back(){
    SetThis(t_threadFiber.get());
    if(swapcontext(&m_ctx,&t_threadFiber->m_ctx)){
        SYLAR_ASSERT2(false,"swapcontext");
    }
}

void Fiber::swapIn(){
    SetThis(this);
    SYLAR_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if(swapcontext(&Scheduler::GetMainFiber()->m_ctx,&m_ctx)){
        SYLAR_ASSERT2(false,"swapcontext");
    } 
}    
//当前切换到后台
void Fiber::swapOut(){
    SetThis(Scheduler::GetMainFiber());
    if(swapcontext( &m_ctx,&Scheduler::GetMainFiber()->m_ctx)){
        SYLAR_ASSERT2(false,"swapcontext");
    }
}
//设置当前协程
void Fiber::SetThis(Fiber * f){
    t_fiber = f;
}
//返回当前协程
Fiber::ptr Fiber::GetThis(){
    if(t_fiber){
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    SYLAR_ASSERT(t_fiber == main_fiber.get());
    t_threadFiber = main_fiber;
    return t_fiber->shared_from_this();

}
//协程切换到后台，设置状态为ready
void Fiber::YieldToReady(){
    Fiber::ptr cur = GetThis();
    cur->m_state = READY;
    cur->swapOut();
} 
//同上，状态切换为hold
void Fiber::YieldToHold(){
    Fiber::ptr cur = GetThis();
    cur->m_state = HOLD;
    cur->swapOut();
}
//总协程数
uint64_t Fiber::Totalfibers(){
    return s_fiber_count;
}

void Fiber::MainFunc(){
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur);
    try{
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    }   catch(std::exception& ex){
            cur->m_state = EXCEPT;
            SYLAR_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
                << "fiber_id= " <<cur->getId()
                << std::endl
                << sylar::BackTraceToString();

    }   catch(...){
            cur->m_state = EXCEPT;
            SYLAR_LOG_ERROR(g_logger) << "Fiber Except";
    }
    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->swapOut();

    SYLAR_ASSERT2(false,"never reach fiber_id= " + std::to_string(raw_ptr->getId()));
}
void Fiber::CallerMainFunc(){
Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur);
    try{
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    }   catch(std::exception& ex){
            cur->m_state = EXCEPT;
            SYLAR_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
                << "fiber_id= " <<cur->getId()
                << std::endl
                << sylar::BackTraceToString();

    }   catch(...){
            cur->m_state = EXCEPT;
            SYLAR_LOG_ERROR(g_logger) << "Fiber Except";
    }
    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->back();

    SYLAR_ASSERT2(false,"never reach fiber_id= " + std::to_string(raw_ptr->getId()));
}

}
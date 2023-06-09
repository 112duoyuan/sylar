#ifndef __SYLAR_FIBER_H__
#define __SYLAR_FIBER_H__

#include <memory>
#include <ucontext.h>
#include "thread.h"
#include <functional>

class Scheduler;
namespace sylar{
class Fiber : public std::enable_shared_from_this<Fiber>{
friend class Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    enum State{
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };
private:
    Fiber();

public:
    Fiber(std::function<void()>cb ,size_t stacksize = 0,bool use_caller = false);
    ~Fiber();
    //重置协程函数与状态
    //init,term状态使用
    void reset(std::function<void()> cb);
    
    void swapIn();// 切换到当前协程执行
    void swapOut();//当前切换到后台

    void call();
    void back();

    uint64_t getId() const {return m_id;}
    State getState() const {return m_state;}

public:
    static void SetThis(Fiber* f);//设置当前协程
    static Fiber::ptr GetThis();//返回当前协程
    static void YieldToReady(); //协程切换到后台，设置状态为ready
    static void YieldToHold();//同上，状态切换为hold
    //总协程数
    static uint64_t Totalfibers();
    
    static void MainFunc();
    static void CallerMainFunc();
    static uint64_t GetFiberId();
private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = INIT;

    ucontext_t m_ctx;
    void* m_stack = nullptr;

    std::function<void()> m_cb;
};
}

#endif
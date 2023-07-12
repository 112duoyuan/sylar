#include "thread.h"
#include "log.h"
#include "util.h"
#include <iostream>
namespace sylar{

static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOW";

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");
Semaphore::Semaphore(uint32_t count){
    //初始化一个信号量
    if(sem_init(&m_semaphore,0,2)){
        throw std::logic_error("sem_init error");
    }
}
Semaphore::~Semaphore(){
   sem_destroy(&m_semaphore); 
}
void Semaphore::wait(){
    //将信号量的值减1  申请资源
    int ret = sem_wait(&m_semaphore);
    if(ret == -1){
        throw std::logic_error("sem_wait error");
    }
    
}
void Semaphore::notify(){
    //将信号量的值加1 释放资源
    if(sem_post(&m_semaphore)) {
        throw std::logic_error("sem_post error");
    }
}

Thread* Thread::GetThis(){
    return t_thread;
}
const std::string& Thread::GetName(){
    return t_thread_name;
}
void Thread::SetName(const std::string& name){
    if(t_thread){
        t_thread->m_name = name;
    }
    t_thread_name = name;

}
Thread::Thread(std::function<void()> cb, const std::string&name)
        :m_cb(cb),m_name(name){
    if(name.empty()){
        m_name = "UNKNOW";
    }
    int rt = pthread_create(&m_thread,nullptr,&Thread::run,this);
    if(rt){
        SYLAR_LOG_ERROR(g_logger) << "pthread_create thread fail, rt=" <<rt
            << " name=" << name;
        throw std::logic_error("pthread_create error");
    }
    m_semaphore.wait();

}

Thread::~Thread(){
    if(m_thread){
        //线程分离，与主控线程断开关系
        pthread_detach(m_thread);
    }
}
void Thread::join(){
    if(m_thread){
        //获取某个线程执行结束时返回的数据
        int rt = pthread_join(m_thread,nullptr);
        if(rt){
            SYLAR_LOG_ERROR(g_logger) << "pthread_join thread fail, rt=" << rt
                << " name=" << m_name;
            throw std::logic_error("pthread_join error");
        }
        m_thread = 0;
    }
}   
void * Thread::run(void * arg){
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    thread->m_id = sylar::GetThreadId();
    //设置线程名称 
    // 第一个参数：需要设置/获取 名称的线程；

    // 第二个参数：要设置/获取 名称的buffer；

    // 同样的，要求name 的buffer 空间不能超过16个字节，不然会报错 ERANGE。
    pthread_setname_np(pthread_self(),thread->m_name.substr(0,15).c_str());
    std::function<void()> cb;
    //获取函数
    cb.swap(thread->m_cb);
    thread->m_semaphore.notify();
    cb();
    return 0;
}


}
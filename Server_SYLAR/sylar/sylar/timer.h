#ifndef __SYLAR_TIMER_H__
#define __SYLAR_TIMER_H__
#include <memory>
#include "thread.h"
#include <set>
#include <vector>


namespace sylar{

class TimerManager;
class Timer : public std::enable_shared_from_this<Timer>{
friend class TimerManager;
public:
    typedef std::shared_ptr<Timer>ptr;
    bool cancel();
    bool refresh();
    bool reset(uint64_t ms,bool from_now);

private:
    Timer(uint64_t ms,std::function<void()>cb,
        bool recurring, TimerManager* manager);
    Timer(uint64_t next);

private:
    bool m_recurring  = false;  //是否循环定时器
    uint64_t m_ms = 0; //时间周期
    uint64_t m_next = 0; //精确的执行时间，到期时间
    std::function<void()>m_cb;
    TimerManager* m_manager = nullptr;
private:
    struct Comparator{
        bool operator()(const Timer::ptr& lhs,const Timer::ptr& rhs)const;
    };
};
class TimerManager{
friend class Timer;
public:
    typedef RWMutex RWMutexType;
    typedef Mutex MutexType;
    TimerManager();
    virtual ~TimerManager();

    Timer::ptr addTimer(uint64_t ms,std::function<void()>cb,
                        bool recurring = false);
                        //weak_ptr 
    Timer::ptr addConditionTimer(uint64_t ms,std::function<void()>cb,
                        std::weak_ptr<void>weak_cond,
                        bool recurring = false);
    uint64_t getNextTimer();
    //Expired 过期的意思 重新加入超时timer
    void listExpiredCb(std::vector<std::function<void()>> &cbs);
    bool hasTimer();

protected:
    virtual void onTimerInsertedAtFront() = 0;
    void addTimer(Timer::ptr val,MutexType::Lock& lock);
private:
    bool detectClockRollover(uint64_t now_ms);
private:
    MutexType m_mutex;
    //set自定义排序
    std::set<Timer::ptr,Timer::Comparator>m_timers;
    bool m_tickled = false;
    uint64_t m_previouseTime;//先前时间
};
}



#endif
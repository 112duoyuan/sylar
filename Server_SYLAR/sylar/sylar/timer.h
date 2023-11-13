#ifndef __SYLAR_TIMER_H__
#define __SYLAR_TIMER_H__
#include <memory>
#include "thread.h"
#include <set>
#include <vector>
/*
-------------------------------20231018 read done ---------------------------------
*/


namespace sylar{

class TimerManager;
//需要在类对象的内部中获得一个指向当前对象的 shared_ptr 对象。
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
    uint64_t m_ms = 0; // 执行周期
    uint64_t m_next = 0; //精确的执行时间，到期时间
    std::function<void()>m_cb;
    TimerManager* m_manager = nullptr;
private:
     /**
         * @brief 比较定时器的智能指针的大小(按执行时间排序)
         * @param[in] lhs 定时器智能指针
         * @param[in] rhs 定时器智能指针
         */
    struct Comparator{
        bool operator()(const Timer::ptr& lhs,const Timer::ptr& rhs)const;
    };
};
class TimerManager{
friend class Timer;
public:
    typedef RWMutex RWMutexType;
    TimerManager();
    virtual ~TimerManager();

    Timer::ptr addTimer(uint64_t ms,std::function<void()>cb,
                        bool recurring = false);
    
    //weak_ptr 
        /**
     * @brief 添加条件定时器
     * @param[in] ms 定时器执行间隔时间
     * @param[in] cb 定时器回调函数
     * @param[in] weak_cond 条件
     * @param[in] recurring 是否循环
     */
    Timer::ptr addConditionTimer(uint64_t ms,std::function<void()>cb,
                        std::weak_ptr<void>weak_cond,
                        bool recurring = false);

    /**
     * @brief 到最近一个定时器执行的时间间隔(毫秒)
     */
    //距离下个定时器执行的时间
    uint64_t getNextTimer();

    /**
     * @brief 获取需要执行的定时器的回调函数列表
     * @param[out] cbs 回调函数数组
     */
    void listExpiredCb(std::vector<std::function<void()>> &cbs);
    
        /**
     * @brief 是否有定时器
     */
    bool hasTimer();

protected:
        /**
     * @brief 当有新的定时器插入到定时器的首部,执行该函数
     */
    virtual void onTimerInsertedAtFront() = 0;

        /**
     * @brief 将定时器添加到管理器中
     */
    void addTimer(Timer::ptr val,RWMutexType::WriteLock& lock);
private:

    /**
     * @brief 检测服务器时间是否被调后了
     */
    bool detectClockRollover(uint64_t now_ms);
private:
    RWMutexType m_mutex;

    //set自定义排序
    std::set<Timer::ptr,Timer::Comparator>m_timers;

    //是否触发onTimerInsertedAtFront
    bool m_tickled = false;

    uint64_t m_previouseTime = 0;//上次执行时间
};
}



#endif
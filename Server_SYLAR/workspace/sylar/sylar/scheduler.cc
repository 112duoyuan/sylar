#include "scheduler.h"
#include "log.h"

namespace sylar{
static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

static thread_local Scheduler* t_scheduler = nullptr;
static thread_local Fiber * t_fiber =nullptr;

Scheduler::Scheduler(size_t threads, bool use_caller , const std::string& name){
}
Scheduler::~Scheduler(){

}

Fiber* Scheduler::GetMainFiber(){

}

void Scheduler::start(){

}
void Scheduler::stop(){
    
}



}

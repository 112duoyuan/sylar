#include "../sylar/sylar.h"
#include <unistd.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
int count = 0;
//sylar::RWMutex s_mutex;
sylar::Mutex s_mutex;
void func1(){
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                            << " this.name: "<< sylar::Thread::GetThis()->getName()
                            << " id: " << sylar::GetThreadId()
                            << "this.id" << sylar::Thread::GetThis()->getId();
    for(int i = 0; count < 100; i++){
        //这里有个问题，析构函数去操作解锁会造成线程2阻塞的现象，只能主动释放锁并sleep(1)
        //sylar::RWMutex::WriteLock lock(s_mutex);
        sylar::Mutex::Lock lock(s_mutex);
        count++;
        SYLAR_LOG_INFO(g_logger) << "count= " << count << std::endl;
        lock.unlock();
        sleep(1);
    }

}
void func2() {
    SYLAR_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
}
void func3(){
    SYLAR_LOG_INFO(g_logger) << "=======================================";
}
int main(int argc,char ** argv){
    SYLAR_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/xu/Server_SYLAR/sylar/bin/conf/log3.yml");
    sylar::Config::LoadFromYaml(root);

    std::vector<sylar::Thread::ptr> thrs;
    for(int i = 0; i < 1;++i){
        sylar::Thread::ptr thr(new sylar::Thread(&func1, "name_" + std::to_string(i * 2)));
        sylar::Thread::ptr thr2(new sylar::Thread(&func1, "name_" + std::to_string(i * 2 + 1)));

        thrs.push_back(thr);
        thrs.push_back(thr2);
    }       
    for(size_t i = 0; i< thrs.size();i++){
        thrs[i]->join();
    }
    SYLAR_LOG_INFO(g_logger) << "thread test end";
    return 0;
}
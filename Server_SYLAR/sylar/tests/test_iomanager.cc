#include "../sylar/sylar.h"
#include "../sylar/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
int sock = 0;
void test_fiber(){
    SYLAR_LOG_INFO(g_logger) << "test_fiber";
    sock = socket(AF_INET,SOCK_STREAM,0);
    fcntl(sock,F_SETFL,O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;//IPv4 网络协议的套接字类型
    addr.sin_port = htons(84);
    // inet_pton - convert IPv4 and IPv6 addresses from text to binary form
    inet_pton(AF_INET,"14.119.104.254",&addr.sin_addr.s_addr);
    SYLAR_LOG_INFO(g_logger) << "connect!!!!!";
    if(connect(sock,(const sockaddr *)&addr,sizeof(addr)) == 0){
            SYLAR_LOG_INFO(g_logger) << "connect success!!!!";
    }else if(errno == EINPROGRESS){//Operation now in progress 操作正在进行中
        SYLAR_LOG_INFO(g_logger) << "add event errno=" <<errno << " "
            << strerror(errno);
        sylar::IOManager::GetThis()->addEvent(sock,sylar::IOManager::READ, [](){
            SYLAR_LOG_INFO(g_logger) << "read callback";
        });
        sylar::IOManager::GetThis()->addEvent(sock,sylar::IOManager::WRITE, [](){
            SYLAR_LOG_INFO(g_logger) << "write callback";
            //close(sock);
            sylar::IOManager::GetThis()->cancelEvent(sock,sylar::IOManager::READ);
            close(sock);
        });   
    }else{
        SYLAR_LOG_INFO(g_logger) << "else " << errno << " "<< strerror(errno);
    }
}

void test(){
    std::cout << "EPOLLIN=" <<EPOLLIN
                << " EPOLLOUT" << EPOLLOUT <<std::endl;
    sylar::IOManager iom(2,false,"XU");
    iom.schedule(&test_fiber);
}
sylar::Timer::ptr s_timer;
void test_timer(){
    sylar::IOManager iom(2);
    s_timer = iom.addTimer(1000,[](){
        static int i= 0;
        SYLAR_LOG_INFO(g_logger) << "hello timer i= " << i;
        if(++i == 3){
            s_timer->cancel();
           s_timer->reset(2000,true);
        }
    },true);
}

int main(int argc,char ** argv){
    //test();
    test_timer();
    return 0;
}
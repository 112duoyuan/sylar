//学习init（） 和输出内容以及系统信息
#include <iostream>
#include "../sylar/log.h"
#include "../sylar/util.h"

//#include <thread>
#include <time.h>

int main(int argc, char** argv) {
    sylar::Logger::ptr logger(new sylar::Logger);
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));

    sylar::StdoutLogAppender::ptr std_appender(new sylar::StdoutLogAppender);
    std_appender->setFormatter(fmt);
    std_appender->setLevel(sylar::LogLevel::UNKNOW);    

    //sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("/home/xu/Server_SYLAR/sylar/bin/log.txt"));
    //file_appender->setFormatter(fmt);
    //file_appender->setLevel(sylar::LogLevel::WARN);

    logger->addAppender(std_appender);

    SYLAR_LOG_INFO(logger) << "test macro1";
    SYLAR_LOG_WARN(logger) << "test macro2";
    SYLAR_LOG_DEBUG(logger) << "test macro3";
    SYLAR_LOG_FATAL(logger) << "test macro4";
    SYLAR_LOG_ERROR(logger) << "test macro5";
    //SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    //auto l = sylar::LoggerMgr::GetInstance()->getLogger("aa");
    //std::cout << "l get name"<<l->getName() << std::endl;
    //SYLAR_LOG_INFO(l) << "xxx";
    return 0;
}
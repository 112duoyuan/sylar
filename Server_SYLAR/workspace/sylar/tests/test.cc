//学习init（） 和输出内容以及系统信息
#include <iostream>
#include "../sylar/log.h"
#include "../sylar/util.h"

//#include <thread>
#include <time.h>

int main(int argc, char** argv) {
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
                                                        //%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::UNKNOW);

    logger->addAppender(file_appender);

    SYLAR_LOG_INFO(logger) << "test macro1";
    SYLAR_LOG_WARN(logger) << "test macro2";
    SYLAR_LOG_DEBUG(logger) << "test macro3";
    SYLAR_LOG_FATAL(logger) << "test macro4";

    //SYLAR_LOG_INFO(logger) << "test macro error5";
    SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error6 %s", "aa");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("aa");
    SYLAR_LOG_INFO(l) << "xxx";
    return 0;
}
#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
 #include <stdint.h>
#include <unistd.h>

namespace sylar {

pid_t GetThreadId();
uint32_t GetFiberId();

}


#endif
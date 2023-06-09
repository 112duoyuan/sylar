
#记得安装yaml
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(sylar) #声明cmake工程
#include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)  #编译过程打印编译信息
#set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 c++0x -Wall  -Werror -Wno-unused-function")
#-Who-builtin-macor-redefined 可以试着添加到上面
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x ")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(/home/xu/yaml/yaml-cpp-master/include)

set(LIB_SRC
    sylar/log.cc
    sylar/fiber.cc
    sylar/util.cc
    sylar/config.cc
    sylar/thread.cc
    sylar/scheduler.cc
)

add_library(sylar SHARED ${LIB_SRC}) #SHARED是动态库
#force_redefine_file_macro_for_sources(sylar) #__FILE__ 重定义file相对路径
set(
    LIB_LIB
    sylar
    pthread 
)

add_executable(test1 tests/test.cc)
add_dependencies(test1 sylar)
#force_redefine_file_macro_for_sources(test) #__FILE__ 重定义file相对路径
target_link_libraries(test1 sylar -lyaml-cpp pthread)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
#force_redefine_file_macro_for_sources(test_config) #__FILE__ 重定义file相对路径
target_link_libraries(test_config sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread sylar)
#force_redefine_file_macro_for_sources(test_thread) #__FILE__ 重定义file相对路径
target_link_libraries(test_thread sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_util sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_fiber sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_scheduler sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #库文件的输出路径


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
    sylar/fd_manager.cc
    sylar/log.cc
    sylar/fiber.cc
    sylar/util.cc
    sylar/config.cc
    sylar/thread.cc
    sylar/scheduler.cc
    sylar/iomanager.cc
    sylar/timer.cc
    sylar/hook.cc
    sylar/address.cc
    sylar/socket.cc
    sylar/bytearray.cc
    sylar/http/http.cc
    sylar/http/http11_parser.rl.cc
    sylar/http/httpclient_parser.rl.cc
    sylar/http/http_parser.cc
    sylar/tcp_server.cc
    sylar/stream.cc
    sylar/socket_stream.cc
    sylar/http/http_session.cc
)

add_library(sylar SHARED ${LIB_SRC}) #SHARED是动态库
#force_redefine_file_macro_for_sources(sylar) #__FILE__ 重定义file相对路径
set(
    LIB_LIB
    sylar
    pthread 
    dl
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

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_iomanager sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_hook sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_address sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_socket sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_bytearray sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_http tests/test_http.cc)
add_dependencies(test_http sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_http sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_http_parser tests/test_http_parser.cc)
add_dependencies(test_http_parser sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_http_parser sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(test_tcp_server tests/test_tcp_server.cc)
add_dependencies(test_tcp_server sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(test_tcp_server sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)

add_executable(echo_server examples/echo_server.cc)
add_dependencies(echo_server sylar)
#force_redefine_file_macro_for_sources(test_util) #__FILE__ 重定义file相对路径
target_link_libraries(echo_server sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp pthread)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #库文件的输出路径

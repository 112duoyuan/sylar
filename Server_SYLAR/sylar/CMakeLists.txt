CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(sylar) #声明cmake工程

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

#set(CMAKE_VERBOSE_MAKEFILE ON)  #编译过程打印编译信息
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x ")
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")


include_directories(/home/xu/yaml/yaml-cpp-master/include)


set(BUILD_TEST "ON for complile test" OFF)

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
    sylar/http/http_server.cc
    sylar/http/servlet.cc
    sylar/http/http_connection.cc
)

ragelmaker(sylar/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar)

add_library(sylar SHARED ${LIB_SRC}) #SHARED是动态库
force_redefine_file_macro_for_sources(sylar) #__FILE__ 重定义file相对路径

set(LIBS
        sylar 
        -L/Server_SYLAR/sylar/lib 
        -lyaml-cpp 
        pthread 
        dl
)
if(BUILD_TEST)
sylar_add_executable(test1 tests/test.cc sylar "${LIBS}")

sylar_add_executable(test_config tests/test_config.cc sylar "${LIBS}")

sylar_add_executable(test_util tests/test_util.cc sylar "${LIBS}")

sylar_add_executable(test_fiber tests/test_fiber.cc sylar "${LIBS}")

sylar_add_executable(test_scheduler tests/test_scheduler.cc sylar "${LIBS}")

sylar_add_executable(test_iomanager tests/test_iomanager.cc sylar "${LIBS}")

sylar_add_executable(test_hook tests/test_hook.cc sylar "${LIBS}")

sylar_add_executable(test_address tests/test_address.cc sylar "${LIBS}")

sylar_add_executable(test_socket tests/test_socket.cc sylar "${LIBS}")

sylar_add_executable(test_bytearray tests/test_bytearray.cc sylar "${LIBS}")

sylar_add_executable(test_http tests/test_http.cc sylar "${LIBS}")

sylar_add_executable(test_http_parser tests/test_http_parser.cc sylar "${LIBS}")

sylar_add_executable(test_tcp_server tests/test_tcp_server.cc sylar "${LIBS}")

sylar_add_executable(echo_server sylar/examples/echo_server.cc sylar "${LIBS}")

#sylar_add_executable(test_http_server tests/test_http_server.cc sylar "${LIBS}")

#sylar_add_executable(test_http_connection tests/test_http_connection.cc sylar "${LIBS}")

#sylar_add_executable(test_uri tests/test_uri.cc sylar "${LIBS}")

sylar_add_executable(test_yml tests/test_yml.cc sylar "${LIBS}")
sylar_add_executable(test_thread tests/test_thread.cc sylar "${LIBS}")
endif()


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #库文件的输出路径

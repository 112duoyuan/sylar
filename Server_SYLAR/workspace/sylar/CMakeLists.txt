
#记得安装yaml
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(sylar) #声明cmake工程

set(CMAKE_VERBOSE_MAKEFILE ON)  #编译过程打印编译信息
set(CMAKE_CXX_CFLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 c++0x -Wall -Who-deprecated -Werror -Wno-unused-function")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x ")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(/home/xu/yaml/yaml-cpp-master/include)

set(LIB_SRC
    sylar/log.cc
    sylar/util.cc
    sylar/config.cc
)
add_library(sylar SHARED ${LIB_SRC}) #SHARED是动态库


add_executable(test1 tests/test.cc)
add_dependencies(test1 sylar)

target_link_libraries(test1 sylar -lyaml-cpp)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)

target_link_libraries(test_config sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #库文件的输出路径

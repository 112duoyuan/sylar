
#记得安装yaml
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(sylar) #声明cmake工程

set(CMAKE_VERBOSE_MAKEFILE ON)  #编译过程打印编译信息
set(CMAKE_CXX_CFLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 c++0x -Wall -Who-deprecated -Werror -Wno-unused-function")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

include_directories(.)
include_directories(/Server_SYLAR/sylar/include)
target_link_libraries(/Server_SYLAR/sylar/lib)

#find_library(YAMLCPP libyaml-cpp)
message("***",${YAMLCPP})


set(LIB_SRC
    sylar/log.cc
    sylar/util.cc
    sylar/config.cc
)
#set (CMAKE_LIBRARY_OUTPUT ${PROJECT_SOURCE_DIR}/lib)

add_library(sylar SHARED ${LIB_SRC}) #SHARED是动态库


add_executable(test tests/test.cc)
add_dependencies(test sylar)
#add_executable(bin_test test1 sylar )
target_link_libraries(test sylar ${YAMLCPP})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
#target_link_libraries(test_config sylar ${YAMLCPP})
target_link_libraries(test_config sylar -L/Server_SYLAR/sylar/lib -lyaml-cpp)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #库文件的输出路径

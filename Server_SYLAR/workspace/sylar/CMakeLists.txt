CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(sylar) #声明cmake工程

set(CMAKE_VERBOSE_MAKEFILE ON)  #编译过程打印编译信息
set(CMAKE_CXX_CFLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 c++0x -Wall -Who-deprecated -Werror -Wno-unused-function")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

include_directories(/apps/sylar/include)
#set(
  #  sylar/log.cc
 #   sylar/util.cc
#)

set(LIB_SRC
    sylar/log.cc
    sylar/util.cc
    sylar/config.cc
)
#set (CMAKE_LIBRARY_OUTPUT ${PROJECT_SOURCE_DIR}/lib)

add_library(sylar SHARED ${LIB_SRC}) #SHARED是动态库
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (sylar_static PROPERTIES OUTPUT_NAME "sylar")


add_executable(test tests/test.cc)
add_dependencies(test sylar)
target_link_libraries(test sylar)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
target_link_libraries(test_config sylar)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #库文件的输出路径

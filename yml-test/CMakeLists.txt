CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(test-x)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_CFLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 c++0x -Wall -Who-deprecated -Werror -Wno-unused-function")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

include_directories(/home/xu/yaml/yaml-cpp-master/include)

add_executable(test1 yamltest.cpp)
target_link_libraries(test1 -lyaml-cpp)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

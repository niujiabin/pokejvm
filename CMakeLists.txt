cmake_minimum_required(VERSION 3.12)
project(pokejvm)

set(CMAKE_CXX_STANDARD 14)

add_executable(CLionWorkSpace main.cpp src/parse/ParentClass.cpp src/parse/ClassParse.cpp src/parse/ClassParse.hpp src/class/ClassFile.cpp)

find_package(Threads REQUIRED)
target_link_libraries(CLionWorkSpace Threads::Threads)
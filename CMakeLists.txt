cmake_minimum_required(VERSION 3.6)
project(FutureAndPromise)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp )
add_executable(FutureAndPromise ${SOURCE_FILES})
target_link_libraries (FutureAndPromise pthread)
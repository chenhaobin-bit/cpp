cmake_minimum_required(VERSION 3.10)
project(MyDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # <-- 加上这句

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(MyDemo
    ../src/3.cpp
)

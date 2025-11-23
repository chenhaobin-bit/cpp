cmake_minimum_required(VERSION 3.10)
project(MyDemo)

include_directories(include)

add_executable(MyDemo 
    src/main.cpp
)

set_target_properties(MyDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output
)
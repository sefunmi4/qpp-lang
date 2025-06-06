cmake_minimum_required(VERSION 3.10)
project(qpp-lang)

set(CMAKE_CXX_STANDARD 17)

add_executable(qppc
    tools/qppc.cpp
    runtime/engine.cpp
    runtime/memory.cpp
    runtime/scheduler.cpp)

target_include_directories(qppc PRIVATE include)

add_executable(qpp-run
    tools/qpp-run.cpp
    runtime/engine.cpp
    runtime/memory.cpp
    runtime/scheduler.cpp)

target_include_directories(qpp-run PRIVATE include)

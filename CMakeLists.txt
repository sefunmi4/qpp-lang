cmake_minimum_required(VERSION 3.10)
project(qpp-lang)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTING "Build tests" ON)

add_library(qpp_runtime
    runtime/engine.cpp
    runtime/memory.cpp
    runtime/scheduler.cpp
    runtime/wavefunction.cpp
)

target_include_directories(qpp_runtime PUBLIC include)

add_executable(qppc tools/qppc.cpp)
target_link_libraries(qppc PRIVATE qpp_runtime)

add_executable(qpp-run tools/qpp-run.cpp)
target_link_libraries(qpp-run PRIVATE qpp_runtime)

if(BUILD_TESTING)
    enable_testing()
    add_executable(wavefunction_test tests/wavefunction_test.cpp)
    target_link_libraries(wavefunction_test PRIVATE qpp_runtime)
    add_test(NAME wavefunction_test COMMAND wavefunction_test)

    add_executable(wavefunction_extra_test tests/wavefunction_extra_test.cpp)
    target_link_libraries(wavefunction_extra_test PRIVATE qpp_runtime)
    add_test(NAME wavefunction_extra_test COMMAND wavefunction_extra_test)

    add_executable(scheduler_test tests/scheduler_test.cpp)
    target_link_libraries(scheduler_test PRIVATE qpp_runtime)
    add_test(NAME scheduler_test COMMAND scheduler_test)

    add_executable(memory_test tests/memory_test.cpp)
    target_link_libraries(memory_test PRIVATE qpp_runtime)
    add_test(NAME memory_test COMMAND memory_test)
endif()

cmake_minimum_required(VERSION 3.10)
project(qpp-lang)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTING "Build tests" ON)
option(USE_CUDA "Enable CUDA acceleration" OFF)

add_library(qpp_runtime
    runtime/engine.cpp
    runtime/memory.cpp
    runtime/scheduler.cpp
    runtime/hardware_api.cpp
    runtime/device.cpp
    runtime/wavefunction.cpp
)

if(USE_CUDA)
    enable_language(CUDA)
    target_sources(qpp_runtime PRIVATE runtime/gpu_kernels.cu)
    target_compile_definitions(qpp_runtime PUBLIC USE_CUDA)
endif()

target_include_directories(qpp_runtime PUBLIC include PRIVATE runtime)

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
    
    add_executable(wavefunction_collapse_test tests/wavefunction_collapse_test.cpp)
    target_link_libraries(wavefunction_collapse_test PRIVATE qpp_runtime)
    add_test(NAME wavefunction_collapse_test COMMAND wavefunction_collapse_test)

    add_executable(wavefunction_twoqubit_measure_test tests/wavefunction_twoqubit_measure_test.cpp)
    target_link_libraries(wavefunction_twoqubit_measure_test PRIVATE qpp_runtime)
    add_test(NAME wavefunction_twoqubit_measure_test COMMAND wavefunction_twoqubit_measure_test)

    add_executable(scheduler_test tests/scheduler_test.cpp)
    target_link_libraries(scheduler_test PRIVATE qpp_runtime)
    add_test(NAME scheduler_test COMMAND scheduler_test)

    add_executable(memory_test tests/memory_test.cpp)
    target_link_libraries(memory_test PRIVATE qpp_runtime)
    add_test(NAME memory_test COMMAND memory_test)

    add_executable(memory_import_test tests/memory_import_test.cpp)
    target_link_libraries(memory_import_test PRIVATE qpp_runtime)
    add_test(NAME memory_import_test COMMAND memory_import_test)

    add_executable(memory_reuse_test tests/memory_reuse_test.cpp)
    target_link_libraries(memory_reuse_test PRIVATE qpp_runtime)
    add_test(NAME memory_reuse_test COMMAND memory_reuse_test)

    add_executable(scheduler_pause_test tests/scheduler_pause_test.cpp)
    target_link_libraries(scheduler_pause_test PRIVATE qpp_runtime)
    add_test(NAME scheduler_pause_test COMMAND scheduler_pause_test)

    add_executable(hardware_api_test tests/hardware_api_test.cpp)
    target_link_libraries(hardware_api_test PRIVATE qpp_runtime)
    add_test(NAME hardware_api_test COMMAND hardware_api_test)

    add_executable(device_fallback_test tests/device_fallback_test.cpp)
    target_link_libraries(device_fallback_test PRIVATE qpp_runtime)
    add_test(NAME device_fallback_test COMMAND device_fallback_test)

    add_test(NAME demo_integration
        COMMAND ${CMAKE_SOURCE_DIR}/tests/demo_integration.sh)
    add_test(NAME examples_compile_test
        COMMAND ${CMAKE_SOURCE_DIR}/tests/examples_compile_test.sh)
endif()

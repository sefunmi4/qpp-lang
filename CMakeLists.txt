cmake_minimum_required(VERSION 3.10)
project(qpp-lang)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)

option(BUILD_TESTING "Build tests" ON)
option(USE_CUDA "Enable CUDA acceleration" OFF)

add_library(qpp_runtime
    runtime/engine.cpp
    runtime/memory.cpp
    runtime/scheduler.cpp
    runtime/hardware_api.cpp
    runtime/device.cpp
    runtime/patterns.cpp
    runtime/hardware_profile.cpp
    runtime/wavefunction.cpp
    runtime/partitioner.cpp
    runtime/sparse_wavefunction.cpp
    runtime/disk_pager.cpp
    runtime/runtime_config.cpp
    runtime/quidd.cpp
)

if(USE_CUDA)
    enable_language(CUDA)
    target_sources(qpp_runtime PRIVATE runtime/gpu_kernels.cu)
    target_compile_definitions(qpp_runtime PUBLIC USE_CUDA)
endif()

if(OpenMP_CXX_FOUND)
    target_link_libraries(qpp_runtime PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(qpp_runtime PUBLIC include PRIVATE runtime)

add_executable(qppc tools/qppc.cpp)
target_link_libraries(qppc PRIVATE qpp_runtime)
target_include_directories(qppc PRIVATE include)

add_executable(qpp-run tools/qpp-run.cpp)
target_link_libraries(qpp-run PRIVATE qpp_runtime)

add_executable(quidd_benchmark tools/quidd_benchmark.cpp)
target_link_libraries(quidd_benchmark PRIVATE qpp_runtime)

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

    add_executable(fused_gate_test tests/fused_gate_test.cpp)
    target_link_libraries(fused_gate_test PRIVATE qpp_runtime)
    add_test(NAME fused_gate_test COMMAND fused_gate_test)
    
    add_executable(wavefunction_sparse_test tests/wavefunction_sparse_test.cpp)
    target_link_libraries(wavefunction_sparse_test PRIVATE qpp_runtime)
    add_test(NAME wavefunction_sparse_test COMMAND wavefunction_sparse_test)


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

    add_executable(resonance_zone_cache_test tests/resonance_zone_cache_test.cpp)
    target_link_libraries(resonance_zone_cache_test PRIVATE qpp_runtime)
    add_test(NAME resonance_zone_cache_test COMMAND resonance_zone_cache_test)

    add_executable(pattern_detection_test tests/pattern_detection_test.cpp)
    target_link_libraries(pattern_detection_test PRIVATE qpp_runtime)
    add_test(NAME pattern_detection_test COMMAND pattern_detection_test)
    
    add_executable(low_rank_factorization_test tests/low_rank_factorization_test.cpp)
    target_link_libraries(low_rank_factorization_test PRIVATE qpp_runtime)
    add_test(NAME low_rank_factorization_test COMMAND low_rank_factorization_test)

    add_executable(quidd_conversion_test tests/quidd_conversion_test.cpp)
    target_link_libraries(quidd_conversion_test PRIVATE qpp_runtime)
    add_test(NAME quidd_conversion_test COMMAND quidd_conversion_test)

    add_executable(checkpoint_test tests/checkpoint_test.cpp)
    target_link_libraries(checkpoint_test PRIVATE qpp_runtime)
    add_test(NAME checkpoint_test COMMAND checkpoint_test)
    
    add_executable(disk_paging_test tests/disk_paging_test.cpp)
    target_link_libraries(disk_paging_test PRIVATE qpp_runtime)
    add_test(NAME disk_paging_test COMMAND disk_paging_test)

    add_executable(memory_usage_test tests/memory_usage_test.cpp)
    target_link_libraries(memory_usage_test PRIVATE qpp_runtime)
    add_test(NAME memory_usage_test COMMAND memory_usage_test)

    add_executable(scheduler_pause_test tests/scheduler_pause_test.cpp)
    target_link_libraries(scheduler_pause_test PRIVATE qpp_runtime)
    add_test(NAME scheduler_pause_test COMMAND scheduler_pause_test)

    add_executable(hardware_api_test tests/hardware_api_test.cpp)
    target_link_libraries(hardware_api_test PRIVATE qpp_runtime)
    add_test(NAME hardware_api_test COMMAND hardware_api_test)

    add_executable(device_fallback_test tests/device_fallback_test.cpp)
    target_link_libraries(device_fallback_test PRIVATE qpp_runtime)
    add_test(NAME device_fallback_test COMMAND device_fallback_test)

    add_executable(partitioner_test tests/partitioner_test.cpp)
    target_link_libraries(partitioner_test PRIVATE qpp_runtime)
    add_test(NAME partitioner_test COMMAND partitioner_test)

    add_executable(sparse_wavefunction_test tests/sparse_wavefunction_test.cpp)
    target_link_libraries(sparse_wavefunction_test PRIVATE qpp_runtime)
    add_test(NAME sparse_wavefunction_test COMMAND sparse_wavefunction_test)

    add_test(NAME demo_integration
        COMMAND ${CMAKE_SOURCE_DIR}/tests/demo_integration.sh)
    add_test(NAME examples_compile_test
        COMMAND ${CMAKE_SOURCE_DIR}/tests/examples_compile_test.sh)
    add_test(NAME compiler_opt_test
        COMMAND ${CMAKE_SOURCE_DIR}/tests/compiler_opt_test.sh)
    add_test(NAME engine_dispatch_test
        COMMAND ${CMAKE_SOURCE_DIR}/tests/engine_dispatch_test.sh)
endif()

project(ht-bench)
cmake_minimum_required(VERSION 3.15)

list(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
find_library(XXHASH_LIBRARY xxhash)

set(BENCH)

macro(add_bench_impl exe def)
    add_executable(${exe} bench.cpp)
    set_target_properties(${exe} PROPERTIES COMPILE_FLAGS ${def})
    target_link_libraries(${exe} ${XXHASH_LIBRARY} ${ARGN})

    add_custom_target(${exe}-run
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${exe}
        DEPENDS ${exe})
    list(APPEND BENCH ${exe}-run)
endmacro()
macro(add_bench exe def)
    add_bench_impl(${exe} "${def}")
    add_bench_impl(${exe}-preallocate "${def} -DPREALLOCATE")
    add_bench_impl(${exe}-preallocate-block "${def} -DPREALLOCATE_BLOCK=65535")
endmacro()

add_bench(bench-sparse_hash -DSPARSE_HASH)
add_bench(bench-dense_hash -DDENSE_HASH)

add_bench(bench-tsl_sparse -DTSL_SPARSE)
target_include_directories(bench-tsl_sparse PRIVATE
    contrib/sparse-map/include)

add_bench(bench-robin_hood -DROBIN_HOOD_HASHING)
target_include_directories(bench-robin_hood PRIVATE
    contrib/robin-hood-hashing/src/include)

add_bench(bench-parallel_hashmap -DPARALLEL_HASHMAP)
target_include_directories(bench-parallel_hashmap
    PRIVATE contrib/parallel-hashmap)

add_bench(bench-sparsepp -DSPARSEPP)
target_include_directories(bench-sparsepp
    PRIVATE contrib/sparsepp)

find_package(folly REQUIRED)
find_package(Threads REQUIRED)
add_bench(bench-f14 -DF14 Folly::folly)
add_bench(bench-f14_vector -DF14_VECTOR Folly::folly)
add_bench(bench-f14_node -DF14_NODE Folly::folly)

message(STATUS "The following benchmarks has been added: ${BENCH}")
add_custom_target(bench ALL
    COMMENT "Run all benchmarks"
    DEPENDS ${BENCH})

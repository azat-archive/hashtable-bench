project(ht-bench)
cmake_minimum_required(VERSION 3.15)

list(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
find_library(XXHASH_LIBRARY xxhash)

set(CMAKE_CXX_STANDARD 20)

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
    add_bench_impl(${exe}-seq "${def} -DSEQ")
    add_bench_impl(${exe}-preallocate "${def} -DPREALLOCATE")
    add_bench_impl(${exe}-preallocate-block "${def} -DPREALLOCATE_BLOCK=65535")
endmacro()

include_directories(contrib/sparse-map/include)
include_directories(contrib/robin-hood-hashing/src/include)
include_directories(contrib/parallel-hashmap)
include_directories(contrib/sparsepp)

add_bench(bench-sparse_hash -DSPARSE_HASH)
add_bench(bench-dense_hash -DDENSE_HASH)
add_bench(bench-tsl_sparse -DTSL_SPARSE)
add_bench(bench-robin_hood -DROBIN_HOOD_HASHING)
add_bench(bench-robin_hood-max95 "-DROBIN_HOOD_HASHING -DMAX_LOAD_FACTOR=95")
add_bench(bench-parallel_hashmap -DPARALLEL_HASHMAP)
add_bench(bench-sparsepp -DSPARSEPP)

find_package(folly REQUIRED)
find_package(Threads REQUIRED)
add_bench(bench-f14 -DF14 Folly::folly)
add_bench(bench-f14_vector -DF14_VECTOR Folly::folly)
add_bench(bench-f14_node -DF14_NODE Folly::folly)

message(STATUS "The following benchmarks has been added: ${BENCH}")
add_custom_target(bench ALL
    COMMENT "Run all benchmarks"
    DEPENDS ${BENCH})

#include <xxhash.h>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <sys/time.h>
#include <sys/resource.h>
#include <string>

#if defined(SPARSE_HASH)
#include <sparsehash/sparse_hash_map>
using map_t = google::sparse_hash_map<uint64_t, uint64_t>;
static std::string name("sparse_hash");
#elif defined(DENSE_HASH)
#include <sparsehash/dense_hash_map>
using map_t = google::dense_hash_map<uint64_t, uint64_t>;
static std::string name("dense_hash");
#elif defined(F14)
#include <folly/container/F14Map.h>
using map_t = folly::F14FastMap<uint64_t, uint64_t>;
static std::string name("f14");
#elif defined(F14_VECTOR)
#include <folly/container/F14Map.h>
using map_t = folly::F14VectorMap<uint64_t, uint64_t>;
static std::string name("f14_vector");
#elif defined(F14_NODE)
#include <folly/container/F14Map.h>
using map_t = folly::F14NodeMap<uint64_t, uint64_t>;
static std::string name("f14_node");
#elif defined(TSL_SPARSE)
#include <tsl/sparse_map.h>
using map_t = tsl::sparse_map<uint64_t, uint64_t>;
static std::string name("tsl_sparse");
#elif defined(ROBIN_HOOD_HASHING)
#include <robin_hood.h>
using map_t = robin_hood::unordered_map<uint64_t, uint64_t>;
static std::string name("robin_hood");
#elif defined(PARALLEL_HASHMAP)
#include <parallel_hashmap/phmap.h>
using map_t = phmap::flat_hash_map<uint64_t, uint64_t>;
static std::string name("parallel_hashmap");
#elif defined(SPARSEPP)
#include <sparsepp/spp.h>
using map_t = spp::sparse_hash_map<uint64_t, uint64_t>;
static std::string name("sparsepp");
#else
#error No define
#endif

static uint64_t constexpr KEYS = 1e7;


template <class F, class ...Args>
auto elapsed(const F &f, Args &&... args)
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    f(std::forward<Args>(args)...);
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    return elapsed_seconds.count();
}

static inline uint64_t hash(uint64_t key)
{ return XXH64((void *)&key, sizeof(key), 0); }

template <class C>
void insert(C &c)
{
    for (uint64_t i = 0; i < KEYS; ++i) {
#ifdef PREALLOCATE_BLOCK
        if ((i % PREALLOCATE_BLOCK) == 0) {
            c.resize(c.size() + PREALLOCATE_BLOCK);
        }
#endif

#ifdef ROBIN_HOOD_HASHING
        c.insert(robin_hood::pair<uint64_t, uint64_t>(hash(i), i));
#else
        c.insert(std::make_pair(hash(i), i));
#endif
    }
}

template <class C>
void find(C &c)
{
    uint64_t found = 0;
    for (uint64_t i = 0; i < KEYS; ++i) {
        found += (c.find(hash(i)) != c.end());
    }
    if (found != KEYS) {
        throw std::runtime_error("mismatch");
    }
}

double tv2usec(const struct timeval &tv)
{ return tv.tv_sec * 1e6 + tv.tv_usec; }

int main(int argc, char **argv)
{
    std::cout << name << "/keys: " << KEYS << "\n";

    map_t map;
#ifdef PREALLOCATE
    map.resize(KEYS);
#endif
#ifdef DENSE_HASH
    map.set_empty_key(UINT64_MAX);
#endif

    std::cout << name << "/insert: " << elapsed([&]() { insert(map); }) << "\n";
    std::cout << name << "/find  : " << elapsed([&]() { find(map); }) << "\n";

    struct rusage rusage_data;
    if (getrusage(RUSAGE_SELF, &rusage_data)) {
        throw std::runtime_error("getrusage");
    }

    double cpu = 0.;
    cpu += tv2usec(rusage_data.ru_utime); // user
    cpu += tv2usec(rusage_data.ru_stime); // system

    std::cout << name << "/maxrss: " << (rusage_data.ru_maxrss>>10) << "MiB \n";
    std::cout << name << "/time:   " << cpu/1e3 << " msec (user+sys)\n";

    return EXIT_SUCCESS;
}

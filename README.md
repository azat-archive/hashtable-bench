### 10'000'000 elements

| container                                                                                     | insert     | find      | maxrss       |
| -----------------------------                                                                 | ---------- | --------- | ------------ |
| [`google::sparse_hash_map`](https://github.com/sparsehash)                                    | 6.39060    | 1.818     | **173MiB**   |
| [`tsl::sparse_map`](https://github.com/Tessil/sparse-map)                                     | 3.51809    | 1.76211   | 218MiB       |
| [`sparsepp`](https://github.com/greg7mdp/sparsepp)                                            | 6.24339    | 1.78245   | 229MiB       |
| [`folly::F14VectorMap`](https://github.com/facebook/folly/blob/master/folly/container/F14.md) | 1.85791    | 0.79995   | 264MiB       |
| [`folly::F14FastMap`](https://github.com/facebook/folly/blob/master/folly/container/F14.md)   | 2.34196    | 0.99701   | 392MiB       |
| [`robin_hood::unordered_map`](https://github.com/martinus/robin-hood-hashing)                 | 1.33476    | 0.86814   | 411MiB       |
| [`phmap::flat_hash_map`](https://github.com/greg7mdp/parallel-hashmap)                        | 1.34150    | 1.39412   | 411MiB       |
| [`folly::F14NodeMap`](https://github.com/facebook/folly/blob/master/folly/container/F14.md)   | 2.42989    | 0.79106   | 441MiB       |
| [`google::dense_hash_map`](https://github.com/sparsehash)                                     | 1.75558    | 0.99429   | 770MiB       |

### 1'000'000'000 elements

| container                                                                                     | insert     | find      | maxrss       |
| -----------------------------                                                                 | ---------- | --------- | ------------ |
| [`google::sparse_hash_map`](https://github.com/sparsehash)                                    | 717.639    | 227.964   | **17575MiB** |
| [`tsl::sparse_map`](https://github.com/Tessil/sparse-map)                                     | 306.876    | 113.303   | 20071MiB     |
| [`sparsepp`](https://github.com/greg7mdp/sparsepp)                                            | 390.99     | 142.251   | 19807MiB     |
| [`folly::F14VectorMap`](https://github.com/facebook/folly/blob/master/folly/container/F14.md) | 134.315    | 49.4222   | 32921MiB     |
| [`folly::F14FastMap`](https://github.com/facebook/folly/blob/master/folly/container/F14.md)   | 140.811    | 61.6684   | 49304MiB     |
| [`robin_hood::unordered_map`](https://github.com/martinus/robin-hood-hashing)                 | 114.843    | 55.5778   | 52227MiB     |
| [`phmap::flat_hash_map`](https://github.com/greg7mdp/parallel-hashmap)                        | 96.6087    | 86.0701   | 52227MiB     |
| [`folly::F14NodeMap`](https://github.com/facebook/folly/blob/master/folly/container/F14.md)   | 195.283    | 49.7657   | 49288MiB     |
| [`google::dense_hash_map`](https://github.com/sparsehash)                                     | 89.6095    | 58.1243   | 49155MiB     |

*NOTE: This numbers was generated on a different machine then 10'000'000 elements*

#include "MurmurHash.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

                    int MurmurHash::initialize() {
                        return initialize(DEFAULT_SEED);
                    }

                    int MurmurHash::initialize(int seed) {
                        return seed;
                    }

                    int MurmurHash::update(int hash, int value) {
                        const int c1 = 0xCC9E2D51;
                        const int c2 = 0x1B873593;
                        const int r1 = 15;
                        const int r2 = 13;
                        const int m = 5;
                        const int n = 0xE6546B64;

                        int k = value;
                        k = k * c1;
                        k = (k << r1) | (static_cast<int>(static_cast<unsigned int>(k) >> (32 - r1)));
                        k = k * c2;

                        hash = hash ^ k;
                        hash = (hash << r2) | (static_cast<int>(static_cast<unsigned int>(hash) >> (32 - r2)));
                        hash = hash * m + n;

                        return hash;
                    }

                    int MurmurHash::update(int hash, void *value) {
                        return update(hash, value != nullptr ? value->hashCode() : 0);
                    }

                    int MurmurHash::finish(int hash, int numberOfWords) {
                        hash = hash ^ (numberOfWords * 4);
                        hash = hash ^ (static_cast<int>(static_cast<unsigned int>(hash) >> 16));
                        hash = hash * 0x85EBCA6B;
                        hash = hash ^ (static_cast<int>(static_cast<unsigned int>(hash) >> 13));
                        hash = hash * 0xC2B2AE35;
                        hash = hash ^ (static_cast<int>(static_cast<unsigned int>(hash) >> 16));
                        return hash;
                    }

template<typename T>
                    int MurmurHash::hashCode(T data[], int seed) {
                        int hash = initialize(seed);
                        for (auto value : data) {
                            hash = update(hash, value);
                        }

                        hash = finish(hash, sizeof(data) / sizeof(data[0]));
                        return hash;
                    }

                    MurmurHash::MurmurHash() {
                    }
                }
            }
        }
    }
}

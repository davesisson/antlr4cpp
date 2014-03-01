#include "MultiMap.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {
                    virtual void MultiMap::map(K key, V value) {
                        std::vector<V> elementsForKey = get(key);
                        if (elementsForKey.empty()) {
                            elementsForKey = std::vector<V>();
                            std::map<K, std::list<V>>::put(key, elementsForKey);
                        }
                        elementsForKey.push_back(value);
                    }
                    virtual std::vector<Pair<K, V>*> getPairs() {
                        std::vector<Pair<K, V>*> pairs = std::vector<Pair<K, V>*>();
                        for (K key : keySet()) {
                            for (V value : get(key)) {
                                pairs.push_back(new Pair<K, V>(key, value));
                            }
                        }
                        return pairs;
                    }
                }
            }
        }
    }
}

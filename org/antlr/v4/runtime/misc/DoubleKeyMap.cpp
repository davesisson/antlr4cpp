#include "DoubleKeyMap.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {
                    
                    virtual DoubleKeyMap::Value put(Key1 k1, Key2 k2, Value v) {
                        std::map<Key2, Value> *data2 = data->get(k1);
                        Value prev = nullptr;
                        if (data2 == nullptr) {
                            data2 = new LinkedHashMap<Key2, Value>();
                            data->put(k1, data2);
                        } else {
                            prev = data2->get(k2);
                        }
                        data2->put(k2, v);
                        return prev;
                    }
                    
                    virtual DoubleKeyMap::Value get(Key1 k1, Key2 k2) {
                        Map<Key2, Value> *data2 = data->get(k1);
                        if (data2 == nullptr) {
                            return nullptr;
                        }
                        return data2->get(k2);
                    }
                    virtual std::map<Key2, Value> * DoubleKeyMap::get(Key1 k1) {
                        return data->get(k1);
                    }
                    
                    virtual std::set<Value> *DoubleKeyMap::values(Key1 k1) {
                        Map<Key2, Value> *data2 = data->get(k1);
                        if (data2 == nullptr) {
                            return nullptr;
                        }
                        return data2->values();
                    }
                    virtual std::set<Key1> *DoubleKeyMap::keySet() {
                        return data->keySet();
                    }
                    virtual std::set<Key2> *DoubleKeyMap::keySet(Key1 k1) {
                        Map<Key2, Value> *data2 = data->get(k1);
                        if (data2 == nullptr) {
                            return nullptr;
                        }
                        return data2->keySet();
                    }
                    void DoubleKeyMap::InitializeInstanceFields() {
                        data = new java::util::LinkedHashMap<Key1, java::util::Map<Key2, Value>*>();
                    }
                }
            }
        }
    }
}

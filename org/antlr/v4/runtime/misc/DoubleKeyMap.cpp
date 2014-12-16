#include "DoubleKeyMap.h"

/*
 * [The "BSD license"]
 *  Copyright (c) 2013 Terence Parr
 *  Copyright (c) 2013 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {
                    
                    
                    template<typename Key1, typename Key2, typename Value>
                    Value put(Key1 k1, Key2 k2, Value v) {
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
                    
                    
                    template<typename Key1, typename Key2, typename Value>
                    Value get(Key1 k1, Key2 k2) {
                        Map<Key2, Value> *data2 = data->get(k1);
                        if (data2 == nullptr) {
                            return nullptr;
                        }
                        return data2->get(k2);
                    }
                    
                    template<typename Key1, typename Key2, typename Value>
                    std::map<Key2, Value> * DoubleKeyMap::get(Key1 k1) {
                        return data->get(k1);
                    }
                    
                    template<typename Key1, typename Key2, typename Value>
                    std::set<Value> *DoubleKeyMap::values(Key1 k1) {
                        Map<Key2, Value> *data2 = data->get(k1);
                        if (data2 == nullptr) {
                            return nullptr;
                        }
                        return data2->values();
                    }
                    
                   
                    template<typename Key1, typename Key2, typename Value>
                    std::set<Key1> * DoubleKeyMap::keySet() {
                        return data->keySet();
                    }
                    
                    template<typename Key1, typename Key2, typename Value>
                    virtual std::set<Key2> *DoubleKeyMap::keySet(Key1 k1) {
                        Map<Key2, Value> *data2 = data->get(k1);
                        if (data2 == nullptr) {
                            return nullptr;
                        }
                        return data2->keySet();
                    }
                    
                    template<typename Key1, typename Key2, typename Value>
                    void DoubleKeyMap::InitializeInstanceFields() {
                        data = new java::util::LinkedHashMap<Key1, java::util::Map<Key2, Value>*>();
                    }
                }
            }
        }
    }
}

#pragma once

#include "AbstractEqualityComparator.h"
#include "ObjectEqualityComparator.h"
#include "MurmurHash.h"
#include "Exceptions.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "vectorhelper.h"
#include <map>
#include "StringBuilder.h"


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


                    /// <summary>
                    /// A limited map (many unsupported operations) that lets me use
                    ///  varying hashCode/equals.
                    /// </summary>
                    template<typename K, typename V>
                    class FlexibleHashMap : public std::map<K, V> {
                    public:
                        template<typename Key, typename Value>
                        class Entry {
                        public:
                            const Key key;
                            Value value;

                            Entry(Key key, Value value) : key(key) {
                                this->value = value;
                            }

                            virtual std::wstring toString() {
                                return key->toString() + std::wstring(L":") + value->toString();
                            }
                        };

                    public:
                        static const int INITAL_CAPACITY = 16; // must be power of 2
                        static const int INITAL_BUCKET_CAPACITY = 8;
                        static const double LOAD_FACTOR;

                    protected:
                        AbstractEqualityComparator<K> *const comparator;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected java.util.LinkedList<Entry<K, V>>[] buckets;
                        std::list<Entry<K, V>*> *buckets;

                        /// <summary>
                        /// How many elements in set </summary>
                        int n;

                        int threshold; // when to expand

                        int currentPrime; // jump by 4 primes each expand or whatever
                        int initialBucketCapacity;
                    private:
                        void InitializeInstanceFields() {
                            n = 0;
                            threshold = static_cast<int>(INITAL_CAPACITY * LOAD_FACTOR);
                            currentPrime = 1;
                            initialBucketCapacity = INITAL_BUCKET_CAPACITY;
                        }
                    public:
                        FlexibleHashMap() {
                        }

                        template<typename T1>
                        FlexibleHashMap(AbstractEqualityComparator<T1> *comparator) { //this(comparator, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                        }

                        template<typename T1>
                        FlexibleHashMap(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity) : comparator(ObjectEqualityComparator::INSTANCE) {
                            InitializeInstanceFields();
                            if (comparator == nullptr) {
                            }

                            this->comparator = comparator;
                            this->buckets = createEntryListArray(initialBucketCapacity);
                            this->initialBucketCapacity = initialBucketCapacity;
                        }

                    private:
                        //template<typename K, typename V>
                        static std::list<Entry<K, V>*> *createEntryListArray(int length) {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                            std::list<Entry<K, V>*> *result = static_cast<std::list<Entry<K, V>*>[]>(new std::list<V>[length]);
                            return result;
                        }

                    protected:
                        virtual int getBucket(K key) {
                            int hash = comparator->hashCode(key);
                            int b = hash & (buckets->length - 1); // assumes len is power of 2
                            return b;
                        }

                    public:
                        virtual V get(void *key)  {
                            K typedKey = static_cast<K>(key);
                            if (key == nullptr) {
                                return nullptr;
                            }
                            int b = getBucket(typedKey);
                            std::list<Entry<K, V>*> bucket = buckets[b];
                            if (bucket.empty()) { // no bucket
                                return nullptr;
                            }
                            for (auto e : bucket) {
                                if (comparator->equals(e.key, typedKey)) {
                                    return e.value;
                                }
                            }
                            return nullptr;
                        }

                        virtual V put(K key, V value)  {
                            if (key == nullptr) {
                                return nullptr;
                            }
                            if (n > threshold) {
                                expand();
                            }
                            int b = getBucket(key);
                            std::list<Entry<K, V>*> bucket = buckets[b];
                            if (bucket.empty()) {
                                bucket = buckets[b] = std::list<Entry<K, V>*>();
                            }
                            for (auto e : bucket) {
                                if (comparator->equals(e.key, key)) {
                                    V prev = e.value;
                                    e.value = value;
                                    n++;
                                    return prev;
                                }
                            }
                            // not there
                            bucket.push_back(new Entry<K, V>(key, value));
                            n++;
                            return nullptr;
                        }

                        virtual V remove(void *key)  {
                            throw new UnsupportedOperationException();
                        }

                        template<typename T1, typename T2>
                        void putAll(std::map<T1, T2> *m)  {
                            throw new UnsupportedOperationException();
                        }

                        virtual std::vector<K> *keySet()  {
                            throw new UnsupportedOperationException();
                        }

                        virtual std::vector<V> *values()  {
                            std::vector<V> a = VectorHelper::VectorWithReservedSize<V>(size());
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }
                                for (auto e : bucket) {
                                    a.push_back(e.value);
                                }
                            }
                            return a;
                        }
#ifdef TODO
                        virtual std::vector<std::map::Entry<K, V>*> *entrySet()  {
                            throw new UnsupportedOperationException();
                        }
#endif

                        virtual bool containsKey(void *key)  {
                            return get(key) != nullptr;
                        }

                        virtual bool containsValue(void *value)  {
                            throw new UnsupportedOperationException();
                        }

                        virtual int hashCode()  {
                            int hash = MurmurHash::initialize();
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }
                                for (auto e : bucket) {
                                    if (e == nullptr) {
                                        break;
                                    }
                                    hash = MurmurHash::update(hash, comparator->hashCode(e.key));
                                }
                            }

                            hash = MurmurHash::finish(hash, size());
                            return hash;
                        }

                        virtual bool equals(void *o)  {
                            throw UnsupportedOperationException();
                        }

                    protected:
                        virtual void expand() {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: java.util.LinkedList<Entry<K, V>>[] old = buckets;
                            std::list<Entry<K, V>*> *old = buckets;
                            currentPrime += 4;
                            int newCapacity = buckets->length * 2;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: java.util.LinkedList<Entry<K, V>>[] newTable = createEntryListArray(newCapacity);
                            std::list<Entry<K, V>*> *newTable = createEntryListArray(newCapacity);
                            buckets = newTable;
                            threshold = static_cast<int>(newCapacity * LOAD_FACTOR);
                    //		System.out.println("new size="+newCapacity+", thres="+threshold);
                            // rehash all existing entries
                            int oldSize = size();
                            for (auto bucket : old) {
                                if (bucket == nullptr) {
                                    continue;
                                }
                                for (auto e : bucket) {
                                    if (e == nullptr) {
                                        break;
                                    }
                                    put(e.key, e.value);
                                }
                            }
                            n = oldSize;
                        }

                    public:
                        virtual int size()  {
                            return n;
                        }

                        virtual bool isEmpty()  {
                            return n == 0;
                        }

                        virtual void clear()  {
                            buckets = createEntryListArray(INITAL_CAPACITY);
                            n = 0;
                        }

                        virtual std::wstring toString()  {
                            if (size() == 0) {
                                return L"{}";
                            }

                            StringBuilder *buf = new StringBuilder();
                            buf->append(L'{');
                            bool first = true;
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }
                                for (auto e : bucket) {
                                    if (e == nullptr) {
                                        break;
                                    }
                                    if (first) {
                                        first = false;
                                    } else {
                                        buf->append(L", ");
                                    }

                                    buf->append(e.toString());
                                }
                            }
                            buf->append(L'}');
                            return buf->toString();
                        }

                        virtual std::wstring toTableString() {
                            StringBuilder *buf = new StringBuilder();
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    buf->append(L"null\n");
                                    continue;
                                }
                                buf->append(L'[');
                                bool first = true;
                                for (auto e : bucket) {
                                    if (first) {
                                        first = false;
                                    } else {
                                        buf->append(L" ");
                                    }
                                    if (e == nullptr) {
                                        buf->append(L"_");
                                    } else {
                                        buf->append(e.toString());
                                    }
                                }
                                buf->append(L"]\n");
                            }
                            return buf->toString();
                        }

                        static void main(std::wstring args[]) {
                            FlexibleHashMap<std::wstring, int> *map = new FlexibleHashMap<std::wstring, int>();
                            map->put(L"hi", 1);
                            map->put(L"mom", 2);
                            map->put(L"foo", 3);
                            map->put(L"ach", 4);
                            map->put(L"cbba", 5);
                            map->put(L"d", 6);
                            map->put(L"edf", 7);
                            map->put(L"mom", 8);
                            map->put(L"hi", 9);
                            std::cout << map << std::endl;
                            //std::cout << map->toTableString();
                            std::cout << std::endl;
                        }

                    };

                }
            }
        }
    }
}

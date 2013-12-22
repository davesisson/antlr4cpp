#pragma once

#include "AbstractEqualityComparator.h"
#include "ObjectEqualityComparator.h"
#include "MurmurHash.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "vectorhelper.h"

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
                    class FlexibleHashMap : public Map<K, V> {
                    public:
                        template<typename K, typename V>
                        class Entry {
                        public:
                            const K key;
                            V value;

                            Entry(K key, V value) : key(key) {
                                this->value = value;
                            }

                            virtual std::wstring toString() override {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                return key->toString() + std::wstring(L":") + value->toString();
                            }
                        };

                    public:
                        static const int INITAL_CAPACITY = 16; // must be power of 2
                        static const int INITAL_BUCKET_CAPACITY = 8;
//JAVA TO C++ CONVERTER TODO TASK: Native C++ only allows initialization of static const integral fields in their declarations:
                        static const double LOAD_FACTOR = 0.75;

                    protected:
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: @NotNull protected final AbstractEqualityComparator<? super K> comparator;
                        AbstractEqualityComparator<? __super K> *const comparator;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected java.util.LinkedList<Entry<K, V>>[] buckets;
                        std::list<Entry<K, V>*> *buckets;

                        /// <summary>
                        /// How many elements in set </summary>
                        int n;

                        int threshold; // when to expand

                        int currentPrime; // jump by 4 primes each expand or whatever
                        int initialBucketCapacity;

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        FlexibleHashMap() { //this(nullptr, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                        }

                        template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public FlexibleHashMap(@Nullable AbstractEqualityComparator<? super K> comparator)
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        FlexibleHashMap(AbstractEqualityComparator<T1> *comparator) { //this(comparator, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                        }

                        template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public FlexibleHashMap(@Nullable AbstractEqualityComparator<? super K> comparator, int initialCapacity, int initialBucketCapacity)
                        FlexibleHashMap(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity) : comparator(ObjectEqualityComparator::INSTANCE) {
                            InitializeInstanceFields();
                            if (comparator == nullptr) {
                            }

                            this->comparator = comparator;
                            this->buckets = createEntryListArray(initialBucketCapacity);
                            this->initialBucketCapacity = initialBucketCapacity;
                        }

                    private:
                        template<typename K, typename V>
                        static std::list<Entry<K, V>*> *createEntryListArray(int length) {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: @SuppressWarnings("unchecked") java.util.LinkedList<Entry<K, V>>[] result = (java.util.LinkedList<Entry<K, V>>[])new java.util.LinkedList<?>[length];
                            std::list<Entry<K, V>*> *result = static_cast<std::list<Entry<K, V>*>[]>(new std::list<?>[length]);
                            return result;
                        }

                    protected:
                        virtual int getBucket(K key) {
                            int hash = comparator->hashCode(key);
                            int b = hash & (buckets->length - 1); // assumes len is power of 2
                            return b;
                        }

                    public:
                        virtual V get(void *key) override {
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

                        virtual V put(K key, V value) override {
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

                        virtual V remove(void *key) override {
                            throw UnsupportedOperationException();
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                        template<typename T1, typename T1> where T1 : K where T1 : V
                        virtual void putAll(Map<T1> *m) override {
                            throw UnsupportedOperationException();
                        }

                        virtual Set<K> *keySet() override {
                            throw UnsupportedOperationException();
                        }

                        virtual Collection<V> *values() override {
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

                        virtual Set<Map::Entry<K, V>*> *entrySet() override {
                            throw UnsupportedOperationException();
                        }

                        virtual bool containsKey(void *key) override {
                            return get(key) != nullptr;
                        }

                        virtual bool containsValue(void *value) override {
                            throw UnsupportedOperationException();
                        }

                        virtual int hashCode() override {
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

                        virtual bool equals(void *o) override {
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
                        virtual int size() override {
                            return n;
                        }

                        virtual bool isEmpty() override {
                            return n == 0;
                        }

                        virtual void clear() override {
                            buckets = createEntryListArray(INITAL_CAPACITY);
                            n = 0;
                        }

                        virtual std::wstring toString() override {
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
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    buf->append(e.toString());
                                }
                            }
                            buf->append(L'}');
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
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
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                        buf->append(e.toString());
                                    }
                                }
                                buf->append(L"]\n");
                            }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
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
                            std::cout << map->toTableString() << std::endl;
                        }

                    private:
                        void InitializeInstanceFields() {
                            n = 0;
                            threshold = static_cast<int>(INITAL_CAPACITY * LOAD_FACTOR);
                            currentPrime = 1;
                            initialBucketCapacity = INITAL_BUCKET_CAPACITY;
                        }
                    };

                }
            }
        }
    }
}

#pragma once

#include "AbstractEqualityComparator.h"
#include "ObjectEqualityComparator.h"
#include "MurmurHash.h"
#include <string>

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
                    /// <seealso cref="Set"/> implementation with closed hashing (open addressing). </summary>
                    template<typename T>
                    class Array2DHashSet : public Set<T> {
                    protected:
                        class SetIterator : public Iterator<T*> {
                                        private:
                                            Array2DHashSet<T*> *const outerInstance;

                        public:
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: final T[] data;
                            const T *data;
                            int nextIndex;
                            bool removed;

                            SetIterator(Array2DHashSet<T> *outerInstance, T data[]);

                            virtual bool hasNext() override;

                            virtual T *next() override;

                            virtual void remove() override;

                        private:
                            void InitializeInstanceFields();
                        };
                    public:
                        static const int INITAL_CAPACITY = 16; // must be power of 2
                        static const int INITAL_BUCKET_CAPACITY = 8;
//JAVA TO C++ CONVERTER TODO TASK: Native C++ only allows initialization of static const integral fields in their declarations:
                        static const double LOAD_FACTOR = 0.75;

                    protected:
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: @NotNull protected final AbstractEqualityComparator<? super T> comparator;
                        AbstractEqualityComparator<? __super T> *const comparator;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected T[][] buckets;
                        T **buckets;

                        /// <summary>
                        /// How many elements in set </summary>
                        int n;

                        int threshold; // when to expand

                        int currentPrime; // jump by 4 primes each expand or whatever
                        int initialBucketCapacity;

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        Array2DHashSet() { //this(nullptr, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                        }

                        template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public Array2DHashSet(@Nullable AbstractEqualityComparator<? super T> comparator)
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        Array2DHashSet(AbstractEqualityComparator<T1> *comparator) { //this(comparator, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                        }

                        template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public Array2DHashSet(@Nullable AbstractEqualityComparator<? super T> comparator, int initialCapacity, int initialBucketCapacity)
                        Array2DHashSet(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity) : comparator(ObjectEqualityComparator::INSTANCE) {
                            InitializeInstanceFields();
                            if (comparator == nullptr) {
                            }

                            this->comparator = comparator;
                            this->buckets = createBuckets(initialCapacity);
                            this->initialBucketCapacity = initialBucketCapacity;
                        }

                        /// <summary>
                        /// Add {@code o} to set if not there; return existing value if already
                        /// there. This method performs the same operation as <seealso cref="#add"/> aside from
                        /// the return value.
                        /// </summary>
                        T getOrAdd(T o) {
                            if (n > threshold) {
                                expand();
                            }
                            return getOrAddImpl(o);
                        }

                    protected:
                        virtual T getOrAddImpl(T o) {
                            int b = getBucket(o);
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[] bucket = buckets[b];
                            T *bucket = buckets[b];

                            // NEW BUCKET
                            if (bucket == nullptr) {
                                bucket = createBucket(initialBucketCapacity);
                                bucket[0] = o;
                                buckets[b] = bucket;
                                n++;
                                return o;
                            }

                            // LOOK FOR IT IN BUCKET
                            for (int i = 0; i < bucket->length; i++) {
                                T existing = bucket[i];
                                if (existing == nullptr) { // empty slot; not there, add.
                                    bucket[i] = o;
                                    n++;
                                    return o;
                                }
                                if (comparator->equals(existing, o)) { // found existing, quit
                                    return existing;
                                }
                            }

                            // FULL BUCKET, expand and add to end
                            int oldLength = bucket->length;
                            bucket = Arrays::copyOf(bucket, bucket->length * 2);
                            buckets[b] = bucket;
                            bucket[oldLength] = o; // add to end
                            n++;
                            return o;
                        }

                    public:
                        virtual T get(T o) {
                            if (o == nullptr) {
                                return o;
                            }
                            int b = getBucket(o);
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[] bucket = buckets[b];
                            T *bucket = buckets[b];
                            if (bucket == nullptr) { // no bucket
                                return nullptr;
                            }
                            for (auto e : bucket) {
                                if (e == nullptr) { // empty slot; not there
                                    return nullptr;
                                }
                                if (comparator->equals(e, o)) {
                                    return e;
                                }
                            }
                            return nullptr;
                        }

                    protected:
                        int getBucket(T o) {
                            int hash = comparator->hashCode(o);
                            int b = hash & (buckets->length - 1); // assumes len is power of 2
                            return b;
                        }

                    public:
                        virtual int hashCode() override {
                            int hash = MurmurHash::initialize();
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }
                                for (auto o : bucket) {
                                    if (o == nullptr) {
                                        break;
                                    }
                                    hash = MurmurHash::update(hash, comparator->hashCode(o));
                                }
                            }

                            hash = MurmurHash::finish(hash, size());
                            return hash;
                        }

                        virtual bool equals(void *o) override {
                            if (o == this) {
                                return true;
                            }
                            if (!(dynamic_cast<Array2DHashSet*>(o) != nullptr)) {
                                return false;
                            }
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: Array2DHashSet<?> other = (Array2DHashSet<?>)o;
                            Array2DHashSet<?> *other = static_cast<Array2DHashSet<?>*>(o);
                            if (other->size() != size()) {
                                return false;
                            }
                            bool same = this->containsAll(other);
                            return same;
                        }

                    protected:
                        virtual void expand() {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[][] old = buckets;
                            T **old = buckets;
                            currentPrime += 4;
                            int newCapacity = buckets->length * 2;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[][] newTable = createBuckets(newCapacity);
                            T **newTable = createBuckets(newCapacity);
                            int newBucketLengths[newTable->length];
                            buckets = newTable;
                            threshold = static_cast<int>(newCapacity * LOAD_FACTOR);
                    //		System.out.println("new size="+newCapacity+", thres="+threshold);
                            // rehash all existing entries
                            int oldSize = size();
                            for (auto bucket : old) {
                                if (bucket == nullptr) {
                                    continue;
                                }

                                for (auto o : bucket) {
                                    if (o == nullptr) {
                                        break;
                                    }

                                    int b = getBucket(o);
                                    int bucketLength = newBucketLengths[b];
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[] newBucket;
                                    T *newBucket;
                                    if (bucketLength == 0) {
                                        // new bucket
                                        newBucket = createBucket(initialBucketCapacity);
                                        newTable[b] = newBucket;
                                    } else {
                                        newBucket = newTable[b];
                                        if (bucketLength == newBucket->length) {
                                            // expand
                                            newBucket = Arrays::copyOf(newBucket, newBucket->length * 2);
                                            newTable[b] = newBucket;
                                        }
                                    }

                                    newBucket[bucketLength] = o;
                                    newBucketLengths[b]++;
                                }
                            }

                            assert(n == oldSize);
                        }

                    public:
                        bool add(T t) override {
                            T existing = getOrAdd(t);
                            return existing == t;
                        }

                        int size() override {
                            return n;
                        }

                        bool isEmpty() override {
                            return n == 0;
                        }

                        bool contains(void *o) override {
                            return containsFast(asElementType(o));
                        }

                        virtual bool containsFast(T obj) {
                            if (obj == nullptr) {
                                return false;
                            }

                            return get(obj) != nullptr;
                        }

                        virtual Iterator<T> *iterator() override {
                            return new SetIterator(this, toArray());
                        }

                        virtual T *toArray() override {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[] a = createBucket(size());
                            T *a = createBucket(size());
                            int i = 0;
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }

                                for (auto o : bucket) {
                                    if (o == nullptr) {
                                        break;
                                    }

                                    a[i++] = o;
                                }
                            }

                            return a;
                        }

                        template<typename U>
                        virtual U *toArray(U a[]) override {
                            if (sizeof(a) / sizeof(a[0]) < size()) {
                                a = Arrays::copyOf(a, size());
                            }

                            int i = 0;
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }

                                for (auto o : bucket) {
                                    if (o == nullptr) {
                                        break;
                                    }

                                    U targetElement = static_cast<U>(o); // array store will check this
                                    a[i++] = targetElement;
                                }
                            }
                            return a;
                        }

                        bool remove(void *o) override {
                            return removeFast(asElementType(o));
                        }

                        virtual bool removeFast(T obj) {
                            if (obj == nullptr) {
                                return false;
                            }

                            int b = getBucket(obj);
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: T[] bucket = buckets[b];
                            T *bucket = buckets[b];
                            if (bucket == nullptr) {
                                // no bucket
                                return false;
                            }

                            for (int i = 0; i < bucket->length; i++) {
                                T e = bucket[i];
                                if (e == nullptr) {
                                    // empty slot; not there
                                    return false;
                                }

                                if (comparator->equals(e, obj)) { // found it
                                    // shift all elements to the right down one
                                    System::arraycopy(bucket, i + 1, bucket, i, bucket->length - i - 1);
                                    bucket[bucket->length - 1] = nullptr;
                                    n--;
                                    return true;
                                }
                            }

                            return false;
                        }

                        template<typename T1>
                        virtual bool containsAll(Collection<T1> *collection) override {
                            if (dynamic_cast<Array2DHashSet*>(collection) != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: Array2DHashSet<?> s = (Array2DHashSet<?>)collection;
                                Array2DHashSet<?> *s = static_cast<Array2DHashSet<?>*>(collection);
                                for (auto bucket : s->buckets) {
                                    if (bucket == nullptr) {
                                        continue;
                                    }
                                    for (auto o : bucket) {
                                        if (o == nullptr) {
                                            break;
                                        }
                                        if (!this->containsFast(asElementType(o))) {
                                            return false;
                                        }
                                    }
                                }
                            } else {
                                for (auto o : collection) {
                                    if (!this->containsFast(asElementType(o))) {
                                        return false;
                                    }
                                }
                            }
                            return true;
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                        template<typename T1> where T1 : T
                        virtual bool addAll(Collection<T1> *c) override {
                            bool changed = false;
                            for (auto o : c) {
                                T existing = getOrAdd(o);
                                if (existing != o) {
                                    changed = true;
                                }
                            }
                            return changed;
                        }

                        template<typename T1>
                        virtual bool retainAll(Collection<T1> *c) override {
                            int newsize = 0;
                            for (auto bucket : buckets) {
                                if (bucket == nullptr) {
                                    continue;
                                }

                                int i;
                                int j;
                                for (i = 0, j = 0; i < sizeof(bucket) / sizeof(bucket[0]); i++) {
                                    if (bucket[i] == nullptr) {
                                        break;
                                    }

                                    if (!c->contains(bucket[i])) {
                                        // removed
                                        continue;
                                    }

                                    // keep
                                    if (i != j) {
                                        bucket[j] = bucket[i];
                                    }

                                    j++;
                                    newsize++;
                                }

                                newsize += j;

                                while (j < i) {
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                    delete bucket[j];
                                    j++;
                                }
                            }

                            bool changed = newsize != n;
                            n = newsize;
                            return changed;
                        }

                        template<typename T1>
                        virtual bool removeAll(Collection<T1> *c) override {
                            bool changed = false;
                            for (auto o : c) {
                                changed |= removeFast(asElementType(o));
                            }

                            return changed;
                        }

                        virtual void clear() override {
                            buckets = createBuckets(INITAL_CAPACITY);
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
                                for (auto o : bucket) {
                                    if (o == nullptr) {
                                        break;
                                    }
                                    if (first) {
                                        first = false;
                                    } else {
                                        buf->append(L", ");
                                    }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    buf->append(o->toString());
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
                                for (auto o : bucket) {
                                    if (first) {
                                        first = false;
                                    } else {
                                        buf->append(L" ");
                                    }
                                    if (o == nullptr) {
                                        buf->append(L"_");
                                    } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                        buf->append(o->toString());
                                    }
                                }
                                buf->append(L"]\n");
                            }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            return buf->toString();
                        }

                        /// <summary>
                        /// Return {@code o} as an instance of the element type {@code T}. If
                        /// {@code o} is non-null but known to not be an instance of {@code T}, this
                        /// method returns {@code null}. The base implementation does not perform any
                        /// type checks; override this method to provide strong type checks for the
                        /// <seealso cref="#contains"/> and <seealso cref="#remove"/> methods to ensure the arguments to
                        /// the <seealso cref="EqualityComparator"/> for the set always have the expected
                        /// types.
                        /// </summary>
                        /// <param name="o"> the object to try and cast to the element type of the set </param>
                        /// <returns> {@code o} if it could be an instance of {@code T}, otherwise
                        /// {@code null}. </returns>
                    protected:
                        virtual T asElementType(void *o) {
                            return static_cast<T>(o);
                        }

                        /// <summary>
                        /// Return an array of {@code T[]} with length {@code capacity}.
                        /// </summary>
                        /// <param name="capacity"> the length of the array to return </param>
                        /// <returns> the newly constructed array </returns>
                        virtual T **createBuckets(int capacity) {
                            return static_cast<T[][]>(new Object[capacity][]);
                        }

                        /// <summary>
                        /// Return an array of {@code T} with length {@code capacity}.
                        /// </summary>
                        /// <param name="capacity"> the length of the array to return </param>
                        /// <returns> the newly constructed array </returns>
                        virtual T *createBucket(int capacity) {
                            return static_cast<T[]>(new Object[capacity]);
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

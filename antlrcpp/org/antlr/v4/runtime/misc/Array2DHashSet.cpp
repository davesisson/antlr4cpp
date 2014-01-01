#include "Array2DHashSet.h"
#include "Exceptions.h"
#include "StringBuilder.h"
#include "Arrays.h"

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
                    template<typename T>
                    Array2DHashSet<T>::SetIterator::SetIterator(Array2DHashSet<T> *outerInstance, T data[]) : data(data), outerInstance(outerInstance) {

                        InitializeInstanceFields();
                    }
                    
                    template<typename T>
                    bool Array2DHashSet<T>::SetIterator::hasNext() {
                        return nextIndex < data->length;
                    }

                    template<typename T>
                    T *Array2DHashSet<T>::SetIterator::next() {
                        if (!hasNext()) {
                            throw new NoSuchElementException();
                        }

                        removed = false;
                        return data[nextIndex++];
                    }

                    template<typename T>
                    void Array2DHashSet<T>::SetIterator::remove() {
                        if (removed) {
                            throw new IllegalStateException();
                        }

                        outerInstance->remove(data[nextIndex - 1]);
                        removed = true;
                    }

                    template<typename T>
                    void Array2DHashSet<T>::SetIterator::InitializeInstanceFields() {
                        nextIndex = 0;
                        removed = true;
                    }
                    
                    template<typename T>
                    template<typename T1>
                    Array2DHashSet<T>::Array2DHashSet(AbstractEqualityComparator<T1> *comparator) { //this(comparator, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                    }
                    
                    template<typename T>
                    template<typename T1>
                    Array2DHashSet<T>::Array2DHashSet(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity) : comparator(ObjectEqualityComparator::INSTANCE) {
                        InitializeInstanceFields();
                        if (comparator == nullptr) {
                        }
                        
                        this->comparator = comparator;
                        this->buckets = createBuckets(initialCapacity);
                        this->initialBucketCapacity = initialBucketCapacity;
                    }
                    
                    template<typename T>
                    T Array2DHashSet<T>::getOrAdd(T o) {
                        if (n > threshold) {
                            expand();
                        }
                        return getOrAddImpl(o);
                    }
                    
                    template<typename T>
                    T Array2DHashSet<T>::getOrAddImpl(T o) {
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
                    
                    template<typename T>
                    T Array2DHashSet<T>::get(T o) {
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
                    
                    template<typename T>
                    int Array2DHashSet<T>::getBucket(T o) {
                        int hash = comparator->hashCode(o);
                        int b = hash & (buckets->length - 1); // assumes len is power of 2
                        return b;
                    }
                    
                    template<typename T>
                    int Array2DHashSet<T>::hashCode()  {
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
                    
                    template<typename T>
                    bool Array2DHashSet<T>::equals(void *o) {
                        if (o == this) {
                            return true;
                        }
                        if (!(dynamic_cast<Array2DHashSet*>(o) != nullptr)) {
                            return false;
                        }
                        //JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
                        //ORIGINAL LINE: Array2DHashSet<?> other = (Array2DHashSet<?>)o;
                        Array2DHashSet<void*> *other = static_cast<Array2DHashSet<void*>*>(o);
                        if (other->size() != size()) {
                            return false;
                        }
                        bool same = this->containsAll(other);
                        return same;
                    }
                    
                    template<typename T>
                    void Array2DHashSet<T>::expand() {
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
                    
                    template<typename T>
                    bool Array2DHashSet<T>::add(T t) {
                        T existing = getOrAdd(t);
                        return existing == t;
                    }
                    
                    template<typename T>
                    int Array2DHashSet<T>::size() {
                        return n;
                    }
                    
                    template<typename T>
                    bool Array2DHashSet<T>::isEmpty() {
                        return n == 0;
                    }
                    
                    template<typename T>
                    bool Array2DHashSet<T>::contains(void *o) {
                        return containsFast(asElementType(o));
                    }
                    
                    template<typename T>
                    bool Array2DHashSet<T>::containsFast(T obj) {
                        if (obj == nullptr) {
                            return false;
                        }
                        
                        return get(obj) != nullptr;
                    }
                    
                    template<typename T>
                    std::iterator<std::random_access_iterator_tag, T> *Array2DHashSet<T>::iterator() {
                        return new SetIterator(this, toArray());
                    }
                    
                    template<typename T>
                    T *Array2DHashSet<T>::toArray()  {
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
                    
                    template<typename T>
                    template<typename U>
                    U *Array2DHashSet<T>::toArray(U a[]) {
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
                    
                    template<typename T>
                    bool Array2DHashSet<T>::remove(void *o) {
                        return removeFast(asElementType(o));
                    }
                    
                    template<typename T>
                    bool Array2DHashSet<T>::removeFast(T obj) {
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
                                //System::arraycopy(bucket, i + 1, bucket, i, bucket->length - i - 1);
                                for (int j = i; j < bucket->length - i - 1; j++) {
                                    bucket[j] = bucket[j + 1];
                                }
                                bucket[bucket->length - 1] = nullptr;
                                n--;
                                return true;
                            }
                        }
                        
                        return false;
                    }
                    
                    template<typename T>
                    template<typename T1>
                    bool Array2DHashSet<T>::containsAll(std::set<T1> *collection) {
                        if (dynamic_cast<Array2DHashSet*>(collection) != nullptr) {
                            //JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
                            //ORIGINAL LINE: Array2DHashSet<?> s = (Array2DHashSet<?>)collection;
                            Array2DHashSet<T> *s = static_cast<Array2DHashSet<T>*>(collection);
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
                    template<typename T>
                    template<typename T1>
                    bool Array2DHashSet<T>::addAll(std::set<T1> *c) {
                        bool changed = false;
                        for (auto o : c) {
                            T existing = getOrAdd(o);
                            if (existing != o) {
                                changed = true;
                            }
                        }
                        return changed;
                    }
                    
                    template<typename T>
                    template<typename T1>
                    bool Array2DHashSet<T>::retainAll(std::set<T1> *c) {
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
                    
                    template<typename T>
                    template<typename T1>
                    bool Array2DHashSet<T>::removeAll(std::set<T1> *c) {
                        bool changed = false;
                        for (auto o : c) {
                            changed |= removeFast(asElementType(o));
                        }
                        
                        return changed;
                    }
                    
                    template<typename T>
                    std::wstring Array2DHashSet<T>::toString() {
                        if (size() == 0) {
                            return L"{}";
                        }
                        
                        StringBuilder *buf = new StringBuilder();
                        buf->append(L"{");
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
                        buf->append(L"}");
                        //JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }
                    
                    template<typename T>
                    std::wstring Array2DHashSet<T>::toTableString() {
                        StringBuilder *buf = new StringBuilder();
                        for (auto bucket : buckets) {
                            if (bucket == nullptr) {
                                buf->append(L"null\n");
                                continue;
                            }
                            buf->append(L"[");
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
                    /// Return an array of {@code T[]} with length {@code capacity}.
                    /// </summary>
                    /// <param name="capacity"> the length of the array to return </param>
                    /// <returns> the newly constructed array </returns>
                    template<typename T>
                    std::vector<std::vector<T>> *Array2DHashSet<T>::createBuckets(int capacity) {
                        return new std::vector<std::vector<T>>();//static_cast<T[][]>(new Object[capacity][]);
                    }
                    
                    /// <summary>
                    /// Return an array of {@code T} with length {@code capacity}.
                    /// </summary>
                    /// <param name="capacity"> the length of the array to return </param>
                    /// <returns> the newly constructed array </returns>
                    template<typename T>
                    std::vector<T> *Array2DHashSet<T>::createBucket(int capacity) {
                        return new std::vector<T>();//static_cast<T[]>(new Object[capacity]);
                    }
                    
                    template<typename T>
                    void Array2DHashSet<T>::clear() {
                        buckets = createBuckets(INITAL_CAPACITY);
                        n = 0;
                    }
                    
                    template<typename T>
                    void Array2DHashSet<T>::InitializeInstanceFields() {
                        n = 0;
                        threshold = static_cast<int>(INITAL_CAPACITY * LOAD_FACTOR);
                        currentPrime = 1;
                        initialBucketCapacity = INITAL_BUCKET_CAPACITY;
                    }
                    
                    
                }
            }
        }
    }
}

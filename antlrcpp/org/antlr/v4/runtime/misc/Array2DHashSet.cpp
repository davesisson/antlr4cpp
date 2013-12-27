#include "Array2DHashSet.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

                    Array2DHashSet<T>::SetIterator::SetIterator(Array2DHashSet<T> *outerInstance, T data[]) : data(data), outerInstance(outerInstance) {

                        InitializeInstanceFields();
                    }

                    bool Array2DHashSet<T>::SetIterator::hasNext() {
                        return nextIndex < data->length;
                    }

                    T *Array2DHashSet<T>::SetIterator::next() {
                        if (!hasNext()) {
                            throw NoSuchElementException();
                        }

                        removed = false;
                        return data[nextIndex++];
                    }

                    void Array2DHashSet<T>::SetIterator::remove() {
                        if (removed) {
                            throw IllegalStateException();
                        }

                        outerInstance->remove(data[nextIndex - 1]);
                        removed = true;
                    }

                    void Array2DHashSet<T>::SetIterator::InitializeInstanceFields() {
                        nextIndex = 0;
                        removed = true;
                    }
                    Array2DHashSet<T>::Array2DHashSet(AbstractEqualityComparator<T1> *comparator) { //this(comparator, INITAL_CAPACITY, INITAL_BUCKET_CAPACITY);
                    }
                    Array2DHashSet<T>::Array2DHashSet(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity) : comparator(ObjectEqualityComparator::INSTANCE) {
                        InitializeInstanceFields();
                        if (comparator == nullptr) {
                        }
                        
                        this->comparator = comparator;
                        this->buckets = createBuckets(initialCapacity);
                        this->initialBucketCapacity = initialBucketCapacity;
                    }
                    
                    T Array2DHashSet<T>::getOrAdd(T o) {
                        if (n > threshold) {
                            expand();
                        }
                        return getOrAddImpl(o);
                    }
                    virtual T Array2DHashSet<T>::getOrAddImpl(T o) {
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
                    virtual T Array2DHashSet<T>::get(T o) {
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
                    int Array2DHashSet<T>::getBucket(T o) {
                        int hash = comparator->hashCode(o);
                        int b = hash & (buckets->length - 1); // assumes len is power of 2
                        return b;
                    }
                    virtual int Array2DHashSet<T>::hashCode() override {
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
                    
                    virtual bool Array2DHashSet<T>::equals(void *o) override {
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
                    virtual void Array2DHashSet<T>::expand() {
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
                    bool Array2DHashSet<T>::add(T t) override {
                        T existing = getOrAdd(t);
                        return existing == t;
                    }
                    
                    int Array2DHashSet<T>::size() override {
                        return n;
                    }
                    
                    bool Array2DHashSet<T>::isEmpty() override {
                        return n == 0;
                    }
                    
                    bool Array2DHashSet<T>::contains(void *o) override {
                        return containsFast(asElementType(o));
                    }
                    
                    virtual bool Array2DHashSet<T>::containsFast(T obj) {
                        if (obj == nullptr) {
                            return false;
                        }
                        
                        return get(obj) != nullptr;
                    }
                    
                    virtual Iterator<T> *Array2DHashSet<T>::iterator() override {
                        return new SetIterator(this, toArray());
                    }
                    virtual T *Array2DHashSet<T>::toArray() override {
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
                    virtual U *Array2DHashSet<T>::toArray(U a[]) override {
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
                    
                    bool Array2DHashSet<T>::remove(void *o) override {
                        return removeFast(asElementType(o));
                    }
                    
                    virtual bool Array2DHashSet<T>::removeFast(T obj) {
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
                    bool Array2DHashSet<T>::containsAll(Collection<T1> *collection) {
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
                    template<typename T1>
                    bool Array2DHashSet<T>::addAll(Collection<T1> *c) {
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
                    bool Array2DHashSet<T>::retainAll(Collection<T1> *c) {
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
                    bool Array2DHashSet<T>::removeAll(Collection<T1> *c) {
                        bool changed = false;
                        for (auto o : c) {
                            changed |= removeFast(asElementType(o));
                        }
                        
                        return changed;
                    }
                    virtual std::wstring Array2DHashSet<T>::toString() override {
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
                    
                    virtual std::wstring Array2DHashSet<T>::toTableString() {
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
                    /// Return an array of {@code T[]} with length {@code capacity}.
                    /// </summary>
                    /// <param name="capacity"> the length of the array to return </param>
                    /// <returns> the newly constructed array </returns>
                    virtual T **Array2DHashSet<T>::createBuckets(int capacity) {
                        return static_cast<T[][]>(new Object[capacity][]);
                    }
                    
                    /// <summary>
                    /// Return an array of {@code T} with length {@code capacity}.
                    /// </summary>
                    /// <param name="capacity"> the length of the array to return </param>
                    /// <returns> the newly constructed array </returns>
                    virtual T *Array2DHashSet<T>::createBucket(int capacity) {
                        return static_cast<T[]>(new Object[capacity]);
                    }
                    virtual void Array2DHashSet<T>::clear() {
                        buckets = createBuckets(INITAL_CAPACITY);
                        n = 0;
                    }
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

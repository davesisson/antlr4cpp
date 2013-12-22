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
                }
            }
        }
    }
}

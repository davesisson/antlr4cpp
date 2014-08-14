#include "IntegerList.h"
#include "Declarations.h"
#include "Exceptions.h"

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

                    int *IntegerList::EMPTY_DATA = new int[0];

                    IntegerList::IntegerList() {
                        InitializeInstanceFields();
                        _data = EMPTY_DATA;
                    }

                    IntegerList::IntegerList(int capacity) {
                        InitializeInstanceFields();
                        if (capacity < 0) {
                            throw new IllegalArgumentException();
                        }

                        if (capacity == 0) {
                            _data = EMPTY_DATA;
                        } else {
                            _data = new int[capacity];
                        }
                    }

                    IntegerList::IntegerList(IntegerList *list) {
                        InitializeInstanceFields();
                        _data = list->_data->clone();
                        _size = list->_size;
                    }

                    IntegerList::IntegerList(Collection<int> *list) {
                        for (auto value : list) {
                            add(value);
                        }
                    }

                    void IntegerList::add(int value) {
                        if (_data->length == _size) {
                            ensureCapacity(_size + 1);
                        }

                        _data[_size] = value;
                        _size++;
                    }

                    void IntegerList::addAll(int array_Renamed[]) {
                        ensureCapacity(_size + sizeof(array_Renamed) / sizeof(array_Renamed[0]));
                        System::arraycopy(array_Renamed, 0, _data, _size, sizeof(array_Renamed) / sizeof(array_Renamed[0]));
                        _size += sizeof(array_Renamed) / sizeof(array_Renamed[0]);
                    }

                    void IntegerList::addAll(IntegerList *list) {
                        ensureCapacity(_size + list->_size);
                        System::arraycopy(list->_data, 0, _data, _size, list->_size);
                        _size += list->_size;
                    }

                    void IntegerList::addAll(Collection<int> *list) {
                        ensureCapacity(_size + list->size());
                        int current = 0;
                        for (auto x : list) {
                            _data[_size + current] = x;
                        }

                        _size += list->size();
                    }

                    int IntegerList::get(int index) {
                        if (index < 0 || index >= _size) {
                            throw IndexOutOfBoundsException();
                        }

                        return _data[index];
                    }

                    bool IntegerList::contains(int value) {
                        for (int i = 0; i < _size; i++) {
                            if (_data[i] == value) {
                                return true;
                            }
                        }

                        return false;
                    }

                    int IntegerList::set(int index, int value) {
                        if (index < 0 || index >= _size) {
                            throw IndexOutOfBoundsException();
                        }

                        int previous = _data[index];
                        _data[index] = value;
                        return previous;
                    }

                    int IntegerList::removeAt(int index) {
                        int value = get(index);
                        System::arraycopy(_data, index + 1, _data, index, _size - index - 1);
                        _data[_size - 1] = 0;
                        _size--;
                        return value;
                    }

                    void IntegerList::removeRange(int fromIndex, int toIndex) {
                        if (fromIndex < 0 || toIndex < 0 || fromIndex > _size || toIndex > _size) {
                            throw IndexOutOfBoundsException();
                        }
                        if (fromIndex > toIndex) {
                            throw IllegalArgumentException();
                        }

                        System::arraycopy(_data, toIndex, _data, fromIndex, _size - toIndex);
                        Arrays::fill(_data, _size - (toIndex - fromIndex), _size, 0);
                        _size -= (toIndex - fromIndex);
                    }

                    bool IntegerList::isEmpty() {
                        return _size == 0;
                    }

                    int IntegerList::size() {
                        return _size;
                    }

                    void IntegerList::trimToSize() {
                        if (_data->length == _size) {
                            return;
                        }

                        _data = Arrays::copyOf(_data, _size);
                    }

                    void IntegerList::clear() {
                        Arrays::fill(_data, 0, _size, 0);
                        _size = 0;
                    }

                    int *IntegerList::toArray() {
                        if (_size == 0) {
                            return EMPTY_DATA;
                        }

                        return Arrays::copyOf(_data, _size);
                    }

                    void IntegerList::sort() {
                        Arrays::sort(_data, 0, _size);
                    }

                    bool IntegerList::equals(void *o) {
                        if (o == this) {
                            return true;
                        }

                        if (!(dynamic_cast<IntegerList*>(o) != nullptr)) {
                            return false;
                        }

                        IntegerList *other = static_cast<IntegerList*>(o);
                        if (_size != other->_size) {
                            return false;
                        }

                        for (int i = 0; i < _size; i++) {
                            if (_data[i] != other->_data[i]) {
                                return false;
                            }
                        }

                        return true;
                    }

                    int IntegerList::hashCode() {
                        int hashCode = 1;
                        for (int i = 0; i < _size; i++) {
                            hashCode = 31*hashCode + _data[i];
                        }

                        return hashCode;
                    }

                    std::wstring IntegerList::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return Arrays->toString(toArray());
                    }

                    int IntegerList::binarySearch(int key) {
                        return Arrays::binarySearch(_data, 0, _size, key);
                    }

                    int IntegerList::binarySearch(int fromIndex, int toIndex, int key) {
                        if (fromIndex < 0 || toIndex < 0 || fromIndex > _size || toIndex > _size) {
                            throw IndexOutOfBoundsException();
                        }

                        return Arrays::binarySearch(_data, fromIndex, toIndex, key);
                    }

                    void IntegerList::ensureCapacity(int capacity) {
                        if (capacity < 0 || capacity > MAX_ARRAY_SIZE) {
                            throw OutOfMemoryError();
                        }

                        int newLength;
                        if (_data->length == 0) {
                            newLength = INITIAL_SIZE;
                        } else {
                            newLength = _data->length;
                        }

                        while (newLength < capacity) {
                            newLength = newLength * 2;
                            if (newLength < 0 || newLength > MAX_ARRAY_SIZE) {
                                newLength = MAX_ARRAY_SIZE;
                            }
                        }

                        _data = Arrays::copyOf(_data, newLength);
                    }

                    void IntegerList::InitializeInstanceFields() {
                        _size = 0;
                    }
                }
            }
        }
    }
}

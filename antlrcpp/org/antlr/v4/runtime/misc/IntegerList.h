#pragma once

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


                    /// 
                    /// <summary>
                    /// @author Sam Harwell
                    /// </summary>
                    class IntegerList {

                    private:
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: private static int[] EMPTY_DATA = new int[0];
                        static int *EMPTY_DATA;

                        static const int INITIAL_SIZE = 4;
                        static const int MAX_ARRAY_SIZE = int::MAX_VALUE - 8;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: @NotNull private int[] _data;
                        int *_data;

                        int _size;

                    public:
                        IntegerList();

                        IntegerList(int capacity);

                        IntegerList(IntegerList *list);

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        IntegerList(Collection<int> *list); //this(list.size());

                        void add(int value);

                        void addAll(int array_Renamed[]);

                        void addAll(IntegerList *list);

                        void addAll(Collection<int> *list);

                        int get(int index);

                        bool contains(int value);

                        int set(int index, int value);

                        int removeAt(int index);

                        void removeRange(int fromIndex, int toIndex);

                        bool isEmpty();

                        int size();

                        void trimToSize();

                        void clear();

                        int *toArray();

                        void sort();

                        /// <summary>
                        /// Compares the specified object with this list for equality.  Returns
                        /// {@code true} if and only if the specified object is also an <seealso cref="IntegerList"/>,
                        /// both lists have the same size, and all corresponding pairs of elements in
                        /// the two lists are equal.  In other words, two lists are defined to be
                        /// equal if they contain the same elements in the same order.
                        /// <para>
                        /// This implementation first checks if the specified object is this
                        /// list. If so, it returns {@code true}; if not, it checks if the
                        /// specified object is an <seealso cref="IntegerList"/>. If not, it returns {@code false};
                        /// if so, it checks the size of both lists. If the lists are not the same size,
                        /// it returns {@code false}; otherwise it iterates over both lists, comparing
                        /// corresponding pairs of elements.  If any comparison returns {@code false},
                        /// this method returns {@code false}.
                        /// 
                        /// </para>
                        /// </summary>
                        /// <param name="o"> the object to be compared for equality with this list </param>
                        /// <returns> {@code true} if the specified object is equal to this list </returns>
                        virtual bool equals(void *o) override;

                        /// <summary>
                        /// Returns the hash code value for this list.
                        /// <p/>
                        /// This implementation uses exactly the code that is used to define the
                        /// list hash function in the documentation for the <seealso cref="List#hashCode"/>
                        /// method.
                        /// </summary>
                        /// <returns> the hash code value for this list </returns>
                        virtual int hashCode() override;

                        /// <summary>
                        /// Returns a string representation of this list.
                        /// </summary>
                        virtual std::wstring toString() override;

                        int binarySearch(int key);

                        int binarySearch(int fromIndex, int toIndex, int key);

                    private:
                        void ensureCapacity(int capacity);


                    private:
                        void InitializeInstanceFields();
                    };

                }
            }
        }
    }
}

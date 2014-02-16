#include "UnbufferedCharStream.h"
#include "IntStream.h"
#include "ANTLRInputStream.h"
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
                using org::antlr::v4::runtime::misc::Interval;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                UnbufferedCharStream::UnbufferedCharStream() {
                }

                UnbufferedCharStream::UnbufferedCharStream(int bufferSize) {
                    InitializeInstanceFields();
                    n = 0;
                    data = new wchar_t[bufferSize];
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                UnbufferedCharStream::UnbufferedCharStream(InputStream *input) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                UnbufferedCharStream::UnbufferedCharStream(Reader *input) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                UnbufferedCharStream::UnbufferedCharStream(InputStream *input, int bufferSize) {
                    this->input = new InputStreamReader(input);
                    fill(1); // prime
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                UnbufferedCharStream::UnbufferedCharStream(Reader *input, int bufferSize) {
                    this->input = input;
                    fill(1); // prime
                }

                void UnbufferedCharStream::consume() {
                    if (LA(1) == IntStream::EOF) {
                        throw IllegalStateException(L"cannot consume EOF");
                    }

                    // buf always has at least data[p==0] in this method due to ctor
                    lastChar = data[p]; // track last char for LA(-1)

                    if (p == n - 1 && numMarkers == 0) {
                        n = 0;
                        p = -1; // p++ will leave this at 0
                        lastCharBufferStart = lastChar;
                    }

                    p++;
                    currentCharIndex++;
                    sync(1);
                }

                void UnbufferedCharStream::sync(int want) {
                    int need = (p + want - 1) - n + 1; // how many more elements we need?
                    if (need > 0) {
                        fill(need);
                    }
                }

                int UnbufferedCharStream::fill(int n) {
                    for (int i = 0; i < n; i++) {
                        if (this->n > 0 && data[this->n - 1] == static_cast<wchar_t>(IntStream::EOF)) {
                            return i;
                        }

                        try {
                            int c = nextChar();
                            add(c);
                        } catch (IOException ioe) {
                            throw std::exception(ioe);
                        }
                    }

                    return n;
                }

                int UnbufferedCharStream::nextChar() throw(IOException) {
                    return input->read();
                }

                void UnbufferedCharStream::add(int c) {
                    if (n >= data->length) {
                        data = Arrays::copyOf(data, data->length * 2);
                    }
                    data[n++] = static_cast<wchar_t>(c);
                }

                int UnbufferedCharStream::LA(int i) {
                    if (i == -1) { // special case
                        return lastChar;
                    }
                    sync(i);
                    int index = p + i - 1;
                    if (index < 0) {
                        throw IndexOutOfBoundsException();
                    }
                    if (index >= n) {
                        return IntStream::EOF;
                    }
                    wchar_t c = data[index];
                    if (c == static_cast<wchar_t>(IntStream::EOF)) {
                        return IntStream::EOF;
                    }
                    return c;
                }

                int UnbufferedCharStream::mark() {
                    if (numMarkers == 0) {
                        lastCharBufferStart = lastChar;
                    }

                    int mark = -numMarkers - 1;
                    numMarkers++;
                    return mark;
                }

                void UnbufferedCharStream::release(int marker) {
                    int expectedMark = -numMarkers;
                    if (marker != expectedMark) {
                        throw IllegalStateException(L"release() called with an invalid marker.");
                    }

                    numMarkers--;
                    if (numMarkers == 0 && p > 0) { // release buffer when we can, but don't do unnecessary work
                        // Copy data[p]..data[n-1] to data[0]..data[(n-1)-p], reset ptrs
                        // p is last valid char; move nothing if p==n as we have no valid char
                        System::arraycopy(data, p, data, 0, n - p); // shift n-p char from p to 0
                        n = n - p;
                        p = 0;
                        lastCharBufferStart = lastChar;
                    }
                }

                int UnbufferedCharStream::index() {
                    return currentCharIndex;
                }

                void UnbufferedCharStream::seek(int index) {
                    if (index == currentCharIndex) {
                        return;
                    }

                    if (index > currentCharIndex) {
                        sync(index - currentCharIndex);
                        index = std::min(index, getBufferStartIndex() + n - 1);
                    }

                    // index == to bufferStartIndex should set p to 0
                    int i = index - getBufferStartIndex();
                    if (i < 0) {
                        throw IllegalArgumentException(std::wstring(L"cannot seek to negative index ") + index);
                    } else if (i >= n) {
                        throw UnsupportedOperationException(std::wstring(L"seek to index outside buffer: ") + index + std::wstring(L" not in ") + getBufferStartIndex() + std::wstring(L"..") + (getBufferStartIndex() + n));
                    }

                    p = i;
                    currentCharIndex = index;
                    if (p == 0) {
                        lastChar = lastCharBufferStart;
                    } else {
                        lastChar = data[p - 1];
                    }
                }

                int UnbufferedCharStream::size() {
                    throw UnsupportedOperationException(L"Unbuffered stream cannot know its size");
                }

                std::wstring UnbufferedCharStream::getSourceName() {
                    return name;
                }

                std::wstring UnbufferedCharStream::getText(Interval *interval) {
                    if (interval->a < 0 || interval->b < interval->a - 1) {
                        throw IllegalArgumentException(L"invalid interval");
                    }

                    int bufferStartIndex = getBufferStartIndex();
                    if (n > 0 && data[n - 1] == wchar_t::MAX_VALUE) {
                        if (interval->a + interval->length() > bufferStartIndex + n) {
                            throw IllegalArgumentException(L"the interval extends past the end of the stream");
                        }
                    }

                    if (interval->a < bufferStartIndex || interval->b >= bufferStartIndex + n) {
                        throw UnsupportedOperationException(std::wstring(L"interval ") + interval + std::wstring(L" outside buffer: ") + bufferStartIndex + std::wstring(L"..") + (bufferStartIndex + n - 1));
                    }
                    // convert from absolute to local index
                    int i = interval->a - bufferStartIndex;
                    return std::wstring(data, i, interval->length());
                }

                int UnbufferedCharStream::getBufferStartIndex() {
                    return currentCharIndex - p;
                }

                void UnbufferedCharStream::InitializeInstanceFields() {
                    n = 0;
                    p = 0;
                    numMarkers = 0;
                    lastChar = -1;
                    lastCharBufferStart = 0;
                    currentCharIndex = 0;
                }
            }
        }
    }
}

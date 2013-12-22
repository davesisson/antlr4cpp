#include "ANTLRInputStream.h"
#include "Java/src/org/antlr/v4/runtime/IntStream.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Interval;

                ANTLRInputStream::ANTLRInputStream() {
                    InitializeInstanceFields();
                }

                ANTLRInputStream::ANTLRInputStream(const std::wstring &input) {
                    InitializeInstanceFields();
                    this->data = input.toCharArray();
                    this->n = input.length();
                }

                ANTLRInputStream::ANTLRInputStream(wchar_t data[], int numberOfActualCharsInArray) {
                    InitializeInstanceFields();
                    this->data = data;
                    this->n = numberOfActualCharsInArray;
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ANTLRInputStream::ANTLRInputStream(Reader *r) throw(IOException) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ANTLRInputStream::ANTLRInputStream(Reader *r, int initialSize) throw(IOException) {
                }

                ANTLRInputStream::ANTLRInputStream(Reader *r, int initialSize, int readChunkSize) throw(IOException) {
                    InitializeInstanceFields();
                    load(r, initialSize, readChunkSize);
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ANTLRInputStream::ANTLRInputStream(InputStream *input) throw(IOException) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ANTLRInputStream::ANTLRInputStream(InputStream *input, int initialSize) throw(IOException) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ANTLRInputStream::ANTLRInputStream(InputStream *input, int initialSize, int readChunkSize) throw(IOException) {
                }

                void ANTLRInputStream::load(Reader *r, int size, int readChunkSize) throw(IOException) {
                    if (r == nullptr) {
                        return;
                    }
                    if (size <= 0) {
                        size = INITIAL_BUFFER_SIZE;
                    }
                    if (readChunkSize <= 0) {
                        readChunkSize = READ_BUFFER_SIZE;
                    }
                       // System.out.println("load "+size+" in chunks of "+readChunkSize);
                       try {
                           // alloc initial buffer size.
                           data = new wchar_t[size];
                           // read all the data in chunks of readChunkSize
                           int numRead = 0;
                           int p = 0;
                           do {
                               if (p + readChunkSize > data->length) { // overflow?
                                   // System.out.println("### overflow p="+p+", data.length="+data.length);
                                   data = Arrays::copyOf(data, data->length * 2);
                               }
                               numRead = r->read(data, p, readChunkSize);
                               // System.out.println("read "+numRead+" chars; p was "+p+" is now "+(p+numRead));
                               p += numRead;
                           } while (numRead != -1); // while not EOF
                           // set the actual size of the data available;
                           // EOF subtracted one above in p+=numRead; add one back
                           n = p + 1;
                           //System.out.println("n="+n);
                       } finally {
                           r->close();
                       }
                }

                void ANTLRInputStream::reset() {
                    p = 0;
                }

                void ANTLRInputStream::consume() {
                    if (p >= n) {
                        assert(LA(1) == IntStream::EOF);
                        throw IllegalStateException(L"cannot consume EOF");
                    }

                    //System.out.println("prev p="+p+", c="+(char)data[p]);
                    if (p < n) {
                        p++;
                        //System.out.println("p moves to "+p+" (c='"+(char)data[p]+"')");
                    }
                }

                int ANTLRInputStream::LA(int i) {
                    if (i == 0) {
                        return 0; // undefined
                    }
                    if (i < 0) {
                        i++; // e.g., translate LA(-1) to use offset i=0; then data[p+0-1]
                        if ((p + i - 1) < 0) {
                            return IntStream::EOF; // invalid; no char before first char
                        }
                    }

                    if ((p + i - 1) >= n) {
                        //System.out.println("char LA("+i+")=EOF; p="+p);
                        return IntStream::EOF;
                    }
                    //System.out.println("char LA("+i+")="+(char)data[p+i-1]+"; p="+p);
                    //System.out.println("LA("+i+"); p="+p+" n="+n+" data.length="+data.length);
                    return data[p + i - 1];
                }

                int ANTLRInputStream::LT(int i) {
                    return LA(i);
                }

                int ANTLRInputStream::index() {
                    return p;
                }

                int ANTLRInputStream::size() {
                    return n;
                }

                int ANTLRInputStream::mark() {
                    return -1;
                }

                void ANTLRInputStream::release(int marker) {
                }

                void ANTLRInputStream::seek(int index) {
                    if (index <= p) {
                        p = index; // just jump; don't update stream state (line, ...)
                        return;
                    }
                    // seek forward, consume until p hits index
                    while (p < index && index < n) {
                        consume();
                    }
                }

                std::wstring ANTLRInputStream::getText(Interval *interval) {
                    int start = interval->a;
                    int stop = interval->b;
                    if (stop >= n) {
                        stop = n - 1;
                    }
                    int count = stop - start + 1;
                    if (start >= n) {
                        return L"";
                    }
                                //		System.err.println("data: "+Arrays.toString(data)+", n="+n+
                                //						   ", start="+start+
                                //						   ", stop="+stop);
                    return std::wstring(data, start, count);
                }

                std::wstring ANTLRInputStream::getSourceName() {
                    return name;
                }

                std::wstring ANTLRInputStream::toString() {
                    return std::wstring(data);
                }

                void ANTLRInputStream::InitializeInstanceFields() {
                    n = 0;
                    p = 0;
                }
            }
        }
    }
}

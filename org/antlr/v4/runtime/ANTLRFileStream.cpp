#include "ANTLRFileStream.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ANTLRFileStream::ANTLRFileStream(const std::wstring &fileName) throw(IOException) {
                }

                ANTLRFileStream::ANTLRFileStream(const std::wstring &fileName, const std::wstring &encoding) throw(IOException) {
                    this->fileName = fileName;
                    load(fileName, encoding);
                }

                void ANTLRFileStream::load(const std::wstring &fileName, const std::wstring &encoding) throw(IOException) {
                    if (fileName == L"") {
                        return;
                    }
                    File *f = new File(fileName);
                    int size = static_cast<int>(f->length());
                    InputStreamReader *isr;
                    FileInputStream *fis = new FileInputStream(fileName);
                    if (encoding != L"") {
                        isr = new InputStreamReader(fis, encoding);
                    } else {
                        isr = new InputStreamReader(fis);
                    }
                    try {
                        data = new wchar_t[size];
                        n = isr->read(data);
                        if (n < data->length) {
                            data = Arrays::copyOf(data, n);
                        }
                    } finally {
                        isr->close();
                    }
                }

                std::wstring ANTLRFileStream::getSourceName() {
                    return fileName;
                }
            }
        }
    }
}

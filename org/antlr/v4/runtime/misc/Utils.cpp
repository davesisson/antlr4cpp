#include "Utils.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

template<typename T>
                    std::wstring Utils::join(Iterator<T> *iter, const std::wstring &separator) {
                        StringBuilder *buf = new StringBuilder();
                        while (iter->hasNext()) {
                            buf->append(iter->next());
                            if (iter->hasNext()) {
                                buf->append(separator);
                            }
                            iter++;
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

template<typename T>
                    std::wstring Utils::join(T array_Renamed[], const std::wstring &separator) {
                        StringBuilder *builder = new StringBuilder();
                        for (int i = 0; i < sizeof(array_Renamed) / sizeof(array_Renamed[0]); i++) {
                            builder->append(array_Renamed[i]);
                            if (i < sizeof(array_Renamed) / sizeof(array_Renamed[0]) - 1) {
                                builder->append(separator);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return builder->toString();
                    }

                    std::wstring Utils::escapeWhitespace(const std::wstring &s, bool escapeSpaces) {
                        StringBuilder *buf = new StringBuilder();
                        for (auto c : s.toCharArray()) {
                            if (c == L' ' && escapeSpaces) {
                                buf->append(L'\u00B7');
                            } else if (c == L'\t') {
                                buf->append(L"\\t");
                            } else if (c == L'\n') {
                                buf->append(L"\\n");
                            } else if (c == L'\r') {
                                buf->append(L"\\r");
                            } else {
                                buf->append(c);
                            }
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    void Utils::writeFile(const std::wstring &fileName, const std::wstring &content) throw(IOException) {
                        FileWriter *fw = new FileWriter(fileName);
                        Writer *w = new BufferedWriter(fw);
                        try {
                            w->write(content);
                        } finally {
                            w->close();
                        }
                    }

                    void Utils::waitForClose(Window *const window) throw(InterruptedException) {
                        const auto lock = new Object();

                        Thread *t = new ThreadAnonymousInnerClassHelper(window, lock);

                        t->start();

                        window->addWindowListener(new WindowAdapterAnonymousInnerClassHelper(window, lock));

                        t->join();
                    }

                    Utils::ThreadAnonymousInnerClassHelper::ThreadAnonymousInnerClassHelper(Window *window, auto lock) {
                        this->window = window;
                        this->lock = lock;
                    }

                    void Utils::ThreadAnonymousInnerClassHelper::run() {
//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(lock) {
                            while (window->isVisible()) {
                                try {
                                    lock->wait(500);
                                } catch (InterruptedException e) {
                                }
                            }
                        }
                    }

                    Utils::WindowAdapterAnonymousInnerClassHelper::WindowAdapterAnonymousInnerClassHelper(Window *window, auto lock) {
                        this->window = window;
                        this->lock = lock;
                    }

                    void Utils::WindowAdapterAnonymousInnerClassHelper::windowClosing(WindowEvent *arg0) {
//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(lock) {
                            window->setVisible(false);
                            lock->notify();
                        }
                    }

                    Map<std::wstring, int> *Utils::toMap(std::wstring keys[]) {
                        Map<std::wstring, int> *m = std::unordered_map<std::wstring, int>();
                        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
                            m->put(keys[i], i);
                        }
                        return m;
                    }

                    wchar_t *Utils::toCharArray(const vector<int> *data) {
                        if (data == nullptr) {
                            return nullptr;
                        }
                        wchar_t cdata[data->size()];
                        for (int i = 0; i < data->size(); i++) {
                            cdata[i] = static_cast<wchar_t>(data->get(i));
                        }
                        return cdata;
                    }
                }
            }
        }
    }
}

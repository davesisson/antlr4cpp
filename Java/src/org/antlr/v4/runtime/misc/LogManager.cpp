#include "LogManager.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

                    LogManager::Record::Record() {
                        InitializeInstanceFields();
                        timestamp = System::currentTimeMillis();
                        location = (std::exception())->getStackTrace()[0];
                    }

                    std::wstring LogManager::Record::toString() {
                        StringBuilder *buf = new StringBuilder();
                        buf->append((new SimpleDateFormat(L"yyyy-MM-dd HH:mm:ss:SSS"))->format(Date(timestamp)));
                        buf->append(L" ");
                        buf->append(component);
                        buf->append(L" ");
                        buf->append(location->getFileName());
                        buf->append(L":");
                        buf->append(location->getLineNumber());
                        buf->append(L" ");
                        buf->append(msg);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    void LogManager::Record::InitializeInstanceFields() {
                        timestamp = 0;
                    }

                    void LogManager::log(const std::wstring &component, const std::wstring &msg) {
                        Record *r = new Record();
                        r->component = component;
                        r->msg = msg;
                        if (records.empty()) {
                            records = std::vector<Record*>();
                        }
                        records.push_back(r);
                    }

                    void LogManager::log(const std::wstring &msg) {
                        log(L"",);
                    }

                    void LogManager::save(const std::wstring &filename) throw(IOException) {
                        FileWriter *fw = new FileWriter(filename);
                        BufferedWriter *bw = new BufferedWriter(fw);
                        try {
                            bw->write(toString());
                        } finally {
                            bw->close();
                        }
                    }

                    std::wstring LogManager::save() throw(IOException) {
                        //String dir = System.getProperty("java.io.tmpdir");
                        std::wstring dir = L".";
                        std::wstring defaultFilename = dir + std::wstring(L"/antlr-") + (new SimpleDateFormat(L"yyyy-MM-dd-HH.mm.ss"))->format(Date()) + std::wstring(L".log");
                        save(defaultFilename);
                        return defaultFilename;
                    }

                    std::wstring LogManager::toString() {
                        if (records.empty()) {
                            return L"";
                        }
                        std::wstring nl = System::getProperty(L"line.separator");
                        StringBuilder *buf = new StringBuilder();
                        for (auto r : records) {
                            buf->append(r);
                            buf->append(nl);
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    void LogManager::main(std::wstring args[]) throw(IOException) {
                        LogManager *mgr = new LogManager();
                        mgr->log(L"atn", L"test msg");
                        mgr->log(L"dfa", L"test msg 2");
                        std::cout << mgr << std::endl;
                        mgr->save();
                    }
                }
            }
        }
    }
}

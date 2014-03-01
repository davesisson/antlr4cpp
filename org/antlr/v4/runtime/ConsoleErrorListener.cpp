#include "ConsoleErrorListener.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {

ConsoleErrorListener *const ConsoleErrorListener::INSTANCE = new ConsoleErrorListener();

template<typename T1, typename T1>
                void ConsoleErrorListener::syntaxError(Recognizer<T1> *recognizer, void *offendingSymbol, int line, int charPositionInLine, const std::wstring &msg, RecognitionException *e) {
                    System::err::println(std::wstring(L"line ") + line + std::wstring(L":") + charPositionInLine + std::wstring(L" ") + msg);
                }
            }
        }
    }
}

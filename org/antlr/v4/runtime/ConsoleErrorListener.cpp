#include "ConsoleErrorListener.h"

namespace org {
namespace antlr {
namespace v4 {
namespace runtime {

ConsoleErrorListener *const ConsoleErrorListener::INSTANCE =
    new ConsoleErrorListener();

template <typename T1, typename T2>
void ConsoleErrorListener::syntaxError(Recognizer<T1, T2> *recognizer,
                                       void *offendingSymbol, int line,
                                       int charPositionInLine,
                                       const std::wstring &msg,
                                       RecognitionException *e) {
  cerr << "line " << line << ":" << charPositionInLine << " " << msg);
}

}  // namespace runtime
}  // namespace v4
}  // namespace antlr
}  // namespace org

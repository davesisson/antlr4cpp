#include "ProxyErrorListener.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::ATNConfigSet;
                using org::antlr::v4::runtime::dfa::DFA;

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T1> where T1 : ANTLRErrorListener
                ProxyErrorListener::ProxyErrorListener(Collection<T1> *delegates) : delegates(delegates) {
                    if (delegates == nullptr) {
                        throw NullPointerException(L"delegates");
                    }

                }

template<typename T1, typename T1>
                void ProxyErrorListener::syntaxError(Recognizer<T1> *recognizer, void *offendingSymbol, int line, int charPositionInLine, const std::wstring &msg, RecognitionException *e) {
                    for (auto listener : delegates) {
                        listener->syntaxError(recognizer, offendingSymbol, line, charPositionInLine, msg, e);
                    }
                }

                void ProxyErrorListener::reportAmbiguity(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, bool exact, BitSet *ambigAlts, ATNConfigSet *configs) {
                    for (auto listener : delegates) {
                        listener->reportAmbiguity(recognizer, dfa, startIndex, stopIndex, exact, ambigAlts, configs);
                    }
                }

                void ProxyErrorListener::reportAttemptingFullContext(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, BitSet *conflictingAlts, ATNConfigSet *configs) {
                    for (auto listener : delegates) {
                        listener->reportAttemptingFullContext(recognizer, dfa, startIndex, stopIndex, conflictingAlts, configs);
                    }
                }

                void ProxyErrorListener::reportContextSensitivity(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, int prediction, ATNConfigSet *configs) {
                    for (auto listener : delegates) {
                        listener->reportContextSensitivity(recognizer, dfa, startIndex, stopIndex, prediction, configs);
                    }
                }
            }
        }
    }
}

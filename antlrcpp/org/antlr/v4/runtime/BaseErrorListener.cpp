#include "BaseErrorListener.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::ATNConfigSet;
                using org::antlr::v4::runtime::dfa::DFA;

template<typename T1, typename T1>
                void BaseErrorListener::syntaxError(Recognizer<T1> *recognizer, void *offendingSymbol, int line, int charPositionInLine, const std::wstring &msg, RecognitionException *e) {
                }

                void BaseErrorListener::reportAmbiguity(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, bool exact, BitSet *ambigAlts, ATNConfigSet *configs) {
                }

                void BaseErrorListener::reportAttemptingFullContext(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, BitSet *conflictingAlts, ATNConfigSet *configs) {
                }

                void BaseErrorListener::reportContextSensitivity(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, int prediction, ATNConfigSet *configs) {
                }
            }
        }
    }
}

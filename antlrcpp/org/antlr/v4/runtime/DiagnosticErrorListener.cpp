#include "DiagnosticErrorListener.h"


namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {


//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                DiagnosticErrorListener::DiagnosticErrorListener() {
                }

                DiagnosticErrorListener::DiagnosticErrorListener(bool exactOnly) : exactOnly(exactOnly) {
                }

                void DiagnosticErrorListener::reportAmbiguity(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, bool exact, BitSet *ambigAlts, ATNConfigSet *configs) {
                    if (exactOnly && !exact) {
                        return;
                    }

                    std::wstring format = L"reportAmbiguity d=%s: ambigAlts=%s, input='%s'";
                    std::wstring decision = getDecisionDescription(recognizer, dfa);
                    BitSet *conflictingAlts = getConflictingAlts(ambigAlts, configs);
                    std::wstring text = recognizer->getTokenStream()->getText(Interval::of(startIndex, stopIndex));
                    std::wstring message = std::wstring::format(format, decision, conflictingAlts, text);
                    recognizer->notifyErrorListeners(message);
                }

                void DiagnosticErrorListener::reportAttemptingFullContext(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, BitSet *conflictingAlts, ATNConfigSet *configs) {
                    std::wstring format = L"reportAttemptingFullContext d=%s, input='%s'";
                    std::wstring decision = getDecisionDescription(recognizer, dfa);
                    std::wstring text = recognizer->getTokenStream()->getText(Interval::of(startIndex, stopIndex));
                    std::wstring message = std::wstring::format(format, decision, text);
                    recognizer->notifyErrorListeners(message);
                }

                void DiagnosticErrorListener::reportContextSensitivity(Parser *recognizer, DFA *dfa, int startIndex, int stopIndex, int prediction, ATNConfigSet *configs) {
                    std::wstring format = L"reportContextSensitivity d=%s, input='%s'";
                    std::wstring decision = getDecisionDescription(recognizer, dfa);
                    std::wstring text = recognizer->getTokenStream()->getText(Interval::of(startIndex, stopIndex));
                    std::wstring message = std::wstring::format(format, decision, text);
                    recognizer->notifyErrorListeners(message);
                }

                std::wstring DiagnosticErrorListener::getDecisionDescription(Parser *recognizer, DFA *dfa) {
                    int decision = dfa->decision;
                    int ruleIndex = dfa->atnStartState->ruleIndex;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: String[] ruleNames = recognizer.getRuleNames();
                    std::wstring *ruleNames = recognizer->getRuleNames();
                    if (ruleIndex < 0 || ruleIndex >= ruleNames->length) {
                        return StringConverterHelper::toString(decision);
                    }

                    std::wstring ruleName = ruleNames[ruleIndex];
                    if (ruleName == L"" || ruleName.isEmpty()) {
                        return StringConverterHelper::toString(decision);
                    }

                    return std::wstring::format(L"%d (%s)", decision, ruleName);
                }

                BitSet *DiagnosticErrorListener::getConflictingAlts(BitSet *reportedAlts, ATNConfigSet *configs) {
                    if (reportedAlts != nullptr) {
                        return reportedAlts;
                    }

                    BitSet *result = new BitSet();
                    for (auto config : configs) {
                        result->set(config->alt);
                    }

                    return result;
                }
            }
        }
    }
}

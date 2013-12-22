#include "FailedPredicateException.h"
#include "Java/src/org/antlr/v4/runtime/atn/ATNState.h"
#include "Java/src/org/antlr/v4/runtime/atn/AbstractPredicateTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/PredicateTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::ATNState;
                using org::antlr::v4::runtime::atn::AbstractPredicateTransition;
                using org::antlr::v4::runtime::atn::PredicateTransition;
                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Nullable;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                FailedPredicateException::FailedPredicateException(Parser *recognizer) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                FailedPredicateException::FailedPredicateException(Parser *recognizer, const std::wstring &predicate) {
                }

                FailedPredicateException::FailedPredicateException(Parser *recognizer, const std::wstring &predicate, const std::wstring &message) : RecognitionException(formatMessage(predicate, message), recognizer, recognizer->getInputStream(), recognizer->_ctx), ruleIndex((static_cast<PredicateTransition*>(trans))->ruleIndex), predicateIndex((static_cast<PredicateTransition*>(trans))->predIndex), predicate(predicate) {
                    ATNState *s = recognizer->getInterpreter()->atn->states[recognizer->getState()];

                    AbstractPredicateTransition *trans = static_cast<AbstractPredicateTransition*>(s->transition(0));
                    if (dynamic_cast<PredicateTransition*>(trans) != nullptr) {
                    } else {
                        this->ruleIndex = 0;
                        this->predicateIndex = 0;
                    }

                    this->setOffendingToken(recognizer->getCurrentToken());
                }

                int FailedPredicateException::getRuleIndex() {
                    return ruleIndex;
                }

                int FailedPredicateException::getPredIndex() {
                    return predicateIndex;
                }

                std::wstring FailedPredicateException::getPredicate() {
                    return predicate;
                }

                std::wstring FailedPredicateException::formatMessage(const std::wstring &predicate, const std::wstring &message) {
                    if (message != L"") {
                        return message;
                    }

                    return std::wstring::format(Locale::getDefault(), L"failed predicate: {%s}?", predicate);
                }
            }
        }
    }
}

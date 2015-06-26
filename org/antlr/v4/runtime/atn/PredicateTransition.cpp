#include "PredicateTransition.h"
#include "stringconverter.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::NotNull;

                    PredicateTransition::PredicateTransition(ATNState *target, int ruleIndex, int predIndex, bool isCtxDependent) : AbstractPredicateTransition(target), ruleIndex(ruleIndex), predIndex(predIndex), isCtxDependent(isCtxDependent) {
                    }

                    int PredicateTransition::getSerializationType() {
                        return PREDICATE;
                    }

                    bool PredicateTransition::isEpsilon() {
                        return true;
                    }

                    bool PredicateTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return false;
                    }

                    org::antlr::v4::runtime::atn::SemanticContext::Predicate *PredicateTransition::getPredicate() {
                        // TODO: who is responsible for managing this memory?
                        return new SemanticContext::Predicate(ruleIndex, predIndex, isCtxDependent);
                    }

                    std::wstring PredicateTransition::toString() {
                        return std::wstring(L"pred_") + antlrcpp::StringConverterHelper::toString(ruleIndex) + std::wstring(L":") + antlrcpp::StringConverterHelper::toString(predIndex);
                    }
                }
            }
        }
    }
}

#include "PrecedencePredicateTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::NotNull;

                    PrecedencePredicateTransition::PrecedencePredicateTransition(ATNState *target, int precedence) : AbstractPredicateTransition(target), precedence(precedence) {
                    }

                    int PrecedencePredicateTransition::getSerializationType() {
                        return PRECEDENCE;
                    }

                    bool PrecedencePredicateTransition::isEpsilon() {
                        return true;
                    }

                    bool PrecedencePredicateTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return false;
                    }

                    org::antlr::v4::runtime::atn::SemanticContext::PrecedencePredicate *PrecedencePredicateTransition::getPredicate() {
                        return new SemanticContext::PrecedencePredicate(precedence);
                    }

                    std::wstring PrecedencePredicateTransition::toString() {
                        return precedence + std::wstring(L" >= _p");
                    }
                }
            }
        }
    }
}

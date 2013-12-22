#include "EpsilonTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::NotNull;

                    EpsilonTransition::EpsilonTransition(ATNState *target) : Transition(target) {
                    }

                    int EpsilonTransition::getSerializationType() {
                        return EPSILON;
                    }

                    bool EpsilonTransition::isEpsilon() {
                        return true;
                    }

                    bool EpsilonTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return false;
                    }

                    std::wstring EpsilonTransition::toString() {
                        return L"epsilon";
                    }
                }
            }
        }
    }
}

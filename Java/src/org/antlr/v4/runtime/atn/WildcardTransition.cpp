#include "WildcardTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::NotNull;

                    WildcardTransition::WildcardTransition(ATNState *target) : Transition(target) {
                    }

                    int WildcardTransition::getSerializationType() {
                        return WILDCARD;
                    }

                    bool WildcardTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return symbol >= minVocabSymbol && symbol <= maxVocabSymbol;
                    }

                    std::wstring WildcardTransition::toString() {
                        return L".";
                    }
                }
            }
        }
    }
}

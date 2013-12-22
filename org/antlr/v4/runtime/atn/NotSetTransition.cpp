#include "NotSetTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    NotSetTransition::NotSetTransition(ATNState *target, IntervalSet *set) : SetTransition(target, set) {
                    }

                    int NotSetTransition::getSerializationType() {
                        return NOT_SET;
                    }

                    bool NotSetTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return symbol >= minVocabSymbol && symbol <= maxVocabSymbol && !SetTransition::matches(symbol, minVocabSymbol, maxVocabSymbol);
                    }

                    std::wstring NotSetTransition::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return L'~' + SetTransition::toString();
                    }
                }
            }
        }
    }
}

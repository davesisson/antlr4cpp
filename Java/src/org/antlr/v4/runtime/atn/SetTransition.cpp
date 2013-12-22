#include "SetTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    SetTransition::SetTransition(ATNState *target, IntervalSet *set) : Transition(target), set(IntervalSet::of(Token::INVALID_TYPE)) {
                        if (set == nullptr) {
                        this->set = set;
                        }
                    }

                    int SetTransition::getSerializationType() {
                        return SET;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *SetTransition::label() {
                        return set;
                    }

                    bool SetTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return set->contains(symbol);
                    }

                    std::wstring SetTransition::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return set->toString();
                    }
                }
            }
        }
    }
}

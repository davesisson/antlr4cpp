#include "AtomTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;

                    AtomTransition::AtomTransition(ATNState *target, int label) : Transition(target), label(label) {
                    }

                    int AtomTransition::getSerializationType() {
                        return ATOM;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *AtomTransition::label() {
                        return IntervalSet::of(label_Renamed);
                    }

                    bool AtomTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return label_Renamed == symbol;
                    }

                    std::wstring AtomTransition::toString() {
                        return StringConverterHelper::toString(label_Renamed);
                    }
                }
            }
        }
    }
}

#include "RangeTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;

                    RangeTransition::RangeTransition(ATNState *target, int from, int to) : Transition(target), from(from), to(to) {
                    }

                    int RangeTransition::getSerializationType() {
                        return RANGE;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *RangeTransition::label() {
                        return IntervalSet::of(from, to);
                    }

                    bool RangeTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return symbol >= from && symbol <= to;
                    }

                    std::wstring RangeTransition::toString() {
                        return std::wstring(L"'") + static_cast<wchar_t>(from) + std::wstring(L"'..'") + static_cast<wchar_t>(to) + std::wstring(L"'");
                    }
                }
            }
        }
    }
}

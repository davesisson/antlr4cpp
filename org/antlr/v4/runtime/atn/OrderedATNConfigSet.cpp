#include "OrderedATNConfigSet.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::ObjectEqualityComparator;

                    OrderedATNConfigSet::LexerConfigHashSet::LexerConfigHashSet() : AbstractConfigHashSet(ObjectEqualityComparator::INSTANCE) {
                    }

                    OrderedATNConfigSet::OrderedATNConfigSet() {
                        this->configLookup = new LexerConfigHashSet();
                    }
                }
            }
        }
    }
}

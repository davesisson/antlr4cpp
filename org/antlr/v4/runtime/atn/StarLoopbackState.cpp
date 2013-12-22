#include "StarLoopbackState.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    org::antlr::v4::runtime::atn::StarLoopEntryState *StarLoopbackState::getLoopEntryState() {
                        return static_cast<StarLoopEntryState*>(transition(0)->target);
                    }

                    int StarLoopbackState::getStateType() {
                        return STAR_LOOP_BACK;
                    }
                }
            }
        }
    }
}

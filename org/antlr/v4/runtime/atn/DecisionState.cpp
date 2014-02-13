#include "DecisionState.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    void DecisionState::InitializeInstanceFields() {
                        decision = -1;
                        nonGreedy = false;
                    }
                }
            }
        }
    }
}

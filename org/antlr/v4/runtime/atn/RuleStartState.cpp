#include "RuleStartState.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    int RuleStartState::getStateType() {
                        return RULE_START;
                    }

                    void RuleStartState::InitializeInstanceFields() {
                        isPrecedenceRule = false;
                    }
                }
            }
        }
    }
}

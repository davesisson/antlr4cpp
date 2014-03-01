#include "InterpreterRuleContext.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Nullable;

                InterpreterRuleContext::InterpreterRuleContext(ParserRuleContext *parent, int invokingStateNumber, int ruleIndex) : ParserRuleContext(parent, invokingStateNumber), ruleIndex(ruleIndex) {
                }

                int InterpreterRuleContext::getRuleIndex() {
                    return ruleIndex;
                }
            }
        }
    }
}

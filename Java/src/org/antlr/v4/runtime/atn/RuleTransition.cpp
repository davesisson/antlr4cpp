#include "RuleTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::NotNull;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    RuleTransition::RuleTransition(RuleStartState *ruleStart, int ruleIndex, ATNState *followState) {
                    }

                    RuleTransition::RuleTransition(RuleStartState *ruleStart, int ruleIndex, int precedence, ATNState *followState) : Transition(ruleStart), ruleIndex(ruleIndex), precedence(precedence) {
                        this->followState = followState;
                    }

                    int RuleTransition::getSerializationType() {
                        return RULE;
                    }

                    bool RuleTransition::isEpsilon() {
                        return true;
                    }

                    bool RuleTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return false;
                    }
                }
            }
        }
    }
}

#include "ActionTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::NotNull;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ActionTransition::ActionTransition(ATNState *target, int ruleIndex) {
                    }

                    ActionTransition::ActionTransition(ATNState *target, int ruleIndex, int actionIndex, bool isCtxDependent) : Transition(target), ruleIndex(ruleIndex), actionIndex(actionIndex), isCtxDependent(isCtxDependent) {
                    }

                    int ActionTransition::getSerializationType() {
                        return ACTION;
                    }

                    bool ActionTransition::isEpsilon() {
                        return true; // we are to be ignored by analysis 'cept for predicates
                    }

                    bool ActionTransition::matches(int symbol, int minVocabSymbol, int maxVocabSymbol) {
                        return false;
                    }

                    std::wstring ActionTransition::toString() {
                        return std::wstring(L"action_") + ruleIndex + std::wstring(L":") + actionIndex;
                    }
                }
            }
        }
    }
}

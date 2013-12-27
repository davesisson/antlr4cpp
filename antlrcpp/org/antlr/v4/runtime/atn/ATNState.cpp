#include "ATNState.h"
#include "ATN.h"
#include "Transition.h"
#include "IntervalSet.h"
fds
namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::IntervalSet;
const std::vector<std::wstring> ATNState::serializationNames = java::util::Collections::unmodifiableList(java::util::Arrays::asList(L"INVALID", L"BASIC", L"RULE_START", L"BLOCK_START", L"PLUS_BLOCK_START", L"STAR_BLOCK_START", L"TOKEN_START", L"RULE_STOP", L"BLOCK_END", L"STAR_LOOP_BACK", L"STAR_LOOP_ENTRY", L"PLUS_LOOP_BACK", L"LOOP_END"));

                    int ATNState::hashCode() {
                        return stateNumber;
                    }

                    bool ATNState::equals(void *o) {
                        // are these states same object?
                        if (dynamic_cast<ATNState*>(o) != nullptr) {
                            return stateNumber == (static_cast<ATNState*>(o))->stateNumber;
                        }
                        return false;
                    }

                    bool ATNState::isNonGreedyExitState() {
                        return false;
                    }

                    std::wstring ATNState::toString() {
                        return StringConverterHelper::toString(stateNumber);
                    }

                    Transition *ATNState::getTransitions() {
                        return transitions.toArray(new Transition[transitions.size()]);
                    }

                    int ATNState::getNumberOfTransitions() {
                        return transitions.size();
                    }

                    void ATNState::addTransition(Transition *e) {
                        addTransition(transitions.size(), e);
                    }

                    void ATNState::addTransition(int index, Transition *e) {
                        if (transitions.empty()) {
                            epsilonOnlyTransitions = e->isEpsilon();
                        } else if (epsilonOnlyTransitions != e->isEpsilon()) {
                            System::err::format(Locale::getDefault(), L"ATN state %d has both epsilon and non-epsilon transitions.\n", stateNumber);
                            epsilonOnlyTransitions = false;
                        }

                        transitions.push_back(index, e);
                    }

                    org::antlr::v4::runtime::atn::Transition *ATNState::transition(int i) {
                        return transitions[i];
                    }

                    void ATNState::setTransition(int i, Transition *e) {
                        transitions[i] = e;
                    }

                    org::antlr::v4::runtime::atn::Transition *ATNState::removeTransition(int index) {
                        return transitions.remove(index);
                    }

                    bool ATNState::onlyHasEpsilonTransitions() {
                        return epsilonOnlyTransitions;
                    }

                    void ATNState::setRuleIndex(int ruleIndex) {
                        this->ruleIndex = ruleIndex;
                    }

                    void ATNState::InitializeInstanceFields() {
                        atn = 0;
                        stateNumber = INVALID_STATE_NUMBER;
                        ruleIndex = 0;
                        epsilonOnlyTransitions = false;
                    }
                }
            }
        }
    }
}

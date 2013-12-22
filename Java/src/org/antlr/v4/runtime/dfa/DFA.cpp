#include "DFA.h"
#include "Java/src/org/antlr/v4/runtime/dfa/DFASerializer.h"
#include "Java/src/org/antlr/v4/runtime/dfa/LexerDFASerializer.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace dfa {
                    using org::antlr::v4::runtime::atn::DecisionState;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    DFA::DFA(DecisionState *atnStartState) : states(new java::util::HashMap<DFAState, DFAState>()) {
                    }

                    DFA::DFA(DecisionState *atnStartState, int decision) : decision(decision), atnStartState(atnStartState), states(new java::util::HashMap<DFAState, DFAState>()) {
                    }

                    std::vector<DFAState*> DFA::getStates() {
                        std::vector<DFAState*> result = std::vector<DFAState*>(states->keySet());
                        Collections::sort(result, new ComparatorAnonymousInnerClassHelper(this));

                        return result;
                    }

                    DFA::ComparatorAnonymousInnerClassHelper::ComparatorAnonymousInnerClassHelper(DFA *outerInstance) {
                        this->outerInstance = outerInstance;
                    }

                    int DFA::ComparatorAnonymousInnerClassHelper::compare(DFAState *o1, DFAState *o2) {
                        return o1->stateNumber - o2->stateNumber;
                    }

                    std::wstring DFA::toString() {
                        return toString(nullptr);
                    }

                    std::wstring DFA::toString(std::wstring tokenNames[]) {
                        if (s0 == nullptr) {
                            return L"";
                        }
                        DFASerializer *serializer = new DFASerializer(this,tokenNames);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return serializer->toString();
                    }

                    std::wstring DFA::toLexerString() {
                        if (s0 == nullptr) {
                            return L"";
                        }
                        DFASerializer *serializer = new LexerDFASerializer(this);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return serializer->toString();
                    }
                }
            }
        }
    }
}

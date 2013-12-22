#include "DFASerializer.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace dfa {
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    DFASerializer::DFASerializer(DFA *dfa, std::wstring tokenNames[]) : dfa(dfa), tokenNames(tokenNames) {
                    }

                    std::wstring DFASerializer::toString() {
                        if (dfa->s0 == nullptr) {
                            return L"";
                        }
                        StringBuilder *buf = new StringBuilder();
                        std::vector<DFAState*> states = dfa->getStates();
                        for (auto s : states) {
                            int n = 0;
                            if (s.edges != nullptr) {
                                n = s.edges::length;
                            }
                            for (int i = 0; i < n; i++) {
                                DFAState *t = s.edges[i];
                                if (t != nullptr && t->stateNumber != int::MAX_VALUE) {
                                    buf->append(getStateString(s));
                                    std::wstring label = getEdgeLabel(i);
                                    buf->append(L"-")->append(label)->append(L"->")->append(getStateString(t))->append(L'\n');
                                }
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        std::wstring output = buf->toString();
                        if (output.length() == 0) {
                            return L"";
                        }
                        //return Utils.sortLinesInString(output);
                        return output;
                    }

                    std::wstring DFASerializer::getEdgeLabel(int i) {
                        std::wstring label;
                        if (i == 0) {
                            return L"EOF";
                        }
                        if (tokenNames != nullptr) {
                            label = tokenNames[i - 1];
                        } else {
                            label = StringConverterHelper::toString(i - 1);
                        }
                        return label;
                    }

                    std::wstring DFASerializer::getStateString(DFAState *s) {
                        int n = s->stateNumber;
                        const std::wstring baseStateStr = (s->isAcceptState ? L":" : L"") + std::wstring(L"s") + n + (s->requiresFullContext ? L"^" : L"");
                        if (s->isAcceptState) {
                            if (s->predicates != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                return baseStateStr + std::wstring(L"=>") + Arrays->toString(s->predicates);
                            } else {
                                return baseStateStr + std::wstring(L"=>") + s->prediction;
                            }
                        } else {
                            return baseStateStr;
                        }
                    }
                }
            }
        }
    }
}

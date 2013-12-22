#include "DFAState.h"
#include "Java/src/org/antlr/v4/runtime/misc/MurmurHash.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace dfa {
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::atn::ATN;
                    using org::antlr::v4::runtime::atn::ATNConfig;
                    using org::antlr::v4::runtime::atn::ATNConfigSet;
                    using org::antlr::v4::runtime::atn::ParserATNSimulator;
                    using org::antlr::v4::runtime::atn::SemanticContext;
                    using org::antlr::v4::runtime::misc::MurmurHash;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    DFAState::PredPrediction::PredPrediction(SemanticContext *pred, int alt) {
                        InitializeInstanceFields();
                        this->alt = alt;
                        this->pred = pred;
                    }

                    std::wstring DFAState::PredPrediction::toString() {
                        return std::wstring(L"(") + pred + std::wstring(L", ") + alt + std::wstring(L")");
                    }

                    void DFAState::PredPrediction::InitializeInstanceFields() {
                        alt = 0;
                    }

                    DFAState::DFAState() {
                        InitializeInstanceFields();
                    }

                    DFAState::DFAState(int stateNumber) {
                        InitializeInstanceFields();
                        this->stateNumber = stateNumber;
                    }

                    DFAState::DFAState(ATNConfigSet *configs) {
                        InitializeInstanceFields();
                        this->configs = configs;
                    }

                    Set<int> *DFAState::getAltSet() {
                        Set<int> *alts = std::set<int>();
                        if (configs != nullptr) {
                            for (auto c : configs) {
                                alts->add(c->alt);
                            }
                        }
                        if (alts->isEmpty()) {
                            return nullptr;
                        }
                        return alts;
                    }

                    int DFAState::hashCode() {
                        int hash = MurmurHash::initialize(7);
                        hash = MurmurHash::update(hash, configs->hashCode());
                        hash = MurmurHash::finish(hash, 1);
                        return hash;
                    }

                    bool DFAState::equals(void *o) {
                        // compare set of ATN configurations in this set with other
                        if (this == o) {
                            return true;
                        }

                        if (!(dynamic_cast<DFAState*>(o) != nullptr)) {
                            return false;
                        }

                        DFAState *other = static_cast<DFAState*>(o);
                        // TODO (sam): what to do when configs==null?
                        bool sameSet = this->configs->equals(other->configs);
                                        //		System.out.println("DFAState.equals: "+configs+(sameSet?"==":"!=")+other.configs);
                        return sameSet;
                    }

                    std::wstring DFAState::toString() {
                        StringBuilder *buf = new StringBuilder();
                        buf->append(stateNumber)->append(L":")->append(configs);
                        if (isAcceptState) {
                            buf->append(L"=>");
                            if (predicates != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                buf->append(Arrays->toString(predicates));
                            } else {
                                buf->append(prediction);
                            }
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    void DFAState::InitializeInstanceFields() {
                        stateNumber = -1;
                        configs = new org::antlr::v4::runtime::atn::ATNConfigSet();
                        isAcceptState = false;
                        prediction = 0;
                        lexerRuleIndex = -1;
                        lexerActionIndex = -1;
                        requiresFullContext = false;
                    }
                }
            }
        }
    }
}

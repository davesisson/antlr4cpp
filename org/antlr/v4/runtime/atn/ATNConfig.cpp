#include "ATNConfig.h"
#include "MurmurHash.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::Recognizer;
                    using org::antlr::v4::runtime::misc::MurmurHash;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    ATNConfig::ATNConfig(ATNConfig *old) : state(old->state), alt(old->alt), semanticContext(old->semanticContext) {
                        InitializeInstanceFields();
                        this->context = old->context;
                        this->reachesIntoOuterContext = old->reachesIntoOuterContext;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfig::ATNConfig(ATNState *state, int alt, PredictionContext *context) {
                    }

                    ATNConfig::ATNConfig(ATNState *state, int alt, PredictionContext *context, SemanticContext *semanticContext) : state(state), alt(alt), semanticContext(semanticContext) {
                        InitializeInstanceFields();
                        this->context = context;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfig::ATNConfig(ATNConfig *c, ATNState *state) {
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfig::ATNConfig(ATNConfig *c, ATNState *state, SemanticContext *semanticContext) {
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfig::ATNConfig(ATNConfig *c, SemanticContext *semanticContext) {
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfig::ATNConfig(ATNConfig *c, ATNState *state, PredictionContext *context) {
                    }

                    ATNConfig::ATNConfig(ATNConfig *c, ATNState *state, PredictionContext *context, SemanticContext *semanticContext) : state(state), alt(c->alt), semanticContext(semanticContext) {
                        InitializeInstanceFields();
                        this->context = context;
                        this->reachesIntoOuterContext = c->reachesIntoOuterContext;
                    }

                    bool ATNConfig::equals(void *o) {
                        if (!(dynamic_cast<ATNConfig*>(o) != nullptr)) {
                            return false;
                        }

                        return this->equals(static_cast<ATNConfig*>(o));
                    }

                    bool ATNConfig::equals(ATNConfig *other) {
                        if (this == other) {
                            return true;
                        } else if (other == nullptr) {
                            return false;
                        }

                        return this->state->stateNumber == other->state->stateNumber && this->alt == other->alt && (this->context == other->context || (this->context != nullptr && this->context->equals(other->context))) && this->semanticContext->equals(other->semanticContext);
                    }

                    int ATNConfig::hashCode() {
                        int hashCode = MurmurHash::initialize(7);
                        hashCode = MurmurHash::update(hashCode, state->stateNumber);
                        hashCode = MurmurHash::update(hashCode, alt);
                        hashCode = MurmurHash::update(hashCode, context);
                        hashCode = MurmurHash::update(hashCode, semanticContext);
                        hashCode = MurmurHash::finish(hashCode, 4);
                        return hashCode;
                    }

                    std::wstring ATNConfig::toString() {
                        return toString(nullptr, true);
                    }

template<typename T1, typename T1>
                    std::wstring ATNConfig::toString(Recognizer<T1> *recog, bool showAlt) {
                        StringBuilder *buf = new StringBuilder();
                                        //		if ( state.ruleIndex>=0 ) {
                                        //			if ( recog!=null ) buf.append(recog.getRuleNames()[state.ruleIndex]+":");
                                        //			else buf.append(state.ruleIndex+":");
                                        //		}
                        buf->append(L'(');
                        buf->append(state);
                        if (showAlt) {
                            buf->append(L",");
                            buf->append(alt);
                        }
                        if (context != nullptr) {
                            buf->append(L",[");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            buf->append(context->toString());
                            buf->append(L"]");
                        }
                        if (semanticContext != nullptr && semanticContext != SemanticContext::NONE) {
                            buf->append(L",");
                            buf->append(semanticContext);
                        }
                        if (reachesIntoOuterContext > 0) {
                            buf->append(L",up=")->append(reachesIntoOuterContext);
                        }
                        buf->append(L')');
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    void ATNConfig::InitializeInstanceFields() {
                        reachesIntoOuterContext = 0;
                    }
                }
            }
        }
    }
}

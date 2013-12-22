#include "LexerATNConfig.h"
#include "Java/src/org/antlr/v4/runtime/misc/MurmurHash.h"
#include "Java/src/org/antlr/v4/runtime/atn/DecisionState.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::MurmurHash;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    LexerATNConfig::LexerATNConfig(ATNState *state, int alt, PredictionContext *context) : ATNConfig(state, alt, context, SemanticContext::NONE), passedThroughNonGreedyDecision(false) {
                        InitializeInstanceFields();
                    }

                    LexerATNConfig::LexerATNConfig(ATNState *state, int alt, PredictionContext *context, int actionIndex) : ATNConfig(state, alt, context, SemanticContext::NONE), passedThroughNonGreedyDecision(false) {
                        InitializeInstanceFields();
                        this->lexerActionIndex = actionIndex;
                    }

                    LexerATNConfig::LexerATNConfig(LexerATNConfig *c, ATNState *state) : ATNConfig(c, state, c->context, c->semanticContext), passedThroughNonGreedyDecision(checkNonGreedyDecision(c, state)) {
                        InitializeInstanceFields();
                        this->lexerActionIndex = c->lexerActionIndex;
                    }

                    LexerATNConfig::LexerATNConfig(LexerATNConfig *c, ATNState *state, int actionIndex) : ATNConfig(c, state, c->context, c->semanticContext), passedThroughNonGreedyDecision(checkNonGreedyDecision(c, state)) {
                        InitializeInstanceFields();
                        this->lexerActionIndex = actionIndex;
                    }

                    LexerATNConfig::LexerATNConfig(LexerATNConfig *c, ATNState *state, PredictionContext *context) : ATNConfig(c, state, context, c->semanticContext), passedThroughNonGreedyDecision(checkNonGreedyDecision(c, state)) {
                        InitializeInstanceFields();
                        this->lexerActionIndex = c->lexerActionIndex;
                    }

                    bool LexerATNConfig::hasPassedThroughNonGreedyDecision() {
                        return passedThroughNonGreedyDecision;
                    }

                    int LexerATNConfig::hashCode() {
                        int hashCode = MurmurHash::initialize(7);
                        hashCode = MurmurHash::update(hashCode, state->stateNumber);
                        hashCode = MurmurHash::update(hashCode, alt);
                        hashCode = MurmurHash::update(hashCode, context);
                        hashCode = MurmurHash::update(hashCode, semanticContext);
                        hashCode = MurmurHash::update(hashCode, passedThroughNonGreedyDecision ? 1 : 0);
                        hashCode = MurmurHash::finish(hashCode, 5);
                        return hashCode;
                    }

                    bool LexerATNConfig::equals(ATNConfig *other) {
                        if (this == other) {
                            return true;
                        } else if (!(dynamic_cast<LexerATNConfig*>(other) != nullptr)) {
                            return false;
                        }

                        LexerATNConfig *lexerOther = static_cast<LexerATNConfig*>(other);
                        if (passedThroughNonGreedyDecision != lexerOther->passedThroughNonGreedyDecision) {
                            return false;
                        }

                        return ATNConfig::equals(other);
                    }

                    bool LexerATNConfig::checkNonGreedyDecision(LexerATNConfig *source, ATNState *target) {
                        return source->passedThroughNonGreedyDecision || dynamic_cast<DecisionState*>(target) != nullptr && (static_cast<DecisionState*>(target))->nonGreedy;
                    }

                    void LexerATNConfig::InitializeInstanceFields() {
                        lexerActionIndex = -1;
                    }
                }
            }
        }
    }
}

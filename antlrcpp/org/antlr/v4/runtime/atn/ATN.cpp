#include "ATN.h"
#include "LL1Analyzer.h"
#include "Token.h"
#include "RuleTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::ParserRuleContext;
                    using org::antlr::v4::runtime::RuleContext;
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    ATN::ATN(ATNType grammarType, int maxTokenType) : grammarType(grammarType), maxTokenType(maxTokenType), states(new java::util::ArrayList<ATNState>()), decisionToState(new java::util::ArrayList<DecisionState>()), modeNameToStartState(new java::util::LinkedHashMap<String, TokensStartState>()), modeToStartState(new java::util::ArrayList<TokensStartState>()) {
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *ATN::nextTokens(ATNState *s, RuleContext *ctx) {
                        LL1Analyzer *anal = new LL1Analyzer(this);
                        IntervalSet *next = anal->LOOK(s, ctx);
                        return next;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *ATN::nextTokens(ATNState *s) {
                        if (s->nextTokenWithinRule != nullptr) {
                            return s->nextTokenWithinRule;
                        }
                        s->nextTokenWithinRule = nextTokens(s, nullptr);
                        s->nextTokenWithinRule->setReadonly(true);
                        return s->nextTokenWithinRule;
                    }

                    void ATN::addState(ATNState *state) {
                        if (state != nullptr) {
                            state->atn = this;
                            state->stateNumber = states.size();
                        }

                        states.push_back(state);
                    }

                    void ATN::removeState(ATNState *state) {
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                        delete states[state->stateNumber]; // just free mem, don't shift states in list
                    }

                    int ATN::defineDecisionState(DecisionState *s) {
                        decisionToState.push_back(s);
                        s->decision = decisionToState.size() - 1;
                        return s->decision;
                    }

                    org::antlr::v4::runtime::atn::DecisionState *ATN::getDecisionState(int decision) {
                        if (!decisionToState.empty()) {
                            return decisionToState[decision];
                        }
                        return nullptr;
                    }

                    int ATN::getNumberOfDecisions() {
                        return decisionToState.size();
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *ATN::getExpectedTokens(int stateNumber, RuleContext *context) {
                        if (stateNumber < 0 || stateNumber >= states.size()) {
                            throw IllegalArgumentException(L"Invalid state number.");
                        }

                        RuleContext *ctx = context;
                        ATNState *s = states[stateNumber];
                        IntervalSet *following = nextTokens(s);
                        if (!following->contains(Token::EPSILON)) {
                            return following;
                        }

                        IntervalSet *expected = new IntervalSet();
                        expected->addAll(following);
                        expected->remove(Token::EPSILON);
                        while (ctx != nullptr && ctx->invokingState >= 0 && following->contains(Token::EPSILON)) {
                            ATNState *invokingState = states[ctx->invokingState];
                            RuleTransition *rt = static_cast<RuleTransition*>(invokingState->transition(0));
                            following = nextTokens(rt->followState);
                            expected->addAll(following);
                            expected->remove(Token::EPSILON);
                            ctx = ctx->parent;
                        }

                        if (following->contains(Token::EPSILON)) {
                            expected->add(Token::EOF);
                        }

                        return expected;
                    }
                }
            }
        }
    }
}

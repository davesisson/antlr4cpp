#include "LL1Analyzer.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleStopState.h"
#include "Java/src/org/antlr/v4/runtime/atn/Transition.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/SingletonPredictionContext.h"
#include "Java/src/org/antlr/v4/runtime/atn/AbstractPredicateTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/WildcardTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/NotSetTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::RuleContext;
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    LL1Analyzer::LL1Analyzer(ATN *atn) : atn(atn) {
                    }

                    IntervalSet *LL1Analyzer::getDecisionLookahead(ATNState *s) {
                                        //		System.out.println("LOOK("+s.stateNumber+")");
                        if (s == nullptr) {
                            return nullptr;
                        }

                        IntervalSet look[s->getNumberOfTransitions()];
                        for (int alt = 0; alt < s->getNumberOfTransitions(); alt++) {
                            look[alt] = new IntervalSet();
                            Set<ATNConfig*> *lookBusy = std::set<ATNConfig*>();
                            bool seeThruPreds = false; // fail to get lookahead upon pred
                            _LOOK(s->transition(alt)->target, nullptr, PredictionContext::EMPTY, look[alt], lookBusy, new BitSet(), seeThruPreds, false);
                            // Wipe out lookahead for this alternative if we found nothing
                            // or we had a predicate when we !seeThruPreds
                            if (look[alt]->size() == 0 || look[alt]->contains(HIT_PRED)) {
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete look[alt];
                            }
                        }
                        return look;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *LL1Analyzer::LOOK(ATNState *s, RuleContext *ctx) {
                        return LOOK(s, nullptr, ctx);
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *LL1Analyzer::LOOK(ATNState *s, ATNState *stopState, RuleContext *ctx) {
                           IntervalSet *r = new IntervalSet();
                        bool seeThruPreds = true; // ignore preds; get all lookahead
                        PredictionContext *lookContext = ctx != nullptr ? PredictionContext::fromRuleContext(s->atn, ctx) : nullptr;
                           _LOOK(s, stopState, lookContext, r, std::set<ATNConfig*>(), new BitSet(), seeThruPreds, true);
                           return r;
                    }

                    void LL1Analyzer::_LOOK(ATNState *s, ATNState *stopState, PredictionContext *ctx, IntervalSet *look, Set<ATNConfig*> *lookBusy, BitSet *calledRuleStack, bool seeThruPreds, bool addEOF) {
                                        //		System.out.println("_LOOK("+s.stateNumber+", ctx="+ctx);
                        ATNConfig *c = new ATNConfig(s, 0, ctx);
                        if (!lookBusy->add(c)) {
                            return;
                        }

                        if (s == stopState) {
                            if (ctx == nullptr) {
                                look->add(Token::EPSILON);
                                return;
                            } else if (ctx->isEmpty() && addEOF) {
                                look->add(Token::EOF);
                                return;
                            }
                        }

                        if (dynamic_cast<RuleStopState*>(s) != nullptr) {
                            if (ctx == nullptr) {
                                look->add(Token::EPSILON);
                                return;
                            } else if (ctx->isEmpty() && addEOF) {
                                look->add(Token::EOF);
                                return;
                            }

                            if (ctx != PredictionContext::EMPTY) {
                                // run thru all possible stack tops in ctx
                                for (int i = 0; i < ctx->size(); i++) {
                                    ATNState *returnState = atn->states[ctx->getReturnState(i)];
                                        //					System.out.println("popping back to "+retState);

                                    bool removed = calledRuleStack->get(returnState->ruleIndex);
                                    try {
                                        calledRuleStack->clear(returnState->ruleIndex);
                                        _LOOK(returnState, stopState, ctx->getParent(i), look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                                    } finally {
                                        if (removed) {
                                            calledRuleStack->set(returnState->ruleIndex);
                                        }
                                    }
                                }
                                return;
                            }
                        }

                        int n = s->getNumberOfTransitions();
                        for (int i = 0; i < n; i++) {
                            Transition *t = s->transition(i);
                            if (t->getClass() == RuleTransition::typeid) {
                                if (calledRuleStack->get((static_cast<RuleTransition*>(t))->target->ruleIndex)) {
                                    continue;
                                }

                                PredictionContext *newContext = SingletonPredictionContext::create(ctx, (static_cast<RuleTransition*>(t))->followState->stateNumber);

                                try {
                                    calledRuleStack->set((static_cast<RuleTransition*>(t))->target->ruleIndex);
                                    _LOOK(t->target, stopState, newContext, look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                                } finally {
                                    calledRuleStack->clear((static_cast<RuleTransition*>(t))->target->ruleIndex);
                                }
                            } else if (dynamic_cast<AbstractPredicateTransition*>(t) != nullptr) {
                                if (seeThruPreds) {
                                    _LOOK(t->target, stopState, ctx, look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                                } else {
                                    look->add(HIT_PRED);
                                }
                            } else if (t->isEpsilon()) {
                                _LOOK(t->target, stopState, ctx, look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                            } else if (t->getClass() == WildcardTransition::typeid) {
                                look->addAll(IntervalSet::of(Token::MIN_USER_TOKEN_TYPE, atn->maxTokenType));
                            } else {
                                        //				System.out.println("adding "+ t);
                                IntervalSet *set = t->label();
                                if (set != nullptr) {
                                    if (dynamic_cast<NotSetTransition*>(t) != nullptr) {
                                        set = set->complement(IntervalSet::of(Token::MIN_USER_TOKEN_TYPE, atn->maxTokenType));
                                    }
                                    look->addAll(set);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

﻿#include "LL1Analyzer.h"
#include "RuleStopState.h"
#include "Transition.h"
#include "RuleTransition.h"
#include "SingletonPredictionContext.h"
#include "AbstractPredicateTransition.h"
#include "WildcardTransition.h"
#include "NotSetTransition.h"
#include "IntervalSet.h"
#include "ATNConfig.h"
#include "ATN.h"

#include <typeinfo>
/*
 * [The "BSD license"]
 *  Copyright (c) 2013 Terence Parr
 *  Copyright (c) 2013 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
 
                    LL1Analyzer::LL1Analyzer(ATN *atn) : atn(atn) {
                    }

                    std::vector<misc::IntervalSet*> LL1Analyzer::getDecisionLookahead(ATNState *s) {
                                        //		System.out.println("LOOK("+s.stateNumber+")");
                        std::vector<misc::IntervalSet*> look;
                        
                        if (s == nullptr) {
                            return look;
                        }
                        
                        // need s->getNumberOfTransitions()); of them
                        for (int alt = 0; alt < s->getNumberOfTransitions(); alt++) {
                            look[alt] = new misc::IntervalSet(0);
                            std::set<ATNConfig*> *lookBusy = new std::set<ATNConfig*>();
                            bool seeThruPreds = false; // fail to get lookahead upon pred
                            _LOOK(s->transition(alt)->target, nullptr, (PredictionContext*)PredictionContext::EMPTY, look[alt], lookBusy, new antlrcpp::BitSet(), seeThruPreds, false);
                            // Wipe out lookahead for this alternative if we found nothing
                            // or we had a predicate when we !seeThruPreds
                            if (look[alt]->size() == 0 || look[alt]->contains(HIT_PRED)) {
                                // TODO: memory managment, delete
                                delete look[alt];
                            }
                        }
                        return look;
                    }

                    misc::IntervalSet *LL1Analyzer::LOOK(ATNState *s, RuleContext *ctx) {
                        return LOOK(s, nullptr, ctx);
                    }

                    misc::IntervalSet *LL1Analyzer::LOOK(ATNState *s, ATNState *stopState, RuleContext *ctx) {
                           misc::IntervalSet *r = new misc::IntervalSet(0);
                        bool seeThruPreds = true; // ignore preds; get all lookahead
                        PredictionContext *lookContext = ctx != nullptr ? PredictionContext::fromRuleContext(s->atn, ctx) : nullptr;
                           _LOOK(s, stopState, lookContext, r, new std::set<ATNConfig*>(), new antlrcpp::BitSet(), seeThruPreds, true);
                           return r;
                    }

                    void LL1Analyzer::_LOOK(ATNState *s, ATNState *stopState, PredictionContext *ctx, misc::IntervalSet *look, std::set<ATNConfig*> *lookBusy,  antlrcpp::BitSet *calledRuleStack, bool seeThruPreds, bool addEOF) {
                                        //		System.out.println("_LOOK("+s.stateNumber+", ctx="+ctx);
                        ATNConfig *c = new ATNConfig(s, 0, ctx);
                        
                        if (!lookBusy->insert(c).second) {
                            return;
                        }

                        if (s == stopState) {
                            if (ctx == nullptr) {
                                look->add(Token::EPSILON);
                                return;
                            } else if (ctx->isEmpty() && addEOF) {
                                look->add(Token::_EOF);
                                return;
                            }
                        }

                        if (dynamic_cast<RuleStopState*>(s) != nullptr) {
                            if (ctx == nullptr) {
                                look->add(Token::EPSILON);
                                return;
                            } else if (ctx->isEmpty() && addEOF) {
                                look->add(Token::_EOF);
                                return;
                            }

                            if (ctx != (PredictionContext*)PredictionContext::EMPTY) {
                                // run thru all possible stack tops in ctx
                                for (int i = 0; i < ctx->size(); i++) {
                                    ATNState *returnState = atn->states[ctx->getReturnState(i)];
                                        //					System.out.println("popping back to "+retState);

                                    bool removed = calledRuleStack->data.test(returnState->ruleIndex);
                                    try {
                                        calledRuleStack->data[returnState->ruleIndex] = false;
                                        _LOOK(returnState, stopState, ctx->getParent(i), look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                                    }
                                    catch(...) {
                                        // Just move to the next steps as a "finally" clause
                                    }
                                    if (removed) {
                                        calledRuleStack->set(returnState->ruleIndex);
    
                                    }
                                }
                                return;
                            }
                        }

                        int n = s->getNumberOfTransitions();
                        for (int i = 0; i < n; i++) {
                            Transition *t = s->transition(i);
                            
                            if (typeid(t) == typeid(RuleTransition)) {
                                if ( (*calledRuleStack).data[(static_cast<RuleTransition*>(t))->target->ruleIndex]) {
                                    continue;
                                }

                                PredictionContext *newContext = SingletonPredictionContext::create(ctx, (static_cast<RuleTransition*>(t))->followState->stateNumber);

                                try {
                                    calledRuleStack->set((static_cast<RuleTransition*>(t))->target->ruleIndex);
                                    _LOOK(t->target, stopState, newContext, look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                                }
                                catch(...) {
                                    // Just move to the next steps as a "finally" clause
                                }
                                calledRuleStack->data[((static_cast<RuleTransition*>(t))->target->ruleIndex)] = false;
                                
                            } else if (dynamic_cast<AbstractPredicateTransition*>(t) != nullptr) {
                                if (seeThruPreds) {
                                    _LOOK(t->target, stopState, ctx, look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                                } else {
                                    look->add(HIT_PRED);
                                }
                            } else if (t->isEpsilon()) {
                                _LOOK(t->target, stopState, ctx, look, lookBusy, calledRuleStack, seeThruPreds, addEOF);
                            } else if (typeid(t) == typeid(WildcardTransition)) {
                                look->addAll(misc::IntervalSet::of(Token::MIN_USER_TOKEN_TYPE, atn->maxTokenType));
                            } else {
                                        //				System.out.println("adding "+ t);
                                misc::IntervalSet *set = t->label();
                                if (set != nullptr) {
                                    if (dynamic_cast<NotSetTransition*>(t) != nullptr) {
                                        set = set->complement(misc::IntervalSet::of(Token::MIN_USER_TOKEN_TYPE, atn->maxTokenType));
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

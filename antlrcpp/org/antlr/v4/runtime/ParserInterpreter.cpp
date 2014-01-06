#include "ParserInterpreter.h"
#include <deque>
#include "ATN.h"
#include "DFA.h"

/*
 * [The "BSD license"]
 * Copyright (c) 2013 Terence Parr
 * Copyright (c) 2013 Sam Harwell
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {

#ifdef TODO 
                this is a mess
                ParserInterpreter::ParserInterpreter(const std::wstring &grammarFileName, std::vector<std::wstring> *tokenNames, std::vector<std::wstring> *ruleNames, atn::ATN *atn, TokenStream *input) : Parser(_input), grammarFileName(grammarFileName), atn(atn), pushRecursionContextStates(new std::bitset<DEFAULT_BITSET_SIZE>()), decisionToDFA(new dfa::DFA()/*[atn->getNumberOfDecisions()]*/), tokenNames(tokenNames->toArray(new std::wstring[tokenNames->size()])), ruleNames(ruleNames->toArray(new std::wstring[ruleNames->size()])), sharedContextCache(new /*org::antlr::v4::runtime->atn->*/PredictionContextCache()), _parentContextStack(new std::deque<misc::Pair<ParserRuleContext, int>>()) {
              

                    for (int i = 0; i < decisionToDFA->length(); i++) {
                        decisionToDFA[i] = new dfa::DFA(atn->getDecisionState(i), i);
                    }

                    // identify the ATN states where pushNewRecursionContext must be called
                    for (auto state : atn->states) {
                        if (!(dynamic_cast<StarLoopEntryState*>(state) != nullptr)) {
                            continue;
                        }

                        RuleStartState *ruleStartState = atn->ruleToStartState[state.ruleIndex];
                        if (!ruleStartState->isPrecedenceRule) {
                            continue;
                        }

                        ATNState *maybeLoopEndState = state.transition(state.getNumberOfTransitions() - 1)->target;
                        if (!(dynamic_cast<LoopEndState*>(maybeLoopEndState) != nullptr)) {
                            continue;
                        }

                        if (maybeLoopEndState->epsilonOnlyTransitions && dynamic_cast<RuleStopState*>(maybeLoopEndState->transition(0)->target) != nullptr) {
                            this->pushRecursionContextStates->set(state.stateNumber);
                        }
                    }

                    // get atn simulator that knows how to do predictions
                    setInterpreter(new /*org::antlr::v4::runtime->atn->*/ParserATNSimulator(this, atn, decisionToDFA, sharedContextCache));
                }
#endif
                org::antlr::v4::runtime::atn::ATN *ParserInterpreter::getATN() {
                    return atn;
                }

                std::wstring *ParserInterpreter::getTokenNames() {
                    return tokenNames;
                }

                std::wstring *ParserInterpreter::getRuleNames() {
                    return ruleNames;
                }

                std::wstring ParserInterpreter::getGrammarFileName() {
                    return grammarFileName;
                }

                org::antlr::v4::runtime::ParserRuleContext *ParserInterpreter::parse(int startRuleIndex) {
                    RuleStartState *startRuleStartState = atn->ruleToStartState[startRuleIndex];

                    InterpreterRuleContext *rootContext = new InterpreterRuleContext(nullptr, org::antlr::v4::runtime->atn->ATNState::INVALID_STATE_NUMBER, startRuleIndex);
                    if (startRuleStartState->isPrecedenceRule) {
                        enterRecursionRule(rootContext, startRuleStartState->stateNumber, startRuleIndex, 0);
                    } else {
                        enterRule(rootContext, startRuleStartState->stateNumber, startRuleIndex);
                    }

                    while (true) {
                        ATNState *p = getATNState();
                        switch (p->getStateType()) {
                        case org::antlr::v4::runtime->atn->ATNState::RULE_STOP :
                            // pop; return from rule
                            if (_ctx->isEmpty()) {
                                exitRule();
                                return rootContext;
                            }

                            visitRuleStopState(p);
                            break;

                        default :
                            visitState(p);
                            break;
                        }
                    }
                }

                void ParserInterpreter::enterRecursionRule(ParserRuleContext *localctx, int state, int ruleIndex, int precedence) {
                    _parentContextStack->push(new Pair<ParserRuleContext*, int>(_ctx, localctx->invokingState));
                    Parser::enterRecursionRule(localctx, state, ruleIndex, precedence);
                }

                org::antlr::v4::runtime::atn::ATNState *ParserInterpreter::getATNState() {
                    return atn->states[getState()];
                }

                void ParserInterpreter::visitState(ATNState *p) {
                    int edge;
                    if (p->getNumberOfTransitions() > 1) {
                        edge = getInterpreter()->adaptivePredict(_input, (static_cast<DecisionState*>(p))->decision, _ctx);
                    } else {
                        edge = 1;
                    }

                    Transition *transition = p->transition(edge - 1);
                    switch (transition->getSerializationType()) {
                    case org::antlr::v4::runtime->atn->Transition::EPSILON:
                        if (pushRecursionContextStates->get(p->stateNumber) && !(dynamic_cast<LoopEndState*>(transition->target) != nullptr)) {
                            InterpreterRuleContext *ctx = new InterpreterRuleContext(_parentContextStack->peek()->a, _parentContextStack->peek()->b, _ctx->getRuleIndex());
                            pushNewRecursionContext(ctx, atn->ruleToStartState[p->ruleIndex]->stateNumber, _ctx->getRuleIndex());
                        }
                        break;

                    case org::antlr::v4::runtime->atn->Transition::ATOM:
                        match((static_cast<AtomTransition*>(transition))->label_Renamed);
                        break;

                    case org::antlr::v4::runtime->atn->Transition::RANGE:
                    case org::antlr::v4::runtime->atn->Transition::SET:
                    case org::antlr::v4::runtime->atn->Transition::NOT_SET:
                        if (!transition->matches(_input->LA(1), Token::MIN_USER_TOKEN_TYPE, 65535)) {
                            _errHandler->recoverInline(this);
                        }
                        matchWildcard();
                        break;

                    case org::antlr::v4::runtime->atn->Transition::WILDCARD:
                        matchWildcard();
                        break;

                    case org::antlr::v4::runtime->atn->Transition::RULE:
                        RuleStartState *ruleStartState = static_cast<RuleStartState*>(transition->target);
                        int ruleIndex = ruleStartState->ruleIndex;
                        InterpreterRuleContext *ctx = new InterpreterRuleContext(_ctx, p->stateNumber, ruleIndex);
                        if (ruleStartState->isPrecedenceRule) {
                            enterRecursionRule(ctx, ruleStartState->stateNumber, ruleIndex, (static_cast<RuleTransition*>(transition))->precedence);
                        } else {
                            enterRule(ctx, transition->target->stateNumber, ruleIndex);
                        }
                        break;

                    case org::antlr::v4::runtime->atn->Transition::PREDICATE:
                        PredicateTransition *predicateTransition = static_cast<PredicateTransition*>(transition);
                        if (!sempred(_ctx, predicateTransition->ruleIndex, predicateTransition->predIndex)) {
                            throw FailedPredicateException(this);
                        }

                        break;

                    case org::antlr::v4::runtime->atn->Transition::ACTION:
                        ActionTransition *actionTransition = static_cast<ActionTransition*>(transition);
                        action(_ctx, actionTransition->ruleIndex, actionTransition->actionIndex);
                        break;

                    case org::antlr::v4::runtime->atn->Transition::PRECEDENCE:
                        if (!precpred(_ctx, (static_cast<PrecedencePredicateTransition*>(transition))->precedence)) {
                            throw FailedPredicateException(this, std::wstring::format(L"precpred(_ctx, %d)", (static_cast<PrecedencePredicateTransition*>(transition))->precedence));
                        }
                        break;

                    default:
                        throw UnsupportedOperationException(L"Unrecognized ATN transition type.");
                    }

                    setState(transition->target->stateNumber);
                }

                void ParserInterpreter::visitRuleStopState(ATNState *p) {
                    RuleStartState *ruleStartState = atn->ruleToStartState[p->ruleIndex];
                    if (ruleStartState->isPrecedenceRule) {
                        Pair<ParserRuleContext*, int> *parentContext = _parentContextStack->pop();
                        unrollRecursionContexts(parentContext->a);
                        setState(parentContext->b);
                    } else {
                        exitRule();
                    }

                    RuleTransition *ruleTransition = static_cast<RuleTransition*>(atn->states[getState()]->transition(0));
                    setState(ruleTransition->followState->stateNumber);
                }
            }
        }
    }
}

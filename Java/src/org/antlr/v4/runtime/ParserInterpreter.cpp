#include "ParserInterpreter.h"
#include "Java/src/org/antlr/v4/runtime/atn/StarLoopEntryState.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleStartState.h"
#include "Java/src/org/antlr/v4/runtime/atn/LoopEndState.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleStopState.h"
#include "Java/src/org/antlr/v4/runtime/atn/ParserATNSimulator.h"
#include "Java/src/org/antlr/v4/runtime/InterpreterRuleContext.h"
#include "Java/src/org/antlr/v4/runtime/atn/DecisionState.h"
#include "Java/src/org/antlr/v4/runtime/atn/Transition.h"
#include "Java/src/org/antlr/v4/runtime/atn/AtomTransition.h"
#include "Java/src/org/antlr/v4/runtime/Token.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/PredicateTransition.h"
#include "Java/src/org/antlr/v4/runtime/FailedPredicateException.h"
#include "Java/src/org/antlr/v4/runtime/atn/ActionTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/PrecedencePredicateTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::ATN;
                using org::antlr::v4::runtime::atn::ATNState;
                using org::antlr::v4::runtime::atn::ActionTransition;
                using org::antlr::v4::runtime::atn::AtomTransition;
                using org::antlr::v4::runtime::atn::DecisionState;
                using org::antlr::v4::runtime::atn::LoopEndState;
                using org::antlr::v4::runtime::atn::ParserATNSimulator;
                using org::antlr::v4::runtime::atn::PrecedencePredicateTransition;
                using org::antlr::v4::runtime::atn::PredicateTransition;
                using org::antlr::v4::runtime::atn::PredictionContextCache;
                using org::antlr::v4::runtime::atn::RuleStartState;
                using org::antlr::v4::runtime::atn::RuleStopState;
                using org::antlr::v4::runtime::atn::RuleTransition;
                using org::antlr::v4::runtime::atn::StarLoopEntryState;
                using org::antlr::v4::runtime::atn::Transition;
                using org::antlr::v4::runtime::dfa::DFA;
                using org::antlr::v4::runtime::misc::Pair;

                ParserInterpreter::ParserInterpreter(const std::wstring &grammarFileName, Collection<std::wstring> *tokenNames, Collection<std::wstring> *ruleNames, ATN *atn, TokenStream *input) : Parser(input), grammarFileName(grammarFileName), atn(atn), pushRecursionContextStates(new BitSet(atn->states.size())), decisionToDFA(new DFA[atn->getNumberOfDecisions()]), tokenNames(tokenNames->toArray(new std::wstring[tokenNames->size()])), ruleNames(ruleNames->toArray(new std::wstring[ruleNames->size()])), sharedContextCache(new org::antlr::v4::runtime->atn->PredictionContextCache()), _parentContextStack(new java::util::ArrayDeque<org::antlr::v4::runtime::misc::Pair<ParserRuleContext, Integer>>()) {
                    for (int i = 0; i < decisionToDFA->length; i++) {
                        decisionToDFA[i] = new DFA(atn->getDecisionState(i), i);
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
                    setInterpreter(new org::antlr::v4::runtime->atn->ParserATNSimulator(this, atn, decisionToDFA, sharedContextCache));
                }

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

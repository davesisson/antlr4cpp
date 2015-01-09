#include "Token.h"
#include "ATN.h"
#include "DFA.h"
#include "Parser.h"
#include "Exceptions.h"
#include "NoViableAltException.h"
#include "DoubleKeyMap.h"
#include "ATNState.h"
#include "TokenStream.h"
#include "DecisionState.h"
#include "ParserRuleContext.h"
#include "ParserATNSimulator.h"

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

                    ParserATNSimulator::ParserATNSimulator(ATN *atn, dfa::DFA decisionToDFA[], PredictionContextCache *sharedContextCache):
                    parser(nullptr) {
                    }

                    ParserATNSimulator::ParserATNSimulator(Parser *parser, ATN *atn, dfa::DFA decisionToDFA[], PredictionContextCache *sharedContextCache) : ATNSimulator(atn,sharedContextCache), parser(parser), decisionToDFA(decisionToDFA) {
                        InitializeInstanceFields();
                        //		DOTGenerator dot = new DOTGenerator(null);
                        //		System.out.println(dot.getDOT(atn.rules.get(0), parser.getRuleNames()));
                        //		System.out.println(dot.getDOT(atn.rules.get(1), parser.getRuleNames()));
                    }

                    void ParserATNSimulator::reset() {
                    }

                    int ParserATNSimulator::adaptivePredict(TokenStream *input, int decision, ParserRuleContext *outerContext) {
                        if (debug || debug_list_atn_decisions) {
                            std::wcout << std::wstring(L"adaptivePredict decision ") << decision << std::wstring(L" exec LA(1)==") << getLookaheadName(input) << std::wstring(L" line ") << input->LT(1)->getLine() << std::wstring(L":") << input->LT(1)->getCharPositionInLine() << std::endl;
                        }

                        _input = input;
                        _startIndex = input->index();
                        _outerContext = outerContext;
                        dfa::DFA dfa = decisionToDFA[decision];

                        int m = input->mark();
                        int index = input->index();

                        // Now we are certain to have a specific decision's DFA
                        // But, do we still need an initial state?
                        try {
                            if (dfa.s0 == nullptr) {
                                if (outerContext == nullptr) {
                                    outerContext = ParserRuleContext::EMPTY;
                                }
                                if (debug || debug_list_atn_decisions) {

                                    std::wcout << std::wstring(L"predictATN decision ") << dfa.decision << std::wstring(L" exec LA(1)==") << getLookaheadName(input) << std::wstring(L", outerContext=") << outerContext->toString(parser) << std::endl;
                                }
                                bool fullCtx = false;
                                ATNConfigSet *s0_closure = computeStartState(dynamic_cast<ATNState*>(dfa.atnStartState),
                                                                             ParserRuleContext::EMPTY, fullCtx);
                                dfa.s0 = addDFAState(&dfa, new dfa::DFAState(s0_closure));
                            }

                            // We can start with an existing DFA.
                            int alt = execATN(&dfa, dfa.s0, input, index, outerContext);
                            if (debug) {

                                std::wcout << "DFA after predictATN: " << dfa.toString(parser->getTokenNames()) << std::endl;
                            }
                            return alt;
                        }
                        catch (void*) {
#ifdef TODO
                            C++ requires a catch block
#endif
                        }
#ifdef TODO
                        finally {
#endif
                            //JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                            delete mergeCache; // wack cache after each prediction
                            input->seek(index);
                            input->release(m);
#ifdef TODO
                        }
#endif
                    }

                    int ParserATNSimulator::execATN(dfa::DFA *dfa, dfa::DFAState *s0, TokenStream *input, int startIndex, ParserRuleContext *outerContext) {
                        if (debug || debug_list_atn_decisions) {
                            std::wcout << std::wstring(L"execATN decision ") << dfa->decision << std::wstring(L" exec LA(1)==") << getLookaheadName(input) << std::wstring(L" line ") << input->LT(1)->getLine() << std::wstring(L":") << input->LT(1)->getCharPositionInLine() << std::endl;
                        }

                        dfa::DFAState *previousD = s0;

                        if (debug) {
                            std::wcout << std::wstring(L"s0 = ") << s0 << std::endl;
                        }

                        int t = input->LA(1);

                        while (true) { // while more work
                            dfa::DFAState *D = getExistingTargetState(previousD, t);
                            if (D == nullptr) {
                                D = computeTargetState(dfa, previousD, t);
                            }

                            if (D == ERROR) {
                                // if any configs in previous dipped into outer context, that
                                // means that input up to t actually finished entry rule
                                // at least for SLL decision. Full LL doesn't dip into outer
                                // so don't need special case.
                                // We will get an error no matter what so delay until after
                                // decision; better error message. Also, no reachable target
                                // ATN states in SLL implies LL will also get nowhere.
                                // If conflict in states that dip out, choose min since we
                                // will get error no matter what.
                                int alt = getAltThatFinishedDecisionEntryRule(previousD->configs);
                                if (alt != ATN::INVALID_ALT_NUMBER) {
                                    // return w/o altering DFA
                                    return alt;
                                }

                                throw noViableAlt(input, outerContext, previousD->configs, startIndex);
                            }

                            if (D->requiresFullContext && mode != PredictionMode::SLL) {
                                // IF PREDS, MIGHT RESOLVE TO SINGLE ALT => SLL (or syntax error)
                                std::bitset<BITSET_SIZE> *conflictingAlts = nullptr;
                                if (D->predicates.size() != 0) {
                                    if (debug) {
                                        std::wcout << std::wstring(L"DFA state has preds in DFA sim LL failover") << std::endl;
                                    }
                                    int conflictIndex = input->index();
                                    if (conflictIndex != startIndex) {
                                        input->seek(startIndex);
                                    }

                                    conflictingAlts = evalSemanticContext(D->predicates, outerContext, true);
                                    if (conflictingAlts->count() == 1) {
                                        if (debug) {
                                            std::wcout << std::wstring(L"Full LL avoided") << std::endl;
                                        }
                                        return conflictingAlts->nextSetBit(0);
                                    }

                                    if (conflictIndex != startIndex) {
                                        // restore the index so reporting the fallback to full
                                        // context occurs with the index at the correct spot
                                        input->seek(conflictIndex);
                                    }
                                }

                                if (dfa_debug) {
                                    std::wcout << std::wstring(L"ctx sensitive state ") << outerContext << std::wstring(L" in ") << D << std::endl;
                                }
                                bool fullCtx = true;
                                ATNConfigSet *s0_closure = computeStartState(dfa->atnStartState, outerContext, fullCtx);
                                reportAttemptingFullContext(dfa, conflictingAlts, D->configs, startIndex, input->index());
                                int alt = execATNWithFullContext(dfa, D, s0_closure, input, startIndex, outerContext);
                                return alt;
                            }

                            if (D->isAcceptState) {
                                if (D->predicates.size() == 0) {
                                    return D->prediction;
                                }

                                int stopIndex = input->index();
                                input->seek(startIndex);
                                std::bitset<BITSET_SIZE> *alts = evalSemanticContext(D->predicates, outerContext, true);
                                switch (alts->count()) {
                                case 0:
                                    throw noViableAlt(input, outerContext, D->configs, startIndex);

                                case 1:
                                    return alts->nextSetBit(0);

                                default:
                                    // report ambiguity after predicate evaluation to make sure the correct
                                    // set of ambig alts is reported.
                                    reportAmbiguity(dfa, D, startIndex, stopIndex, false, alts, D->configs);
                                    return alts->nextSetBit(0);
                                }
                            }

                            previousD = D;

                            if (t != IntStream::_EOF) {
                                input->consume();
                                t = input->LA(1);
                            }
                        }
                    }

                    dfa::DFAState *ParserATNSimulator::getExistingTargetState(dfa::DFAState *previousD, int t) {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: org.antlr.v4.runtime.dfa.DFAState[] edges = previousD.edges;
                        std::vector<dfa::DFAState *>edges = previousD->edges;
                        if (edges.size() == 0 || t + 1 < 0 || t + 1 >= edges.size()) {
                            return nullptr;
                        }

                        return edges[t + 1];
                    }

                    dfa::DFAState *ParserATNSimulator::computeTargetState(dfa::DFA *dfa, dfa::DFAState *previousD, int t) {
                        ATNConfigSet *reach = computeReachSet(previousD->configs, t, false);
                        if (reach == nullptr) {
                            addDFAEdge(dfa, previousD, t, ERROR);
                            return ERROR;
                        }

                        // create new target state; we'll add to DFA after it's complete
                        dfa::DFAState *D = new dfa::DFAState(reach);

                        int predictedAlt = getUniqueAlt(reach);

                        if (debug) {
#ifdef TODO
                            std::vector<std::bitset<BITSET_SIZE>*> *altSubSets = PredictionMode::getConflictingAltSubsets(reach);
                            std::wcout << std::wstring(L"SLL altSubSets=") << altSubSets << std::wstring(L", configs=") << reach << std::wstring(L", predict=") << predictedAlt << std::wstring(L", allSubsetsConflict=") << PredictionMode::allSubsetsConflict(altSubSets) << std::wstring(L", conflictingAlts=") << getConflictingAlts(reach) << std::endl;
#endif
                        }

                        if (predictedAlt != ATN::INVALID_ALT_NUMBER) {
                            // NO CONFLICT, UNIQUELY PREDICTED ALT
                            D->isAcceptState = true;
                            D->configs->uniqueAlt = predictedAlt;
                            D->prediction = predictedAlt;
                        } else if (hasSLLConflictTerminatingPrediction(&mode, reach)) {
                            // MORE THAN ONE VIABLE ALTERNATIVE
                            D->configs->conflictingAlts = getConflictingAlts(reach);
                            D->requiresFullContext = true;
                            // in SLL-only mode, we will stop at this state and return the minimum alt
                            D->isAcceptState = true;
                            D->prediction = D->configs->conflictingAlts->nextSetBit(0);
                        }

                        if (D->isAcceptState && D->configs->hasSemanticContext) {
                            predicateDFAState(D, atn->getDecisionState(dfa->decision));
                            if (D->predicates != nullptr) {
                                D->prediction = ATN::INVALID_ALT_NUMBER;
                            }
                        }

                        // all adds to dfa are done after we've created full D state
                        D = addDFAEdge(dfa, previousD, t, D);
                        return D;
                    }

                    void ParserATNSimulator::predicateDFAState(dfa::DFAState *dfaState, DecisionState *decisionState) {
                        // We need to test all predicates, even in DFA states that
                        // uniquely predict alternative.
                        int nalts = decisionState->getNumberOfTransitions();
                        // Update DFA so reach becomes accept state with (predicate,alt)
                        // pairs if preds found for conflicting alts
                        BitSet *altsToCollectPredsFrom = getConflictingAltsOrUniqueAlt(dfaState->configs);
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: SemanticContext[] altToPred = getPredsForAmbigAlts(altsToCollectPredsFrom, dfaState.configs, nalts);
                        SemanticContext *altToPred = getPredsForAmbigAlts(altsToCollectPredsFrom, dfaState->configs, nalts);
                        if (altToPred != nullptr) {
                            dfaState->predicates = getPredicatePredictions(altsToCollectPredsFrom, altToPred);
                            dfaState->prediction = ATN::INVALID_ALT_NUMBER; // make sure we use preds
                        } else {
                            // There are preds in configs but they might go away
                            // when OR'd together like {p}? || NONE == NONE. If neither
                            // alt has preds, resolve to min alt
                            dfaState->prediction = altsToCollectPredsFrom->nextSetBit(0);
                        }
                    }

                    int ParserATNSimulator::execATNWithFullContext(dfa::DFA *dfa, dfa::DFAState *D, ATNConfigSet *s0, TokenStream *input, int startIndex, ParserRuleContext *outerContext) {
                        if (debug || debug_list_atn_decisions) {
                            std::cout << "execATNWithFullContext " << s0 << std::endl;
                        }
                        bool fullCtx = true;
                        bool foundExactAmbig = false;
                        ATNConfigSet *reach = nullptr;
                        ATNConfigSet *previous = s0;
                        input->seek(startIndex);
                        int t = input->LA(1);
                        int predictedAlt;
                        while (true) { // while more work
                                        //			System.out.println("LL REACH "+getLookaheadName(input)+
                                        //							   " from configs.size="+previous.size()+
                                        //							   " line "+input.LT(1).getLine()+":"+input.LT(1).getCharPositionInLine());
                            reach = computeReachSet(previous, t, fullCtx);
                            if (reach == nullptr) {
                                // if any configs in previous dipped into outer context, that
                                // means that input up to t actually finished entry rule
                                // at least for LL decision. Full LL doesn't dip into outer
                                // so don't need special case.
                                // We will get an error no matter what so delay until after
                                // decision; better error message. Also, no reachable target
                                // ATN states in SLL implies LL will also get nowhere.
                                // If conflict in states that dip out, choose min since we
                                // will get error no matter what.
                                int alt = getAltThatFinishedDecisionEntryRule(previous);
                                if (alt != ATN::INVALID_ALT_NUMBER) {
                                    return alt;
                                }
                                throw noViableAlt(input, outerContext, previous, startIndex);
                            }

                            Collection<BitSet*> *altSubSets = PredictionMode::getConflictingAltSubsets(reach);
                            if (debug) {
                                std::cout << std::wstring(L"LL altSubSets=") << altSubSets << std::wstring(L", predict=") << PredictionMode::getUniqueAlt(altSubSets) << std::wstring(L", resolvesToJustOneViableAlt=") << PredictionMode::resolvesToJustOneViableAlt(altSubSets) << std::endl;
                            }

                                        //			System.out.println("altSubSets: "+altSubSets);
                            reach->uniqueAlt = getUniqueAlt(reach);
                            // unique prediction?
                            if (reach->uniqueAlt != ATN::INVALID_ALT_NUMBER) {
                                predictedAlt = reach->uniqueAlt;
                                break;
                            }
                            if (mode != PredictionMode::LL_EXACT_AMBIG_DETECTION) {
                                predictedAlt = PredictionMode::resolvesToJustOneViableAlt(altSubSets);
                                if (predictedAlt != ATN::INVALID_ALT_NUMBER) {
                                    break;
                                }
                            } else {
                                // In exact ambiguity mode, we never try to terminate early.
                                // Just keeps scarfing until we know what the conflict is
                                if (PredictionMode::allSubsetsConflict(altSubSets) && PredictionMode::allSubsetsEqual(altSubSets)) {
                                    foundExactAmbig = true;
                                    predictedAlt = PredictionMode::getSingleViableAlt(altSubSets);
                                    break;
                                }
                                // else there are multiple non-conflicting subsets or
                                // we're not sure what the ambiguity is yet.
                                // So, keep going.
                            }

                            previous = reach;
                            if (t != IntStream::_EOF) {
                                input->consume();
                                t = input->LA(1);
                            }
                        }

                        // If the configuration set uniquely predicts an alternative,
                        // without conflict, then we know that it's a full LL decision
                        // not SLL.
                        if (reach->uniqueAlt != ATN::INVALID_ALT_NUMBER) {
                            reportContextSensitivity(dfa, predictedAlt, reach, startIndex, input->index());
                            return predictedAlt;
                        }

                        // We do not check predicates here because we have checked them
                        // on-the-fly when doing full context prediction.

                        /*
                        In non-exact ambiguity detection mode, we might	actually be able to
                        detect an exact ambiguity, but I'm not going to spend the cycles
                        needed to check. We only emit ambiguity warnings in exact ambiguity
                        mode.
                                        
                        For example, we might know that we have conflicting configurations.
                        But, that does not mean that there is no way forward without a
                        conflict. It's possible to have nonconflicting alt subsets as in:
                                        
                           LL altSubSets=[{1, 2}, {1, 2}, {1}, {1, 2}]
                                        
                        from
                                        
                           [(17,1,[5 $]), (13,1,[5 10 $]), (21,1,[5 10 $]), (11,1,[$]),
                        	(13,2,[5 10 $]), (21,2,[5 10 $]), (11,2,[$])]
                                        
                        In this case, (17,1,[5 $]) indicates there is some next sequence that
                        would resolve this without conflict to alternative 1. Any other viable
                        next sequence, however, is associated with a conflict.  We stop
                        looking for input because no amount of further lookahead will alter
                        the fact that we should predict alternative 1.  We just can't say for
                        sure that there is an ambiguity without looking further.
                        */
                        reportAmbiguity(dfa, D, startIndex, input->index(), foundExactAmbig, nullptr, reach);

                        return predictedAlt;
                    }

                    org::antlr::v4::runtime::atn::ATNConfigSet *ParserATNSimulator::computeReachSet(ATNConfigSet *closure, int t, bool fullCtx) {
                        if (debug) {
                            std::cout << std::wstring(L"in computeReachSet, starting closure: ") << closure << std::endl;
                        }

                        if (mergeCache == nullptr) {
                            mergeCache = new DoubleKeyMap<PredictionContext*, PredictionContext*, PredictionContext*>();
                        }

                        ATNConfigSet *intermediate = new ATNConfigSet(fullCtx);

                        /* Configurations already in a rule stop state indicate reaching the end
                         * of the decision rule (local context) or end of the start rule (full
                         * context). Once reached, these configurations are never updated by a
                         * closure operation, so they are handled separately for the performance
                         * advantage of having a smaller intermediate set when calling closure.
                         *
                         * For full-context reach operations, separate handling is required to
                         * ensure that the alternative matching the longest overall sequence is
                         * chosen when multiple such configurations can match the input.
                         */
                        std::vector<ATNConfig*> skippedStopStates;

                        // First figure out where we can reach on input t
                        for (auto c : closure) {
                            if (debug) {
                                std::wcout << std::wstring(L"testing ") << getTokenName(t) << std::wstring(L" at ") << c->toString() << std::endl;
                            }

                            if (dynamic_cast<RuleStopState*>(c->state) != nullptr) {
                                assert(c->context->isEmpty());
                                if (fullCtx || t == IntStream::_EOF) {
                                    if (skippedStopStates.empty()) {
                                        skippedStopStates = std::vector<ATNConfig*>();
                                    }

                                    skippedStopStates.push_back(c);
                                }

                                continue;
                            }

                            int n = c->state->getNumberOfTransitions();
                            for (int ti = 0; ti < n; ti++) { // for each transition
                                Transition *trans = c->state->transition(ti);
                                ATNState *target = getReachableTarget(trans, t);
                                if (target != nullptr) {
                                    intermediate->add(new ATNConfig(c, target), mergeCache);
                                }
                            }
                        }

                        // Now figure out where the reach operation can take us...

                        ATNConfigSet *reach = nullptr;

                        /* This block optimizes the reach operation for intermediate sets which
                         * trivially indicate a termination state for the overall
                         * adaptivePredict operation.
                         *
                         * The conditions assume that intermediate
                         * contains all configurations relevant to the reach set, but this
                         * condition is not true when one or more configurations have been
                         * withheld in skippedStopStates.
                         */
                        if (skippedStopStates.empty()) {
                            if (intermediate->size() == 1) {
                                // Don't pursue the closure if there is just one state.
                                // It can only have one alternative; just add to result
                                // Also don't pursue the closure if there is unique alternative
                                // among the configurations.
                                reach = intermediate;
                            } else if (getUniqueAlt(intermediate) != ATN::INVALID_ALT_NUMBER) {
                                // Also don't pursue the closure if there is unique alternative
                                // among the configurations.
                                reach = intermediate;
                            }
                        }

                        /* If the reach set could not be trivially determined, perform a closure
                         * operation on the intermediate set to compute its initial value.
                         */
                        if (reach == nullptr) {
                            reach = new ATNConfigSet(fullCtx);
                            Set<ATNConfig*> *closureBusy = std::set<ATNConfig*>();
                            for (auto c : intermediate) {
                                closure(c, reach, closureBusy, false, fullCtx);
                            }
                        }

                        if (t == IntStream::_EOF) {
                            /* After consuming EOF no additional input is possible, so we are
                             * only interested in configurations which reached the end of the
                             * decision rule (local context) or end of the start rule (full
                             * context). Update reach to contain only these configurations. This
                             * handles both explicit EOF transitions in the grammar and implicit
                             * EOF transitions following the end of the decision or start rule.
                             *
                             * When reach==intermediate, no closure operation was performed. In
                             * this case, removeAllConfigsNotInRuleStopState needs to check for
                             * reachable rule stop states as well as configurations already in
                             * a rule stop state.
                             *
                             * This is handled before the configurations in skippedStopStates,
                             * because any configurations potentially added from that list are
                             * already guaranteed to meet this condition whether or not it's
                             * required.
                             */
                            reach = removeAllConfigsNotInRuleStopState(reach, reach == intermediate);
                        }

                        /* If skippedStopStates is not null, then it contains at least one
                         * configuration. For full-context reach operations, these
                         * configurations reached the end of the start rule, in which case we
                         * only add them back to reach if no configuration during the current
                         * closure operation reached such a state. This ensures adaptivePredict
                         * chooses an alternative matching the longest overall sequence when
                         * multiple alternatives are viable.
                         */
                        if (skippedStopStates.size() > 0 && (!fullCtx || !PredictionMode::hasConfigInRuleStopState(reach))) {
                            assert(!skippedStopStates.empty());
                            for (auto c : skippedStopStates) {
                                reach->add(c, mergeCache);
                            }
                        }

                        if (reach->isEmpty()) {
                            return nullptr;
                        }
                        return reach;
                    }

                    org::antlr::v4::runtime::atn::ATNConfigSet *ParserATNSimulator::removeAllConfigsNotInRuleStopState(ATNConfigSet *configs, bool lookToEndOfRule) {
                        if (PredictionMode::allConfigsInRuleStopStates(configs)) {
                            return configs;
                        }

                        ATNConfigSet *result = new ATNConfigSet(configs->fullCtx);
                        for (auto config : configs) {
                            if (dynamic_cast<RuleStopState*>(config->state) != nullptr) {
                                result->add(config, mergeCache);
                                continue;
                            }

                            if (lookToEndOfRule && config->state->onlyHasEpsilonTransitions()) {
                                IntervalSet *nextTokens = atn->nextTokens(config->state);
                                if (nextTokens->contains(Token::EPSILON)) {
                                    ATNState *endOfRuleState = atn->ruleToStopState[config->state->ruleIndex];
                                    result->add(new ATNConfig(config, endOfRuleState), mergeCache);
                                }
                            }
                        }

                        return result;
                    }

                    org::antlr::v4::runtime::atn::ATNConfigSet *ParserATNSimulator::computeStartState(ATNState *p, RuleContext *ctx, bool fullCtx) {
                        // always at least the implicit call to start rule
                        PredictionContext *initialContext = PredictionContext::fromRuleContext(atn, ctx);
                        ATNConfigSet *configs = new ATNConfigSet(fullCtx);

                        for (int i = 0; i < p->getNumberOfTransitions(); i++) {
                            ATNState *target = p->transition(i)->target;
                            ATNConfig *c = new ATNConfig(target, i + 1, initialContext);
                            Set<ATNConfig*> *closureBusy = std::set<ATNConfig*>();
                            closure(c, configs, closureBusy, true, fullCtx);
                        }

                        return configs;
                    }

                    org::antlr::v4::runtime::atn::ATNState *ParserATNSimulator::getReachableTarget(Transition *trans, int ttype) {
                        if (trans->matches(ttype, 0, atn->maxTokenType)) {
                            return trans->target;
                        }

                        return nullptr;
                    }

                    SemanticContext *ParserATNSimulator::getPredsForAmbigAlts(BitSet *ambigAlts, ATNConfigSet *configs, int nalts) {
                        // REACH=[1|1|[]|0:0, 1|2|[]|0:1]
                        /* altToPred starts as an array of all null contexts. The entry at index i
                         * corresponds to alternative i. altToPred[i] may have one of three values:
                         *   1. null: no ATNConfig c is found such that c.alt==i
                         *   2. SemanticContext.NONE: At least one ATNConfig c exists such that
                         *      c.alt==i and c.semanticContext==SemanticContext.NONE. In other words,
                         *      alt i has at least one unpredicated config.
                         *   3. Non-NONE Semantic Context: There exists at least one, and for all
                         *      ATNConfig c such that c.alt==i, c.semanticContext!=SemanticContext.NONE.
                         *
                         * From this, it is clear that NONE||anything==NONE.
                         */
                        SemanticContext altToPred[nalts + 1];
                        for (auto c : configs) {
                            if (ambigAlts->get(c->alt)) {
                                altToPred[c->alt] = SemanticContext::or(altToPred[c->alt], c->semanticContext);
                            }
                        }

                        int nPredAlts = 0;
                        for (int i = 1; i <= nalts; i++) {
                            if (altToPred[i] == nullptr) {
                                altToPred[i] = SemanticContext::NONE;
                            } else if (altToPred[i] != SemanticContext::NONE) {
                                nPredAlts++;
                            }
                        }

                                        //		// Optimize away p||p and p&&p TODO: optimize() was a no-op
                                        //		for (int i = 0; i < altToPred.length; i++) {
                                        //			altToPred[i] = altToPred[i].optimize();
                                        //		}

                        // nonambig alts are null in altToPred
                        if (nPredAlts == 0) {
                            altToPred = nullptr;
                        }
                        if (debug) {
                            std::wcout << std::wstring(L"getPredsForAmbigAlts result ") << Arrays->toString(altToPred) << std::endl;
                        }
                        return altToPred;
                    }

                    DFAState::PredPrediction *ParserATNSimulator::getPredicatePredictions(BitSet *ambigAlts, SemanticContext altToPred[]) {
                        std::vector<DFAState::PredPrediction*> pairs = std::vector<DFAState::PredPrediction*>();
                        bool containsPredicate = false;
                        for (int i = 1; i < sizeof(altToPred) / sizeof(altToPred[0]); i++) {
                            SemanticContext *pred = altToPred[i];

                            // unpredicated is indicated by SemanticContext.NONE
                            assert(pred != nullptr);

                            if (ambigAlts != nullptr && ambigAlts->get(i)) {
                                pairs.push_back(new DFAState::PredPrediction(pred, i));
                            }
                            if (pred != SemanticContext::NONE) {
                                containsPredicate = true;
                            }
                        }

                        if (!containsPredicate) {
                            return nullptr;
                        }

                                        //		System.out.println(Arrays.toString(altToPred)+"->"+pairs);
                        return pairs.toArray(new DFAState::PredPrediction[pairs.size()]);
                    }

                    int ParserATNSimulator::getAltThatFinishedDecisionEntryRule(ATNConfigSet *configs) {
                        IntervalSet *alts = new IntervalSet();
                        for (auto c : configs) {
                            if (c->reachesIntoOuterContext > 0 || (dynamic_cast<RuleStopState*>(c->state) != nullptr && c->context->hasEmptyPath())) {
                                alts->add(c->alt);
                            }
                        }
                        if (alts->size() == 0) {
                            return ATN::INVALID_ALT_NUMBER;
                        }
                        return alts->getMinElement();
                    }

                    BitSet *ParserATNSimulator::evalSemanticContext(DFAState::PredPrediction predPredictions[], ParserRuleContext *outerContext, bool complete) {
                        BitSet *predictions = new BitSet();
                        for (auto pair : predPredictions) {
                            if (pair->pred == SemanticContext::NONE) {
                                predictions->set(pair->alt);
                                if (!complete) {
                                    break;
                                }
                                continue;
                            }

                            bool predicateEvaluationResult = pair->pred->eval(parser, outerContext);
                            if (debug || dfa_debug) {
                                std::cout << std::wstring(L"eval pred ") << pair << std::wstring(L"=") << predicateEvaluationResult << std::endl;
                            }

                            if (predicateEvaluationResult) {
                                if (debug || dfa_debug) {
                                    std::cout << std::wstring(L"PREDICT ") << pair->alt << std::endl;
                                }
                                predictions->set(pair->alt);
                                if (!complete) {
                                    break;
                                }
                            }
                        }

                        return predictions;
                    }

                    void ParserATNSimulator::closure(ATNConfig *config, ATNConfigSet *configs, Set<ATNConfig*> *closureBusy, bool collectPredicates, bool fullCtx) {
                        const int initialDepth = 0;
                        closureCheckingStopState(config, configs, closureBusy, collectPredicates, fullCtx, initialDepth);
                        assert(!fullCtx || !configs->dipsIntoOuterContext);
                    }

                    void ParserATNSimulator::closureCheckingStopState(ATNConfig *config, ATNConfigSet *configs, Set<ATNConfig*> *closureBusy, bool collectPredicates, bool fullCtx, int depth) {
                        if (debug) {
                            std::cout << std::wstring(L"closure(") << config->toString(parser,true) << std::wstring(L")") << std::endl;
                        }

                        if (dynamic_cast<RuleStopState*>(config->state) != nullptr) {
                            // We hit rule end. If we have context info, use it
                            // run thru all possible stack tops in ctx
                            if (!config->context->isEmpty()) {
                                for (int i = 0; i < config->context->size(); i++) {
                                    if (config->context->getReturnState(i) == PredictionContext::EMPTY_RETURN_STATE) {
                                        if (fullCtx) {
                                            configs->add(new ATNConfig(config, config->state, PredictionContext::EMPTY), mergeCache);
                                            continue;
                                        } else {
                                            // we have no context info, just chase follow links (if greedy)
                                            if (debug) {
                                                std::cout << std::wstring(L"FALLING off rule ") << getRuleName(config->state->ruleIndex) << std::endl;
                                            }
                                            closure_(config, configs, closureBusy, collectPredicates, fullCtx, depth);
                                        }
                                        continue;
                                    }
                                    ATNState *returnState = atn->states[config->context->getReturnState(i)];
                                    PredictionContext *newContext = config->context->getParent(i); // "pop" return state
                                    ATNConfig *c = new ATNConfig(returnState, config->alt, newContext, config->semanticContext);
                                    // While we have context to pop back from, we may have
                                    // gotten that context AFTER having falling off a rule.
                                    // Make sure we track that we are now out of context.
                                    c->reachesIntoOuterContext = config->reachesIntoOuterContext;
                                    assert(depth > int::MIN_VALUE);
                                    closureCheckingStopState(c, configs, closureBusy, collectPredicates, fullCtx, depth - 1);
                                }
                                return;
                            } else if (fullCtx) {
                                // reached end of start rule
                                configs->add(config, mergeCache);
                                return;
                            } else {
                                // else if we have no context info, just chase follow links (if greedy)
                                if (debug) {
                                    std::cout << std::wstring(L"FALLING off rule ") << getRuleName(config->state->ruleIndex) << std::endl;
                                }
                            }
                        }

                        closure_(config, configs, closureBusy, collectPredicates, fullCtx, depth);
                    }

                    void ParserATNSimulator::closure_(ATNConfig *config, ATNConfigSet *configs, Set<ATNConfig*> *closureBusy, bool collectPredicates, bool fullCtx, int depth) {
                        ATNState *p = config->state;
                        // optimization
                        if (!p->onlyHasEpsilonTransitions()) {
                            configs->add(config, mergeCache);
                                        //            if ( debug ) System.out.println("added config "+configs);
                        }

                        for (int i = 0; i < p->getNumberOfTransitions(); i++) {
                            Transition *t = p->transition(i);
                            bool continueCollecting = !(dynamic_cast<ActionTransition*>(t) != nullptr) && collectPredicates;
                            ATNConfig *c = getEpsilonTarget(config, t, continueCollecting, depth == 0, fullCtx);
                            if (c != nullptr) {
                                int newDepth = depth;
                                if (dynamic_cast<RuleStopState*>(config->state) != nullptr) {
                                    assert(!fullCtx);
                                    // target fell off end of rule; mark resulting c as having dipped into outer context
                                    // We can't get here if incoming config was rule stop and we had context
                                    // track how far we dip into outer context.  Might
                                    // come in handy and we avoid evaluating context dependent
                                    // preds if this is > 0.

                                    if (!closureBusy->add(c)) {
                                        // avoid infinite recursion for right-recursive rules
                                        continue;
                                    }

                                    c->reachesIntoOuterContext++;
                                    configs->dipsIntoOuterContext = true; // TODO: can remove? only care when we add to set per middle of this method
                                    assert(newDepth > int::MIN_VALUE);
                                    newDepth--;
                                    if (debug) {
                                        std::cout << std::wstring(L"dips into outer ctx: ") << c << std::endl;
                                    }
                                } else if (dynamic_cast<RuleTransition*>(t) != nullptr) {
                                    // latch when newDepth goes negative - once we step out of the entry context we can't return
                                    if (newDepth >= 0) {
                                        newDepth++;
                                    }
                                }

                                closureCheckingStopState(c, configs, closureBusy, continueCollecting, fullCtx, newDepth);
                            }
                        }
                    }

                    std::wstring ParserATNSimulator::getRuleName(int index) {
                        if (parser != nullptr && index >= 0) {
                            return parser->getRuleNames()[index];
                        }
                        return std::wstring(L"<rule ") + index + std::wstring(L">");
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ParserATNSimulator::getEpsilonTarget(ATNConfig *config, Transition *t, bool collectPredicates, bool inContext, bool fullCtx) {
                        switch (t->getSerializationType()) {
                        case Transition::RULE:
                            return ruleTransition(config, static_cast<RuleTransition*>(t));

                        case Transition::PRECEDENCE:
                            return precedenceTransition(config, static_cast<PrecedencePredicateTransition*>(t), collectPredicates, inContext, fullCtx);

                        case Transition::PREDICATE:
                            return predTransition(config, static_cast<PredicateTransition*>(t), collectPredicates, inContext, fullCtx);

                        case Transition::ACTION:
                            return actionTransition(config, static_cast<ActionTransition*>(t));

                        case Transition::EPSILON:
                            return new ATNConfig(config, t->target);

                        default:
                            return nullptr;
                        }
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ParserATNSimulator::actionTransition(ATNConfig *config, ActionTransition *t) {
                        if (debug) {
                            std::cout << std::wstring(L"ACTION edge ") << t->ruleIndex << std::wstring(L":") << t->actionIndex << std::endl;
                        }
                        return new ATNConfig(config, t->target);
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ParserATNSimulator::precedenceTransition(ATNConfig *config, PrecedencePredicateTransition *pt, bool collectPredicates, bool inContext, bool fullCtx) {
                        if (debug) {
                            std::cout << std::wstring(L"PRED (collectPredicates=") << collectPredicates << std::wstring(L") ") << pt->precedence << std::wstring(L">=_p") << std::wstring(L", ctx dependent=true") << std::endl;
                            if (parser != nullptr) {
                                std::cout << std::wstring(L"context surrounding pred is ") << parser->getRuleInvocationStack() << std::endl;
                            }
                        }

                        ATNConfig *c = nullptr;
                        if (collectPredicates && inContext) {
                            if (fullCtx) {
                                // In full context mode, we can evaluate predicates on-the-fly
                                // during closure, which dramatically reduces the size of
                                // the config sets. It also obviates the need to test predicates
                                // later during conflict resolution.
                                int currentPosition = _input->index();
                                _input->seek(_startIndex);
                                bool predSucceeds = pt->getPredicate()->eval(parser, _outerContext);
                                _input->seek(currentPosition);
                                if (predSucceeds) {
                                    c = new ATNConfig(config, pt->target); // no pred context
                                }
                            } else {
                                SemanticContext *newSemCtx = SemanticContext::and(config->semanticContext, pt->getPredicate());
                                c = new ATNConfig(config, pt->target, newSemCtx);
                            }
                        } else {
                            c = new ATNConfig(config, pt->target);
                        }

                        if (debug) {
                            std::cout << std::wstring(L"config from pred transition=") << c << std::endl;
                        }
                        return c;
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ParserATNSimulator::predTransition(ATNConfig *config, PredicateTransition *pt, bool collectPredicates, bool inContext, bool fullCtx) {
                        if (debug) {
                            std::cout << std::wstring(L"PRED (collectPredicates=") << collectPredicates << std::wstring(L") ") << pt->ruleIndex << std::wstring(L":") << pt->predIndex << std::wstring(L", ctx dependent=") << pt->isCtxDependent << std::endl;
                            if (parser != nullptr) {
                                std::cout << std::wstring(L"context surrounding pred is ") << parser->getRuleInvocationStack() << std::endl;
                            }
                        }

                        ATNConfig *c = nullptr;
                        if (collectPredicates && (!pt->isCtxDependent || (pt->isCtxDependent && inContext))) {
                            if (fullCtx) {
                                // In full context mode, we can evaluate predicates on-the-fly
                                // during closure, which dramatically reduces the size of
                                // the config sets. It also obviates the need to test predicates
                                // later during conflict resolution.
                                int currentPosition = _input->index();
                                _input->seek(_startIndex);
                                bool predSucceeds = pt->getPredicate()->eval(parser, _outerContext);
                                _input->seek(currentPosition);
                                if (predSucceeds) {
                                    c = new ATNConfig(config, pt->target); // no pred context
                                }
                            } else {
                                SemanticContext *newSemCtx = SemanticContext::and(config->semanticContext, pt->getPredicate());
                                c = new ATNConfig(config, pt->target, newSemCtx);
                            }
                        } else {
                            c = new ATNConfig(config, pt->target);
                        }

                        if (debug) {
                            std::cout << std::wstring(L"config from pred transition=") << c << std::endl;
                        }
                        return c;
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ParserATNSimulator::ruleTransition(ATNConfig *config, RuleTransition *t) {
                        if (debug) {
                            std::cout << std::wstring(L"CALL rule ") << getRuleName(t->target->ruleIndex) << std::wstring(L", ctx=") << config->context << std::endl;
                        }

                        ATNState *returnState = t->followState;
                        PredictionContext *newContext = SingletonPredictionContext::create(config->context, returnState->stateNumber);
                        return new ATNConfig(config, t->target, newContext);
                    }

                    BitSet *ParserATNSimulator::getConflictingAlts(ATNConfigSet *configs) {
                        Collection<BitSet*> *altsets = PredictionMode::getConflictingAltSubsets(configs);
                        return PredictionMode::getAlts(altsets);
                    }

                    BitSet *ParserATNSimulator::getConflictingAltsOrUniqueAlt(ATNConfigSet *configs) {
                        BitSet *conflictingAlts;
                        if (configs->uniqueAlt != ATN::INVALID_ALT_NUMBER) {
                            conflictingAlts = new BitSet();
                            conflictingAlts->set(configs->uniqueAlt);
                        } else {
                            conflictingAlts = configs->conflictingAlts;
                        }
                        return conflictingAlts;
                    }

                    std::wstring ParserATNSimulator::getTokenName(int t) {
                        if (t == Token::_EOF) {
                            return L"EOF";
                        }
                        if (parser != nullptr && parser->getTokenNames() != nullptr) {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: String[] tokensNames = parser.getTokenNames();
                            std::wstring *tokensNames = parser->getTokenNames();
                            if (t >= tokensNames->length) {
                                System::err::println(t + std::wstring(L" ttype out of range: ") + Arrays->toString(tokensNames));
                                System::err::println((static_cast<CommonTokenStream*>(parser->getInputStream()))->getTokens());
                            } else {
                                return tokensNames[t] + std::wstring(L"<") + t + std::wstring(L">");
                            }
                        }
                        return StringConverterHelper::toString(t);
                    }

                    std::wstring ParserATNSimulator::getLookaheadName(TokenStream *input) {
                        return getTokenName(input->LA(1));
                    }

                    void ParserATNSimulator::dumpDeadEndConfigs(NoViableAltException *nvae) {
                        System::err::println(L"dead end configs: ");
                        for (auto c : nvae->getDeadEndConfigs()) {
                            std::wstring trans = L"no edges";
                            if (c->state->getNumberOfTransitions() > 0) {
                                Transition *t = c->state->transition(0);
                                if (dynamic_cast<AtomTransition*>(t) != nullptr) {
                                    AtomTransition *at = static_cast<AtomTransition*>(t);
                                    trans = std::wstring(L"Atom ") + getTokenName(at->label_Renamed);
                                } else if (dynamic_cast<SetTransition*>(t) != nullptr) {
                                    SetTransition *st = static_cast<SetTransition*>(t);
                                    bool not = dynamic_cast<NotSetTransition*>(st) != nullptr;
                                    trans = (not?L"~":L"") + std::wstring(L"Set ") + st->set->toString();
                                }
                            }
                            System::err::println(c->toString(parser, true) + std::wstring(L":") + trans);
                        }
                    }

                    org::antlr::v4::runtime::NoViableAltException *ParserATNSimulator::noViableAlt(TokenStream *input, ParserRuleContext *outerContext, ATNConfigSet *configs, int startIndex) {
                        return new NoViableAltException(parser, input, input->get(startIndex), input->LT(1), configs, outerContext);
                    }

                    int ParserATNSimulator::getUniqueAlt(ATNConfigSet *configs) {
                        int alt = ATN::INVALID_ALT_NUMBER;
                        for (auto c : configs) {
                            if (alt == ATN::INVALID_ALT_NUMBER) {
                                alt = c->alt; // found first alt
                            } else if (c->alt != alt) {
                                return ATN::INVALID_ALT_NUMBER;
                            }
                        }
                        return alt;
                    }

                    org::antlr::v4::runtime::dfa::DFAState *ParserATNSimulator::addDFAEdge(DFA *dfa, DFAState *from, int t, DFAState *to) {
                        if (debug) {
                            std::cout << std::wstring(L"EDGE ") << from << std::wstring(L" -> ") << to << std::wstring(L" upon ") << getTokenName(t) << std::endl;
                        }

                        if (to == nullptr) {
                            return nullptr;
                        }

                        to = addDFAState(dfa, to); // used existing if possible not incoming
                        if (from == nullptr || t < -1 || t > atn->maxTokenType) {
                            return to;
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(from) {
                            if (from->edges == nullptr) {
                                from->edges = new DFAState[atn->maxTokenType+1 + 1];
                            }

                            from->edges[t + 1] = to; // connect
                        }

                        if (debug) {
                            std::cout << std::wstring(L"DFA=\n") << dfa->toString(parser != nullptr?parser->getTokenNames():nullptr) << std::endl;
                        }

                        return to;
                    }

                    org::antlr::v4::runtime::dfa::DFAState *ParserATNSimulator::addDFAState(DFA *dfa, DFAState *D) {
                        if (D == ERROR) {
                            return D;
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(dfa->states) {
                            DFAState *existing = dfa->states->get(D);
                            if (existing != nullptr) {
                                return existing;
                            }

                            D->stateNumber = dfa->states->size();
                            if (!D->configs->isReadonly()) {
                                D->configs->optimizeConfigs(this);
                                D->configs->setReadonly(true);
                            }
                            dfa->states->put(D, D);
                            if (debug) {
                                std::cout << std::wstring(L"adding new DFA state: ") << D << std::endl;
                            }
                            return D;
                        }
                    }

                    void ParserATNSimulator::reportAttemptingFullContext(DFA *dfa, BitSet *conflictingAlts, ATNConfigSet *configs, int startIndex, int stopIndex) {
                        if (debug || retry_debug) {
                            Interval *interval = Interval::of(startIndex, stopIndex);
                            std::cout << std::wstring(L"reportAttemptingFullContext decision=") << dfa->decision << std::wstring(L":") << configs << std::wstring(L", input=") << parser->getTokenStream()->getText(interval) << std::endl;
                        }
                        if (parser != nullptr) {
                            parser->getErrorListenerDispatch()->reportAttemptingFullContext(parser, dfa, startIndex, stopIndex, conflictingAlts, configs);
                        }
                    }

                    void ParserATNSimulator::reportContextSensitivity(DFA *dfa, int prediction, ATNConfigSet *configs, int startIndex, int stopIndex) {
                        if (debug || retry_debug) {
                            Interval *interval = Interval::of(startIndex, stopIndex);
                            std::cout << std::wstring(L"reportContextSensitivity decision=") << dfa->decision << std::wstring(L":") << configs << std::wstring(L", input=") << parser->getTokenStream()->getText(interval) << std::endl;
                        }
                        if (parser != nullptr) {
                            parser->getErrorListenerDispatch()->reportContextSensitivity(parser, dfa, startIndex, stopIndex, prediction, configs);
                        }
                    }

                    void ParserATNSimulator::reportAmbiguity(DFA *dfa, DFAState *D, int startIndex, int stopIndex, bool exact, BitSet *ambigAlts, ATNConfigSet *configs) {
                        if (debug || retry_debug) {
                                        //			ParserATNPathFinder finder = new ParserATNPathFinder(parser, atn);
                                        //			int i = 1;
                                        //			for (Transition t : dfa.atnStartState.transitions) {
                                        //				System.out.println("ALT "+i+"=");
                                        //				System.out.println(startIndex+".."+stopIndex+", len(input)="+parser.getInputStream().size());
                                        //				TraceTree path = finder.trace(t.target, parser.getContext(), (TokenStream)parser.getInputStream(),
                                        //											  startIndex, stopIndex);
                                        //				if ( path!=null ) {
                                        //					System.out.println("path = "+path.toStringTree());
                                        //					for (TraceTree leaf : path.leaves) {
                                        //						List<ATNState> states = path.getPathToNode(leaf);
                                        //						System.out.println("states="+states);
                                        //					}
                                        //				}
                                        //				i++;
                                        //			}
                            Interval *interval = Interval::of(startIndex, stopIndex);
                            std::cout << std::wstring(L"reportAmbiguity ") << ambigAlts << std::wstring(L":") << configs << std::wstring(L", input=") << parser->getTokenStream()->getText(interval) << std::endl;
                        }
                        if (parser != nullptr) {
                            parser->getErrorListenerDispatch()->reportAmbiguity(parser, dfa, startIndex, stopIndex, exact, ambigAlts, configs);
                        }
                    }

                    void ParserATNSimulator::setPredictionMode(PredictionMode mode) {
                        this->mode = mode;
                    }

                    org::antlr::v4::runtime::atn::PredictionMode ParserATNSimulator::getPredictionMode() {
                        return mode;
                    }

                    void ParserATNSimulator::InitializeInstanceFields() {
                        mode = PredictionMode::LL;
                        _startIndex = 0;
                    }
                }
            }
        }
    }
}

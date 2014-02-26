#include "ATNDeserializer.h"
#include "ATNType.h"
#include "LoopEndState.h"
#include "Pair.h"
#include "BlockStartState.h"
#include "BlockEndState.h"
#include "DecisionState.h"
#include "RuleStartState.h"
#include "Token.h"
#include "RuleStopState.h"
#include "TokensStartState.h"
#include "RuleTransition.h"
#include "EpsilonTransition.h"
#include "PlusLoopbackState.h"
#include "PlusBlockStartState.h"
#include "StarLoopbackState.h"
#include "StarLoopEntryState.h"
#include "BasicBlockStartState.h"
#include "BasicState.h"
#include "AtomTransition.h"
#include "StarBlockStartState.h"
#include "RangeTransition.h"
#include "PredicateTransition.h"
#include "PrecedencePredicateTransition.h"
#include "ActionTransition.h"
#include "SetTransition.h"
#include "NotSetTransition.h"
#include "WildcardTransition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;
                    using org::antlr::v4::runtime::misc::Pair;
const int ATNDeserializer::SERIALIZED_VERSION = 0;

//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not allowed in native C++:
                    ATNDeserializer::ATNDeserializer() {
                        /* This value should never change. Updates following this version are
                         * reflected as change in the unique ID SERIALIZED_UUID.
                         */
                        SERIALIZED_VERSION = 3;
                        /* WARNING: DO NOT MERGE THIS LINE. If UUIDs differ during a merge,
                         * resolve the conflict by generating a new ID!
                         */
                        BASE_SERIALIZED_UUID = UUID::fromString(L"33761B2D-78BB-4A43-8B0B-4F5BEE8AACF3");
                        ADDED_PRECEDENCE_TRANSITIONS = UUID::fromString(L"1DA0C57D-6C06-438A-9B27-10BCB3CE0F61");

                        SUPPORTED_UUIDS = std::vector<UUID*>();
                        SUPPORTED_UUIDS.push_back(BASE_SERIALIZED_UUID);
                        SUPPORTED_UUIDS.push_back(ADDED_PRECEDENCE_TRANSITIONS);

                        SERIALIZED_UUID = ADDED_PRECEDENCE_TRANSITIONS;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNDeserializer::ATNDeserializer() {
                    }

                    ATNDeserializer::ATNDeserializer(ATNDeserializationOptions *deserializationOptions) : deserializationOptions(ATNDeserializationOptions::getDefaultOptions()) {
                        if (deserializationOptions == nullptr) {
                        }

                        this->deserializationOptions = deserializationOptions;
                    }

                    bool ATNDeserializer::isFeatureSupported(UUID *feature, UUID *actualUuid) {
                        int featureIndex = SUPPORTED_UUIDS.find(feature);
                        if (featureIndex < 0) {
                            return false;
                        }

                        return SUPPORTED_UUIDS.find(actualUuid) >= featureIndex;
                    }

                    org::antlr::v4::runtime::atn::ATN *ATNDeserializer::deserialize(wchar_t data[]) {
                        data = data->clone();
                        // don't adjust the first value since that's the version number
                        for (int i = 1; i < sizeof(data) / sizeof(data[0]); i++) {
                            data[i] = static_cast<wchar_t>(data[i] - 2);
                        }

                        int p = 0;
                        int version = toInt(data[p++]);
                        if (version != SERIALIZED_VERSION) {
                            std::wstring reason = std::wstring::format(Locale::getDefault(), L"Could not deserialize ATN with version %d (expected %d).", version, SERIALIZED_VERSION);
                            throw UnsupportedOperationException(InvalidClassException(ATN::typeid::getName(), reason));
                        }

                        UUID *uuid = toUUID(data, p);
                        p += 8;
                        if (!uuid->equals(SERIALIZED_UUID) && !uuid->equals(BASE_SERIALIZED_UUID)) {
                            std::wstring reason = std::wstring::format(Locale::getDefault(), L"Could not deserialize ATN with UUID %s (expected %s or a legacy UUID).", uuid, SERIALIZED_UUID);
                            throw UnsupportedOperationException(InvalidClassException(ATN::typeid::getName(), reason));
                        }

                        bool supportsPrecedencePredicates = isFeatureSupported(ADDED_PRECEDENCE_TRANSITIONS, uuid);

                        ATNType grammarType = ATNType::values()[toInt(data[p++])];
                        int maxTokenType = toInt(data[p++]);
                        ATN *atn = new ATN(grammarType, maxTokenType);

                        //
                        // STATES
                        //
                        std::vector<Pair<LoopEndState*, int>*> loopBackStateNumbers = std::vector<Pair<LoopEndState*, int>*>();
                        std::vector<Pair<BlockStartState*, int>*> endStateNumbers = std::vector<Pair<BlockStartState*, int>*>();
                        int nstates = toInt(data[p++]);
                        for (int i = 0; i < nstates; i++) {
                            int stype = toInt(data[p++]);
                            // ignore bad type of states
                            if (stype == ATNState::INVALID_TYPE) {
                                atn->addState(nullptr);
                                continue;
                            }

                            int ruleIndex = toInt(data[p++]);
                            if (ruleIndex == wchar_t::MAX_VALUE) {
                                ruleIndex = -1;
                            }

                            ATNState *s = stateFactory(stype, ruleIndex);
                            if (stype == ATNState::LOOP_END) { // special case
                                int loopBackStateNumber = toInt(data[p++]);
                                loopBackStateNumbers.push_back(new Pair<LoopEndState*, int>(static_cast<LoopEndState*>(s), loopBackStateNumber));
                            } else if (dynamic_cast<BlockStartState*>(s) != nullptr) {
                                int endStateNumber = toInt(data[p++]);
                                endStateNumbers.push_back(new Pair<BlockStartState*, int>(static_cast<BlockStartState*>(s), endStateNumber));
                            }
                            atn->addState(s);
                        }

                        // delay the assignment of loop back and end states until we know all the state instances have been initialized
                        for (auto pair : loopBackStateNumbers) {
                            pair.a->loopBackState = atn->states[pair.b];
                        }

                        for (auto pair : endStateNumbers) {
                            pair.a->endState = static_cast<BlockEndState*>(atn->states[pair.b]);
                        }

                        int numNonGreedyStates = toInt(data[p++]);
                        for (int i = 0; i < numNonGreedyStates; i++) {
                            int stateNumber = toInt(data[p++]);
                            (static_cast<DecisionState*>(atn->states[stateNumber]))->nonGreedy = true;
                        }

                        if (supportsPrecedencePredicates) {
                            int numPrecedenceStates = toInt(data[p++]);
                            for (int i = 0; i < numPrecedenceStates; i++) {
                                int stateNumber = toInt(data[p++]);
                                (static_cast<RuleStartState*>(atn->states[stateNumber]))->isPrecedenceRule = true;
                            }
                        }

                        //
                        // RULES
                        //
                        int nrules = toInt(data[p++]);
                        if (atn->grammarType == ATNType::LEXER) {
                            atn->ruleToTokenType = new int[nrules];
                            atn->ruleToActionIndex = new int[nrules];
                        }
                        atn->ruleToStartState = new RuleStartState[nrules];
                        for (int i = 0; i < nrules; i++) {
                            int s = toInt(data[p++]);
                            RuleStartState *startState = static_cast<RuleStartState*>(atn->states[s]);
                            atn->ruleToStartState[i] = startState;
                            if (atn->grammarType == ATNType::LEXER) {
                                int tokenType = toInt(data[p++]);
                                if (tokenType == 0xFFFF) {
                                    tokenType = Token::EOF;
                                }

                                atn->ruleToTokenType[i] = tokenType;
                                int actionIndex = toInt(data[p++]);
                                if (actionIndex == 0xFFFF) {
                                    actionIndex = -1;
                                }

                                atn->ruleToActionIndex[i] = actionIndex;
                            }
                        }

                        atn->ruleToStopState = new RuleStopState[nrules];
                        for (ATNState *state : atn->states) {
                            if (!(dynamic_cast<RuleStopState*>(state) != nullptr)) {
                                continue;
                            }

                            RuleStopState *stopState = static_cast<RuleStopState*>(state);
                            atn->ruleToStopState[state.ruleIndex] = stopState;
                            atn->ruleToStartState[state.ruleIndex]->stopState = stopState;
                        }

                        //
                        // MODES
                        //
                        int nmodes = toInt(data[p++]);
                        for (int i = 0; i < nmodes; i++) {
                            int s = toInt(data[p++]);
                            atn->modeToStartState->push_back(static_cast<TokensStartState*>(atn->states[s]));
                        }

                        //
                        // SETS
                        //
                        std::vector<IntervalSet*> sets = std::vector<IntervalSet*>();
                        int nsets = toInt(data[p++]);
                        for (int i = 0; i < nsets; i++) {
                            int nintervals = toInt(data[p]);
                            p++;
                            IntervalSet *set = new IntervalSet();
                            sets.push_back(set);

                            bool containsEof = toInt(data[p++]) != 0;
                            if (containsEof) {
                                set->add(-1);
                            }

                            for (int j = 0; j < nintervals; j++) {
                                set->add(toInt(data[p]), toInt(data[p + 1]));
                                p += 2;
                            }
                        }

                        //
                        // EDGES
                        //
                        int nedges = toInt(data[p++]);
                        for (int i = 0; i < nedges; i++) {
                            int src = toInt(data[p]);
                            int trg = toInt(data[p + 1]);
                            int ttype = toInt(data[p + 2]);
                            int arg1 = toInt(data[p + 3]);
                            int arg2 = toInt(data[p + 4]);
                            int arg3 = toInt(data[p + 5]);
                            Transition *trans = edgeFactory(atn, ttype, src, trg, arg1, arg2, arg3, sets);
                                        //			System.out.println("EDGE "+trans.getClass().getSimpleName()+" "+
                                        //							   src+"->"+trg+
                                        //					   " "+Transition.serializationNames[ttype]+
                                        //					   " "+arg1+","+arg2+","+arg3);
                            ATNState *srcState = atn->states[src];
                            srcState->addTransition(trans);
                            p += 6;
                        }

                        // edges for rule stop states can be derived, so they aren't serialized
                        for (ATNState *state : atn->states) {
                            for (int i = 0; i < state.getNumberOfTransitions(); i++) {
                                Transition *t = state.transition(i);
                                if (!(dynamic_cast<RuleTransition*>(t) != nullptr)) {
                                    continue;
                                }

                                RuleTransition *ruleTransition = static_cast<RuleTransition*>(t);
                                atn->ruleToStopState[ruleTransition->target->ruleIndex]->addTransition(new EpsilonTransition(ruleTransition->followState));
                            }
                        }

                        for (ATNState *state : atn->states) {
                            if (dynamic_cast<BlockStartState*>(state) != nullptr) {
                                // we need to know the end state to set its start state
                                if ((static_cast<BlockStartState*>(state))->endState == nullptr) {
                                    throw IllegalStateException();
                                }

                                // block end states can only be associated to a single block start state
                                if ((static_cast<BlockStartState*>(state))->endState->startState != nullptr) {
                                    throw IllegalStateException();
                                }

                                (static_cast<BlockStartState*>(state))->endState->startState = static_cast<BlockStartState*>(state);
                            }

                            if (dynamic_cast<PlusLoopbackState*>(state) != nullptr) {
                                PlusLoopbackState *loopbackState = static_cast<PlusLoopbackState*>(state);
                                for (int i = 0; i < loopbackState->getNumberOfTransitions(); i++) {
                                    ATNState *target = loopbackState->transition(i)->target;
                                    if (dynamic_cast<PlusBlockStartState*>(target) != nullptr) {
                                        (static_cast<PlusBlockStartState*>(target))->loopBackState = loopbackState;
                                    }
                                }
                            } else if (dynamic_cast<StarLoopbackState*>(state) != nullptr) {
                                StarLoopbackState *loopbackState = static_cast<StarLoopbackState*>(state);
                                for (int i = 0; i < loopbackState->getNumberOfTransitions(); i++) {
                                    ATNState *target = loopbackState->transition(i)->target;
                                    if (dynamic_cast<StarLoopEntryState*>(target) != nullptr) {
                                        (static_cast<StarLoopEntryState*>(target))->loopBackState = loopbackState;
                                    }
                                }
                            }
                        }

                        //
                        // DECISIONS
                        //
                        int ndecisions = toInt(data[p++]);
                        for (int i = 1; i <= ndecisions; i++) {
                            int s = toInt(data[p++]);
                            DecisionState *decState = static_cast<DecisionState*>(atn->states[s]);
                            atn->decisionToState->push_back(decState);
                            decState->decision = i - 1;
                        }

                        if (deserializationOptions->isVerifyATN()) {
                            verifyATN(atn);
                        }

                        if (deserializationOptions->isGenerateRuleBypassTransitions() && atn->grammarType == ATNType::PARSER) {
                            atn->ruleToTokenType = new int[atn->ruleToStartState->length];
                            for (int i = 0; i < atn->ruleToStartState->length; i++) {
                                atn->ruleToTokenType[i] = atn->maxTokenType + i + 1;
                            }

                            for (int i = 0; i < atn->ruleToStartState->length; i++) {
                                BasicBlockStartState *bypassStart = new BasicBlockStartState();
                                bypassStart->ruleIndex = i;
                                atn->addState(bypassStart);

                                BlockEndState *bypassStop = new BlockEndState();
                                bypassStop->ruleIndex = i;
                                atn->addState(bypassStop);

                                bypassStart->endState = bypassStop;
                                atn->defineDecisionState(bypassStart);

                                bypassStop->startState = bypassStart;

                                ATNState *endState;
                                Transition *excludeTransition = nullptr;
                                if (atn->ruleToStartState[i]->isPrecedenceRule) {
                                    // wrap from the beginning of the rule to the StarLoopEntryState
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                    delete endState;
                                    for (ATNState *state : atn->states) {
                                        if (state.ruleIndex != i) {
                                            continue;
                                        }

                                        if (!(dynamic_cast<StarLoopEntryState*>(state) != nullptr)) {
                                            continue;
                                        }

                                        ATNState *maybeLoopEndState = state.transition(state.getNumberOfTransitions() - 1)->target;
                                        if (!(dynamic_cast<LoopEndState*>(maybeLoopEndState) != nullptr)) {
                                            continue;
                                        }

                                        if (maybeLoopEndState->epsilonOnlyTransitions && dynamic_cast<RuleStopState*>(maybeLoopEndState->transition(0)->target) != nullptr) {
                                            endState = state;
                                            break;
                                        }
                                    }

                                    if (endState == nullptr) {
                                        throw UnsupportedOperationException(L"Couldn't identify final state of the precedence rule prefix section.");
                                    }

                                    excludeTransition = (static_cast<StarLoopEntryState*>(endState))->loopBackState->transition(0);
                                } else {
                                    endState = atn->ruleToStopState[i];
                                }

                                // all non-excluded transitions that currently target end state need to target blockEnd instead
                                for (ATNState *state : atn->states) {
                                    for (Transition *transition : state.transitions) {
                                        if (transition == excludeTransition) {
                                            continue;
                                        }

                                        if (transition.target == endState) {
                                            transition.target = bypassStop;
                                        }
                                    }
                                }

                                // all transitions leaving the rule start state need to leave blockStart instead
                                while (atn->ruleToStartState[i]->getNumberOfTransitions() > 0) {
                                    Transition *transition = atn->ruleToStartState[i]->removeTransition(atn->ruleToStartState[i]->getNumberOfTransitions() - 1);
                                    bypassStart->addTransition(transition);
                                }

                                // link the new states
                                atn->ruleToStartState[i]->addTransition(new EpsilonTransition(bypassStart));
                                bypassStop->addTransition(new EpsilonTransition(endState));

                                ATNState *matchState = new BasicState();
                                atn->addState(matchState);
                                matchState->addTransition(new AtomTransition(bypassStop, atn->ruleToTokenType[i]));
                                bypassStart->addTransition(new EpsilonTransition(matchState));
                            }

                            if (deserializationOptions->isVerifyATN()) {
                                // reverify after modification
                                verifyATN(atn);
                            }
                        }

                        return atn;
                    }

                    void ATNDeserializer::verifyATN(ATN *atn) {
                        // verify assumptions
                        for (ATNState *state : atn->states) {
                            if (state == nullptr) {
                                continue;
                            }

                            checkCondition(state.onlyHasEpsilonTransitions() || state.getNumberOfTransitions() <= 1);

                            if (dynamic_cast<PlusBlockStartState*>(state) != nullptr) {
                                checkCondition((static_cast<PlusBlockStartState*>(state))->loopBackState != nullptr);
                            }

                            if (dynamic_cast<StarLoopEntryState*>(state) != nullptr) {
                                StarLoopEntryState *starLoopEntryState = static_cast<StarLoopEntryState*>(state);
                                checkCondition(starLoopEntryState->loopBackState != nullptr);
                                checkCondition(starLoopEntryState->getNumberOfTransitions() == 2);

                                if (dynamic_cast<StarBlockStartState*>(starLoopEntryState->transition(0)->target) != nullptr) {
                                    checkCondition(dynamic_cast<LoopEndState*>(starLoopEntryState->transition(1)->target) != nullptr);
                                    checkCondition(!starLoopEntryState->nonGreedy);
                                } else if (dynamic_cast<LoopEndState*>(starLoopEntryState->transition(0)->target) != nullptr) {
                                    checkCondition(dynamic_cast<StarBlockStartState*>(starLoopEntryState->transition(1)->target) != nullptr);
                                    checkCondition(starLoopEntryState->nonGreedy);
                                } else {
                                    throw IllegalStateException();
                                }
                            }

                            if (dynamic_cast<StarLoopbackState*>(state) != nullptr) {
                                checkCondition(state.getNumberOfTransitions() == 1);
                                checkCondition(dynamic_cast<StarLoopEntryState*>(state.transition(0)->target) != nullptr);
                            }

                            if (dynamic_cast<LoopEndState*>(state) != nullptr) {
                                checkCondition((static_cast<LoopEndState*>(state))->loopBackState != nullptr);
                            }

                            if (dynamic_cast<RuleStartState*>(state) != nullptr) {
                                checkCondition((static_cast<RuleStartState*>(state))->stopState != nullptr);
                            }

                            if (dynamic_cast<BlockStartState*>(state) != nullptr) {
                                checkCondition((static_cast<BlockStartState*>(state))->endState != nullptr);
                            }

                            if (dynamic_cast<BlockEndState*>(state) != nullptr) {
                                checkCondition((static_cast<BlockEndState*>(state))->startState != nullptr);
                            }

                            if (dynamic_cast<DecisionState*>(state) != nullptr) {
                                DecisionState *decisionState = static_cast<DecisionState*>(state);
                                checkCondition(decisionState->getNumberOfTransitions() <= 1 || decisionState->decision >= 0);
                            } else {
                                checkCondition(state.getNumberOfTransitions() <= 1 || dynamic_cast<RuleStopState*>(state) != nullptr);
                            }
                        }
                    }

                    void ATNDeserializer::checkCondition(bool condition) {
                        checkCondition(condition, L"");
                    }

                    void ATNDeserializer::checkCondition(bool condition, const std::wstring &message) {
                        if (!condition) {
                            throw IllegalStateException(message);
                        }
                    }

                    int ATNDeserializer::toInt(wchar_t c) {
                        return c;
                    }

                    int ATNDeserializer::toInt32(wchar_t data[], int offset) {
                        return static_cast<int>(data[offset]) | (static_cast<int>(data[offset + 1]) << 16);
                    }

                    long long ATNDeserializer::toLong(wchar_t data[], int offset) {
                        long long lowOrder = toInt32(data, offset) & 0x00000000FFFFFFFFLL;
                        return lowOrder | (static_cast<long long>(toInt32(data, offset + 2)) << 32);
                    }

                    UUID *ATNDeserializer::toUUID(wchar_t data[], int offset) {
                        long long leastSigBits = toLong(data, offset);
                        long long mostSigBits = toLong(data, offset + 4);
                        return new UUID(mostSigBits, leastSigBits);
                    }

                    org::antlr::v4::runtime::atn::Transition *ATNDeserializer::edgeFactory(ATN *atn, int type, int src, int trg, int arg1, int arg2, int arg3, std::vector<IntervalSet*> &sets) {
                        ATNState *target = atn->states[trg];
                        switch (type) {
                            case Transition::EPSILON :
                                return new EpsilonTransition(target);
                            case Transition::RANGE :
                                if (arg3 != 0) {
                                    return new RangeTransition(target, Token::EOF, arg2);
                                } else {
                                    return new RangeTransition(target, arg1, arg2);
                                }
                            case Transition::RULE :
                                RuleTransition *rt = new RuleTransition(static_cast<RuleStartState*>(atn->states[arg1]), arg2, arg3, target);
                                return rt;
                            case Transition::PREDICATE :
                                PredicateTransition *pt = new PredicateTransition(target, arg1, arg2, arg3 != 0);
                                return pt;
                            case Transition::PRECEDENCE:
                                return new PrecedencePredicateTransition(target, arg1);
                            case Transition::ATOM :
                                if (arg3 != 0) {
                                    return new AtomTransition(target, Token::EOF);
                                } else {
                                    return new AtomTransition(target, arg1);
                                }
                            case Transition::ACTION :
                                ActionTransition *a = new ActionTransition(target, arg1, arg2, arg3 != 0);
                                return a;
                            case Transition::SET :
                                return new SetTransition(target, sets[arg1]);
                            case Transition::NOT_SET :
                                return new NotSetTransition(target, sets[arg1]);
                            case Transition::WILDCARD :
                                return new WildcardTransition(target);
                        }

                        throw IllegalArgumentException(L"The specified transition type is not valid.");
                    }

                    org::antlr::v4::runtime::atn::ATNState *ATNDeserializer::stateFactory(int type, int ruleIndex) {
                        ATNState *s;
                        switch (type) {
                            case ATNState::INVALID_TYPE:
                                return nullptr;
                            case ATNState::BASIC :
                                s = new BasicState();
                                break;
                            case ATNState::RULE_START :
                                s = new RuleStartState();
                                break;
                            case ATNState::BLOCK_START :
                                s = new BasicBlockStartState();
                                break;
                            case ATNState::PLUS_BLOCK_START :
                                s = new PlusBlockStartState();
                                break;
                            case ATNState::STAR_BLOCK_START :
                                s = new StarBlockStartState();
                                break;
                            case ATNState::TOKEN_START :
                                s = new TokensStartState();
                                break;
                            case ATNState::RULE_STOP :
                                s = new RuleStopState();
                                break;
                            case ATNState::BLOCK_END :
                                s = new BlockEndState();
                                break;
                            case ATNState::STAR_LOOP_BACK :
                                s = new StarLoopbackState();
                                break;
                            case ATNState::STAR_LOOP_ENTRY :
                                s = new StarLoopEntryState();
                                break;
                            case ATNState::PLUS_LOOP_BACK :
                                s = new PlusLoopbackState();
                                break;
                            case ATNState::LOOP_END :
                                s = new LoopEndState();
                                break;
                            default :
                                std::wstring message = std::wstring::format(Locale::getDefault(), L"The specified state type %d is not valid.", type);
                                throw IllegalArgumentException(message);
                        }

                        s->ruleIndex = ruleIndex;
                        return s;
                    }
                }
            }
        }
    }
}

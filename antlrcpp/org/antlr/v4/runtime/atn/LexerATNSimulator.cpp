#include "LexerATNSimulator.h"
#include "Java/src/org/antlr/v4/runtime/IntStream.h"
#include "Java/src/org/antlr/v4/runtime/atn/OrderedATNConfigSet.h"
#include "Java/src/org/antlr/v4/runtime/Token.h"
#include "Java/src/org/antlr/v4/runtime/LexerNoViableAltException.h"
#include "Java/src/org/antlr/v4/runtime/atn/PredictionContext.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleStopState.h"
#include "Java/src/org/antlr/v4/runtime/atn/RuleTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/SingletonPredictionContext.h"
#include "Java/src/org/antlr/v4/runtime/atn/PredicateTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/ActionTransition.h"
#include "Java/src/org/antlr/v4/runtime/atn/ATNConfig.h"
#include "Java/src/org/antlr/v4/runtime/misc/Interval.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::CharStream;
                    using org::antlr::v4::runtime::IntStream;
                    using org::antlr::v4::runtime::Lexer;
                    using org::antlr::v4::runtime::LexerNoViableAltException;
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::dfa::DFA;
                    using org::antlr::v4::runtime::dfa::DFAState;
                    using org::antlr::v4::runtime::misc::Interval;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;

                    void LexerATNSimulator::SimState::reset() {
                        index = -1;
                        line = 0;
                        charPos = -1;
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                        delete dfaState;
                    }

                    void LexerATNSimulator::SimState::InitializeInstanceFields() {
                        index = -1;
                        line = 0;
                        charPos = -1;
                    }

int LexerATNSimulator::match_calls = 0;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    LexerATNSimulator::LexerATNSimulator(ATN *atn, DFA decisionToDFA[], PredictionContextCache *sharedContextCache) : prevAccept(new SimState()) {
                    }

                    LexerATNSimulator::LexerATNSimulator(Lexer *recog, ATN *atn, DFA decisionToDFA[], PredictionContextCache *sharedContextCache) : ATNSimulator(atn,sharedContextCache), recog(recog), decisionToDFA(decisionToDFA), prevAccept(new SimState()) {
                        InitializeInstanceFields();
                    }

                    void LexerATNSimulator::copyState(LexerATNSimulator *simulator) {
                        this->charPositionInLine = simulator->charPositionInLine;
                        this->line = simulator->line;
                        this->mode = simulator->mode;
                        this->startIndex = simulator->startIndex;
                    }

                    int LexerATNSimulator::match(CharStream *input, int mode) {
                        match_calls++;
                        this->mode = mode;
                        int mark = input->mark();
                        try {
                            this->startIndex = input->index();
                            this->prevAccept->reset();
                            DFA *dfa = decisionToDFA[mode];
                            if (dfa->s0 == nullptr) {
                                return matchATN(input);
                            } else {
                                return execATN(input, dfa->s0);
                            }
                        } finally {
                            input->release(mark);
                        }
                    }

                    void LexerATNSimulator::reset() {
                        prevAccept->reset();
                        startIndex = -1;
                        line = 1;
                        charPositionInLine = 0;
                        mode = Lexer::DEFAULT_MODE;
                    }

                    int LexerATNSimulator::matchATN(CharStream *input) {
                        ATNState *startState = atn->modeToStartState[mode];

                        if (debug) {
                            System::out::format(Locale::getDefault(), L"matchATN mode %d start: %s\n", mode, startState);
                        }

                        int old_mode = mode;

                        ATNConfigSet *s0_closure = computeStartState(input, startState);
                        bool suppressEdge = s0_closure->hasSemanticContext;
                        s0_closure->hasSemanticContext = false;

                        DFAState *next = addDFAState(s0_closure);
                        if (!suppressEdge) {
                            decisionToDFA[mode]->s0 = next;
                        }

                        int predict = execATN(input, next);

                        if (debug) {
                            System::out::format(Locale::getDefault(), L"DFA after matchATN: %s\n", decisionToDFA[old_mode]->toLexerString());
                        }

                        return predict;
                    }

                    int LexerATNSimulator::execATN(CharStream *input, DFAState *ds0) {
                        //System.out.println("enter exec index "+input.index()+" from "+ds0.configs);
                        if (debug) {
                            System::out::format(Locale::getDefault(), L"start state closure=%s\n", ds0->configs);
                        }

                        int t = input->LA(1);
                        DFAState *s = ds0; // s is current/from DFA state

                        while (true) { // while more work
                            if (debug) {
                                System::out::format(Locale::getDefault(), L"execATN loop starting closure: %s\n", s->configs);
                            }

                            // As we move src->trg, src->trg, we keep track of the previous trg to
                            // avoid looking up the DFA state again, which is expensive.
                            // If the previous target was already part of the DFA, we might
                            // be able to avoid doing a reach operation upon t. If s!=null,
                            // it means that semantic predicates didn't prevent us from
                            // creating a DFA state. Once we know s!=null, we check to see if
                            // the DFA state has an edge already for t. If so, we can just reuse
                            // it's configuration set; there's no point in re-computing it.
                            // This is kind of like doing DFA simulation within the ATN
                            // simulation because DFA simulation is really just a way to avoid
                            // computing reach/closure sets. Technically, once we know that
                            // we have a previously added DFA state, we could jump over to
                            // the DFA simulator. But, that would mean popping back and forth
                            // a lot and making things more complicated algorithmically.
                            // This optimization makes a lot of sense for loops within DFA.
                            // A character will take us back to an existing DFA state
                            // that already has lots of edges out of it. e.g., .* in comments.
                            DFAState *target = getExistingTargetState(s, t);
                            if (target == nullptr) {
                                target = computeTargetState(input, s, t);
                            }

                            if (target == ERROR) {
                                break;
                            }

                            if (target->isAcceptState) {
                                captureSimState(prevAccept, input, target);
                                if (t == IntStream::EOF) {
                                    break;
                                }
                            }

                            if (t != IntStream::EOF) {
                                consume(input);
                                t = input->LA(1);
                            }

                            s = target; // flip; current DFA target becomes new src/from state
                        }

                        return failOrAccept(prevAccept, input, s->configs, t);
                    }

                    org::antlr::v4::runtime::dfa::DFAState *LexerATNSimulator::getExistingTargetState(DFAState *s, int t) {
                        if (s->edges == nullptr || t < MIN_DFA_EDGE || t > MAX_DFA_EDGE) {
                            return nullptr;
                        }

                        DFAState *target = s->edges[t - MIN_DFA_EDGE];
                        if (debug && target != nullptr) {
                            std::cout << std::wstring(L"reuse state ") << s->stateNumber << std::wstring(L" edge to ") << target->stateNumber << std::endl;
                        }

                        return target;
                    }

                    org::antlr::v4::runtime::dfa::DFAState *LexerATNSimulator::computeTargetState(CharStream *input, DFAState *s, int t) {
                        ATNConfigSet *reach = new OrderedATNConfigSet();

                        // if we don't find an existing DFA state
                        // Fill reach starting from closure, following t transitions
                        getReachableConfigSet(input, s->configs, reach, t);

                        if (reach->isEmpty()) { // we got nowhere on t from s
                            // we got nowhere on t, don't throw out this knowledge; it'd
                            // cause a failover from DFA later.
                            addDFAEdge(s, t, ERROR);
                            // stop when we can't match any more char
                            return ERROR;
                        }

                        // Add an edge from s to target DFA found/created for reach
                        return addDFAEdge(s, t, reach);
                    }

                    int LexerATNSimulator::failOrAccept(SimState *prevAccept, CharStream *input, ATNConfigSet *reach, int t) {
                        if (prevAccept->dfaState != nullptr) {
                            int ruleIndex = prevAccept->dfaState->lexerRuleIndex;
                            int actionIndex = prevAccept->dfaState->lexerActionIndex;
                            accept(input, ruleIndex, actionIndex, prevAccept->index, prevAccept->line, prevAccept->charPos);
                            return prevAccept->dfaState->prediction;
                        } else {
                            // if no accept and EOF is first char, return EOF
                            if (t == IntStream::EOF && input->index() == startIndex) {
                                return Token::EOF;
                            }

                            throw LexerNoViableAltException(recog, input, startIndex, reach);
                        }
                    }

                    void LexerATNSimulator::getReachableConfigSet(CharStream *input, ATNConfigSet *closure, ATNConfigSet *reach, int t) {
                        // this is used to skip processing for configs which have a lower priority
                        // than a config that already reached an accept state for the same rule
                        int skipAlt = ATN::INVALID_ALT_NUMBER;
                        for (auto c : closure) {
                            bool currentAltReachedAcceptState = c->alt == skipAlt;
                            if (currentAltReachedAcceptState && (static_cast<LexerATNConfig*>(c))->hasPassedThroughNonGreedyDecision()) {
                                continue;
                            }

                            if (debug) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                System::out::format(Locale::getDefault(), L"testing %s at %s\n", getTokenName(t), c->toString(recog, true));
                            }

                            int n = c->state->getNumberOfTransitions();
                            for (int ti = 0; ti < n; ti++) { // for each transition
                                Transition *trans = c->state->transition(ti);
                                ATNState *target = getReachableTarget(trans, t);
                                if (target != nullptr) {
                                    if (closure(input, new LexerATNConfig(static_cast<LexerATNConfig*>(c), target), reach, currentAltReachedAcceptState, true)) {
                                        // any remaining configs for this alt have a lower priority than
                                        // the one that just reached an accept state.
                                        skipAlt = c->alt;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    void LexerATNSimulator::accept(CharStream *input, int ruleIndex, int actionIndex, int index, int line, int charPos) {
                        if (debug) {
                            System::out::format(Locale::getDefault(), L"ACTION %s:%d\n", recog != nullptr ? recog->getRuleNames()[ruleIndex] : ruleIndex, actionIndex);
                        }

                        if (actionIndex >= 0 && recog != nullptr) {
                            recog->action(nullptr, ruleIndex, actionIndex);
                        }

                        // seek to after last char in token
                        input->seek(index);
                        this->line = line;
                        this->charPositionInLine = charPos;
                        if (input->LA(1) != IntStream::EOF) {
                            consume(input);
                        }
                    }

                    org::antlr::v4::runtime::atn::ATNState *LexerATNSimulator::getReachableTarget(Transition *trans, int t) {
                        if (trans->matches(t, wchar_t::MIN_VALUE, wchar_t::MAX_VALUE)) {
                            return trans->target;
                        }

                        return nullptr;
                    }

                    org::antlr::v4::runtime::atn::ATNConfigSet *LexerATNSimulator::computeStartState(CharStream *input, ATNState *p) {
                        PredictionContext *initialContext = PredictionContext::EMPTY;
                        ATNConfigSet *configs = new OrderedATNConfigSet();
                        for (int i = 0; i < p->getNumberOfTransitions(); i++) {
                            ATNState *target = p->transition(i)->target;
                            LexerATNConfig *c = new LexerATNConfig(target, i + 1, initialContext);
                            closure(input, c, configs, false, false);
                        }
                        return configs;
                    }

                    bool LexerATNSimulator::closure(CharStream *input, LexerATNConfig *config, ATNConfigSet *configs, bool currentAltReachedAcceptState, bool speculative) {
                        if (debug) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            std::cout << std::wstring(L"closure(") << config->toString(recog, true) << std::wstring(L")") << std::endl;
                        }

                        if (dynamic_cast<RuleStopState*>(config->state) != nullptr) {
                            if (debug) {
                                if (recog != nullptr) {
                                    System::out::format(Locale::getDefault(), L"closure at %s rule stop %s\n", recog->getRuleNames()[config->state->ruleIndex], config);
                                } else {
                                    System::out::format(Locale::getDefault(), L"closure at rule stop %s\n", config);
                                }
                            }

                            if (config->context == nullptr || config->context->hasEmptyPath()) {
                                if (config->context == nullptr || config->context->isEmpty()) {
                                    configs->add(config);
                                    return true;
                                } else {
                                    configs->add(new LexerATNConfig(config, config->state, PredictionContext::EMPTY));
                                    currentAltReachedAcceptState = true;
                                }
                            }

                            if (config->context != nullptr && !config->context->isEmpty()) {
                                for (int i = 0; i < config->context->size(); i++) {
                                    if (config->context->getReturnState(i) != PredictionContext::EMPTY_RETURN_STATE) {
                                        PredictionContext *newContext = config->context->getParent(i); // "pop" return state
                                        ATNState *returnState = atn->states[config->context->getReturnState(i)];
                                        LexerATNConfig *c = new LexerATNConfig(returnState, config->alt, newContext);
                                        currentAltReachedAcceptState = closure(input, c, configs, currentAltReachedAcceptState, speculative);
                                    }
                                }
                            }

                            return currentAltReachedAcceptState;
                        }

                        // optimization
                        if (!config->state->onlyHasEpsilonTransitions()) {
                            if (!currentAltReachedAcceptState || !config->hasPassedThroughNonGreedyDecision()) {
                                configs->add(config);
                            }
                        }

                        ATNState *p = config->state;
                        for (int i = 0; i < p->getNumberOfTransitions(); i++) {
                            Transition *t = p->transition(i);
                            LexerATNConfig *c = getEpsilonTarget(input, config, t, configs, speculative);
                            if (c != nullptr) {
                                currentAltReachedAcceptState = closure(input, c, configs, currentAltReachedAcceptState, speculative);
                            }
                        }

                        return currentAltReachedAcceptState;
                    }

                    org::antlr::v4::runtime::atn::LexerATNConfig *LexerATNSimulator::getEpsilonTarget(CharStream *input, LexerATNConfig *config, Transition *t, ATNConfigSet *configs, bool speculative) {
                        LexerATNConfig *c = nullptr;
                        switch (t->getSerializationType()) {
                            case Transition::RULE:
                                RuleTransition *ruleTransition = static_cast<RuleTransition*>(t);
                                PredictionContext *newContext = SingletonPredictionContext::create(config->context, ruleTransition->followState->stateNumber);
                                c = new LexerATNConfig(config, t->target, newContext);
                                break;

                            case Transition::PRECEDENCE:
                                throw UnsupportedOperationException(L"Precedence predicates are not supported in lexers.");

                            case Transition::PREDICATE:
                                /*  Track traversing semantic predicates. If we traverse,
                                 we cannot add a DFA state for this "reach" computation
                                 because the DFA would not test the predicate again in the
                                 future. Rather than creating collections of semantic predicates
                                 like v3 and testing them on prediction, v4 will test them on the
                                 fly all the time using the ATN not the DFA. This is slower but
                                 semantically it's not used that often. One of the key elements to
                                 this predicate mechanism is not adding DFA states that see
                                 predicates immediately afterwards in the ATN. For example,
                                        
                                 a : ID {p1}? | ID {p2}? ;
                                        
                                 should create the start state for rule 'a' (to save start state
                                 competition), but should not create target of ID state. The
                                 collection of ATN states the following ID references includes
                                 states reached by traversing predicates. Since this is when we
                                 test them, we cannot cash the DFA state target of ID.
                             */
                                PredicateTransition *pt = static_cast<PredicateTransition*>(t);
                                if (debug) {
                                    std::cout << std::wstring(L"EVAL rule ") << pt->ruleIndex << std::wstring(L":") << pt->predIndex << std::endl;
                                }
                                configs->hasSemanticContext = true;
                                if (evaluatePredicate(input, pt->ruleIndex, pt->predIndex, speculative)) {
                                    c = new LexerATNConfig(config, t->target);
                                }
                                break;
                            // ignore actions; just exec one per rule upon accept
                            case Transition::ACTION:
                                c = new LexerATNConfig(config, t->target, (static_cast<ActionTransition*>(t))->actionIndex);
                                break;
                            case Transition::EPSILON:
                                c = new LexerATNConfig(config, t->target);
                                break;
                        }

                        return c;
                    }

                    bool LexerATNSimulator::evaluatePredicate(CharStream *input, int ruleIndex, int predIndex, bool speculative) {
                        // assume true if no recognizer was provided
                        if (recog == nullptr) {
                            return true;
                        }

                        if (!speculative) {
                            return recog->sempred(nullptr, ruleIndex, predIndex);
                        }

                        int savedCharPositionInLine = charPositionInLine;
                        int savedLine = line;
                        int index = input->index();
                        int marker = input->mark();
                        try {
                            consume(input);
                            return recog->sempred(nullptr, ruleIndex, predIndex);
                        } finally {
                            charPositionInLine = savedCharPositionInLine;
                            line = savedLine;
                            input->seek(index);
                            input->release(marker);
                        }
                    }

                    void LexerATNSimulator::captureSimState(SimState *settings, CharStream *input, DFAState *dfaState) {
                        settings->index = input->index();
                        settings->line = line;
                        settings->charPos = charPositionInLine;
                        settings->dfaState = dfaState;
                    }

                    org::antlr::v4::runtime::dfa::DFAState *LexerATNSimulator::addDFAEdge(DFAState *from, int t, ATNConfigSet *q) {
                        /* leading to this call, ATNConfigSet.hasSemanticContext is used as a
                         * marker indicating dynamic predicate evaluation makes this edge
                         * dependent on the specific input sequence, so the static edge in the
                         * DFA should be omitted. The target DFAState is still created since
                         * execATN has the ability to resynchronize with the DFA state cache
                         * following the predicate evaluation step.
                         *
                         * TJP notes: next time through the DFA, we see a pred again and eval.
                         * If that gets us to a previously created (but dangling) DFA
                         * state, we can continue in pure DFA mode from there.
                         */
                        bool suppressEdge = q->hasSemanticContext;
                        q->hasSemanticContext = false;

                        DFAState *to = addDFAState(q);

                        if (suppressEdge) {
                            return to;
                        }

                        addDFAEdge(from, t, to);
                        return to;
                    }

                    void LexerATNSimulator::addDFAEdge(DFAState *p, int t, DFAState *q) {
                        if (t < MIN_DFA_EDGE || t > MAX_DFA_EDGE) {
                            // Only track edges within the DFA bounds
                            return;
                        }

                        if (debug) {
                            std::cout << std::wstring(L"EDGE ") << p << std::wstring(L" -> ") << q << std::wstring(L" upon ") << (static_cast<wchar_t>(t)) << std::endl;
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(p) {
                            if (p->edges == nullptr) {
                                //  make room for tokens 1..n and -1 masquerading as index 0
                                p->edges = new DFAState[MAX_DFA_EDGE - MIN_DFA_EDGE+1];
                            }
                            p->edges[t - MIN_DFA_EDGE] = q; // connect
                        }
                    }

                    org::antlr::v4::runtime::dfa::DFAState *LexerATNSimulator::addDFAState(ATNConfigSet *configs) {
                        /* the lexer evaluates predicates on-the-fly; by this point configs
                         * should not contain any configurations with unevaluated predicates.
                         */
                        assert(!configs->hasSemanticContext);

                        DFAState *proposed = new DFAState(configs);
                        ATNConfig *firstConfigWithRuleStopState = nullptr;
                        for (auto c : configs) {
                            if (dynamic_cast<RuleStopState*>(c->state) != nullptr) {
                                firstConfigWithRuleStopState = c;
                                break;
                            }
                        }

                        if (firstConfigWithRuleStopState != nullptr) {
                            proposed->isAcceptState = true;
                            proposed->lexerRuleIndex = firstConfigWithRuleStopState->state->ruleIndex;
                            proposed->lexerActionIndex = (static_cast<LexerATNConfig*>(firstConfigWithRuleStopState))->lexerActionIndex;
                            proposed->prediction = atn->ruleToTokenType[proposed->lexerRuleIndex];
                        }

                        DFA *dfa = decisionToDFA[mode];
//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(dfa->states) {
                            DFAState *existing = dfa->states->get(proposed);
                            if (existing != nullptr) {
                                return existing;
                            }

                            DFAState *newState = proposed;

                            newState->stateNumber = dfa->states->size();
                            configs->setReadonly(true);
                            newState->configs = configs;
                            dfa->states->put(newState, newState);
                            return newState;
                        }
                    }

                    org::antlr::v4::runtime::dfa::DFA *LexerATNSimulator::getDFA(int mode) {
                        return decisionToDFA[mode];
                    }

                    std::wstring LexerATNSimulator::getText(CharStream *input) {
                        // index is first lookahead char, don't include.
                        return input->getText(Interval::of(startIndex, input->index() - 1));
                    }

                    int LexerATNSimulator::getLine() {
                        return line;
                    }

                    void LexerATNSimulator::setLine(int line) {
                        this->line = line;
                    }

                    int LexerATNSimulator::getCharPositionInLine() {
                        return charPositionInLine;
                    }

                    void LexerATNSimulator::setCharPositionInLine(int charPositionInLine) {
                        this->charPositionInLine = charPositionInLine;
                    }

                    void LexerATNSimulator::consume(CharStream *input) {
                        int curChar = input->LA(1);
                        if (curChar == L'\n') {
                            line++;
                            charPositionInLine = 0;
                        } else {
                            charPositionInLine++;
                        }
                        input->consume();
                    }

                    std::wstring LexerATNSimulator::getTokenName(int t) {
                        if (t == -1) {
                            return L"EOF";
                        }
                        //if ( atn.g!=null ) return atn.g.getTokenDisplayName(t);
                        return std::wstring(L"'") + static_cast<wchar_t>(t) + std::wstring(L"'");
                    }

                    void LexerATNSimulator::InitializeInstanceFields() {
                        startIndex = -1;
                        line = 1;
                        charPositionInLine = 0;
                        mode = org::antlr::v4::runtime::Lexer::DEFAULT_MODE;
                    }
                }
            }
        }
    }
}

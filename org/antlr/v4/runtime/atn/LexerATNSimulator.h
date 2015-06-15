﻿#pragma once

#include <string>
#include <iostream>

#include "ATNSimulator.h"
#include "Declarations.h"

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

                    /// <summary>
                    /// "dup" of ParserInterpreter </summary>
                    class LexerATNSimulator : public ATNSimulator {
                    protected:
                        class SimState {
                        protected:
                            int index;
                            int line;
                            int charPos;
                            dfa::DFAState *dfaState;
                            virtual void reset();
                            friend class LexerATNSimulator;
                            
                        private:
                            void InitializeInstanceFields();

                        public:
                            SimState() {
                                InitializeInstanceFields();
                            }
                            
                        };
                        
                        
                    public:
                        static const bool debug = false;
                        static const bool dfa_debug = false;

                        static const int MIN_DFA_EDGE = 0;
                        static const int MAX_DFA_EDGE = 127; // forces unicode to stay in ATN

                        /// <summary>
                        /// When we hit an accept state in either the DFA or the ATN, we
                        ///  have to notify the character stream to start buffering characters
                        ///  via <seealso cref="IntStream#mark"/> and record the current state. The current sim state
                        ///  includes the current index into the input, the current line,
                        ///  and current character position in that line. Note that the Lexer is
                        ///  tracking the starting line and characterization of the token. These
                        ///  variables track the "state" of the simulator when it hits an accept state.
                        /// <p/>
                        ///  We track these variables separately for the DFA and ATN simulation
                        ///  because the DFA simulation often has to fail over to the ATN
                        ///  simulation. If the ATN simulation fails, we need the DFA to fall
                        ///  back to its previously accepted state, if any. If the ATN succeeds,
                        ///  then the ATN does the accept and the DFA simulator that invoked it
                        ///  can simply return the predicted token type.
                        /// </summary>
                    protected:
                        Lexer *const recog;

                        /// <summary>
                        /// The current token's starting index into the character stream.
                        ///  Shared across DFA to ATN simulation in case the ATN fails and the
                        ///  DFA did not have a previous accept state. In this case, we use the
                        ///  ATN-generated exception object.
                        /// </summary>
                        int startIndex;

                        /// <summary>
                        /// line number 1..n within the input </summary>
                        int line;

                        /// <summary>
                        /// The index of the character relative to the beginning of the line 0..n-1 </summary>
                        int charPositionInLine;

                    public:
                        const std::vector<dfa::DFA*> decisionToDFA;
                    protected:
                        int mode;

                        /// <summary>
                        /// Used during DFA/ATN exec to record the most recent accept configuration info </summary>
                        SimState *const prevAccept;

                    public:
                        static int match_calls;

                        LexerATNSimulator(ATN *atn, std::vector<dfa::DFA*> decisionToDFA, PredictionContextCache *sharedContextCache); //this(nullptr, atn, decisionToDFA,sharedContextCache);

                        LexerATNSimulator(Lexer *recog, ATN *atn, std::vector<dfa::DFA*> decisionToDFA, PredictionContextCache *sharedContextCache);

                        virtual void copyState(LexerATNSimulator *simulator);

                        virtual int match(CharStream *input, int mode);

                        virtual void reset() override;

                    protected:
                        virtual int matchATN(CharStream *input);

                        virtual int execATN(CharStream *input, dfa::DFAState *ds0);

                        /// <summary>
                        /// Get an existing target state for an edge in the DFA. If the target state
                        /// for the edge has not yet been computed or is otherwise not available,
                        /// this method returns {@code null}.
                        /// </summary>
                        /// <param name="s"> The current DFA state </param>
                        /// <param name="t"> The next input symbol </param>
                        /// <returns> The existing target DFA state for the given input symbol
                        /// {@code t}, or {@code null} if the target state for this edge is not
                        /// already cached </returns>
                        virtual dfa::DFAState *getExistingTargetState(dfa::DFAState *s, int t);

                        /// <summary>
                        /// Compute a target state for an edge in the DFA, and attempt to add the
                        /// computed state and corresponding edge to the DFA.
                        /// </summary>
                        /// <param name="input"> The input stream </param>
                        /// <param name="s"> The current DFA state </param>
                        /// <param name="t"> The next input symbol
                        /// </param>
                        /// <returns> The computed target DFA state for the given input symbol
                        /// {@code t}. If {@code t} does not lead to a valid DFA state, this method
                        /// returns <seealso cref="#ERROR"/>. </returns>
                        virtual dfa::DFAState *computeTargetState(CharStream *input, dfa::DFAState *s, int t);

                        virtual int failOrAccept(SimState *prevAccept, CharStream *input, ATNConfigSet *reach, int t);

                        /// <summary>
                        /// Given a starting configuration set, figure out all ATN configurations
                        ///  we can reach upon input {@code t}. Parameter {@code reach} is a return
                        ///  parameter.
                        /// </summary>
                        virtual void getReachableConfigSet(CharStream *input, ATNConfigSet *closure, ATNConfigSet *reach, int t);

                        virtual void accept(CharStream *input, int ruleIndex, int actionIndex, int index, int line, int charPos);

                        virtual ATNState *getReachableTarget(Transition *trans, int t);

                        virtual atn::ATNConfigSet *computeStartState(CharStream *input, ATNState *p);

                        /// <summary>
                        /// Since the alternatives within any lexer decision are ordered by
                        /// preference, this method stops pursuing the closure as soon as an accept
                        /// state is reached. After the first accept state is reached by depth-first
                        /// search from {@code config}, all other (potentially reachable) states for
                        /// this rule would have a lower priority.
                        /// </summary>
                        /// <returns> {@code true} if an accept state is reached, otherwise
                        /// {@code false}. </returns>
                        virtual bool closure(CharStream *input, LexerATNConfig *config, ATNConfigSet *configs, bool currentAltReachedAcceptState, bool speculative);

                        // side-effect: can alter configs.hasSemanticContext
                        virtual LexerATNConfig *getEpsilonTarget(CharStream *input, LexerATNConfig *config, Transition *t, ATNConfigSet *configs, bool speculative);

                        /// <summary>
                        /// Evaluate a predicate specified in the lexer.
                        /// <p/>
                        /// If {@code speculative} is {@code true}, this method was called before
                        /// <seealso cref="#consume"/> for the matched character. This method should call
                        /// <seealso cref="#consume"/> before evaluating the predicate to ensure position
                        /// sensitive values, including <seealso cref="Lexer#getText"/>, <seealso cref="Lexer#getLine"/>,
                        /// and <seealso cref="Lexer#getCharPositionInLine"/>, properly reflect the current
                        /// lexer state. This method should restore {@code input} and the simulator
                        /// to the original state before returning (i.e. undo the actions made by the
                        /// call to <seealso cref="#consume"/>.
                        /// </summary>
                        /// <param name="input"> The input stream. </param>
                        /// <param name="ruleIndex"> The rule containing the predicate. </param>
                        /// <param name="predIndex"> The index of the predicate within the rule. </param>
                        /// <param name="speculative"> {@code true} if the current index in {@code input} is
                        /// one character before the predicate's location.
                        /// </param>
                        /// <returns> {@code true} if the specified predicate evaluates to
                        /// {@code true}. </returns>
                        virtual bool evaluatePredicate(CharStream *input, int ruleIndex, int predIndex, bool speculative);

                        virtual void captureSimState(SimState *settings, CharStream *input, dfa::DFAState *dfaState);

                        virtual dfa::DFAState *addDFAEdge(dfa::DFAState *from, int t, ATNConfigSet *q);

                        virtual void addDFAEdge(dfa::DFAState *p, int t, dfa::DFAState *q);

                        /// <summary>
                        /// Add a new DFA state if there isn't one with this set of
                        /// configurations already. This method also detects the first
                        /// configuration containing an ATN rule stop state. Later, when
                        /// traversing the DFA, we will know which rule to accept.
                        /// </summary>
                        virtual dfa::DFAState *addDFAState(ATNConfigSet *configs);

                    public:
                        dfa::DFA *getDFA(int mode);

                        /// <summary>
                        /// Get the text matched so far for the current token.
                        /// </summary>
                        virtual std::wstring getText(CharStream *input);

                        virtual int getLine();

                        virtual void setLine(int line);

                        virtual int getCharPositionInLine();

                        virtual void setCharPositionInLine(int charPositionInLine);

                        virtual void consume(CharStream *input);

                        virtual std::wstring getTokenName(int t);

                    private:
                        void InitializeInstanceFields();
                    };

                }
            }
        }
    }
}

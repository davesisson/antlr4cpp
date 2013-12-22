#pragma once

#include "Parser.h"
#include "atn/ATN.h"
#include "dfa/DFA.h"
#include "atn/PredictionContextCache.h"
#include "ParserRuleContext.h"
#include "misc/Pair.h"
#include "TokenStream.h"
#include "atn/ATNState.h"
#include <string>

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


                /// <summary>
                /// A parser simulator that mimics what ANTLR's generated
                ///  parser code does. A ParserATNSimulator is used to make
                ///  predictions via adaptivePredict but this class moves a pointer through the
                ///  ATN to simulate parsing. ParserATNSimulator just
                ///  makes us efficient rather than having to backtrack, for example.
                /// 
                ///  This properly creates parse trees even for left recursive rules.
                /// 
                ///  We rely on the left recursive rule invocation and special predicate
                ///  transitions to make left recursive rules work.
                /// 
                ///  See TestParserInterpreter for examples.
                /// </summary>
                class ParserInterpreter : public Parser {
                protected:
                    const std::wstring grammarFileName;
                    ATN *const atn;
                    BitSet *const pushRecursionContextStates;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final org.antlr.v4.runtime.dfa.DFA[] decisionToDFA;
                    const DFA *decisionToDFA; // not shared like it is for generated parsers
                    PredictionContextCache *const sharedContextCache;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] tokenNames;
                    const std::wstring *tokenNames;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] ruleNames;
                    const std::wstring *ruleNames;

                    Deque<Pair<ParserRuleContext*, int>*> *const _parentContextStack;

                public:
                    ParserInterpreter(const std::wstring &grammarFileName, Collection<std::wstring> *tokenNames, Collection<std::wstring> *ruleNames, ATN *atn, TokenStream *input);

                    virtual ATN *getATN() override;

                    virtual std::wstring *getTokenNames() override;

                    virtual std::wstring *getRuleNames() override;

                    virtual std::wstring getGrammarFileName() override;

                    /// <summary>
                    /// Begin parsing at startRuleIndex </summary>
                    virtual ParserRuleContext *parse(int startRuleIndex);

                    virtual void enterRecursionRule(ParserRuleContext *localctx, int state, int ruleIndex, int precedence) override;

                protected:
                    virtual ATNState *getATNState();

                    virtual void visitState(ATNState *p);

                    virtual void visitRuleStopState(ATNState *p);
                };

            }
        }
    }
}

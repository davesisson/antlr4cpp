#pragma once

#include "Parser.h"
#include "Declarations.h"
#include <string>
#include <bitset>
#include <deque>
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
                    static const int DEFAULT_BITSET_SIZE = 1024;
                    
                    const std::wstring grammarFileName;
                    atn::ATN *const atn;
                    std::bitset<DEFAULT_BITSET_SIZE> *const pushRecursionContextStates;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final org.antlr.v4.runtime.dfa.DFA[] decisionToDFA;
                    const dfa::DFA *decisionToDFA; // not shared like it is for generated parsers
                    atn::PredictionContextCache *const sharedContextCache;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] tokenNames;
                    const std::wstring *tokenNames;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] ruleNames;
                    const std::wstring *ruleNames;

                    std::deque<misc::Pair<ParserRuleContext*, int>*> *const _parentContextStack;

                public:
                    ParserInterpreter(const std::wstring &grammarFileName, std::vector<std::wstring> *tokenNames, std::vector<std::wstring> *ruleNames, atn::ATN *atn, TokenStream *input);

                    virtual atn::ATN *getATN() override;

                    virtual std::wstring *getTokenNames() override;

                    virtual std::wstring *getRuleNames() override;

                    virtual std::wstring getGrammarFileName() override;

                    /// <summary>
                    /// Begin parsing at startRuleIndex </summary>
                    virtual ParserRuleContext *parse(int startRuleIndex);

                    virtual void enterRecursionRule(ParserRuleContext *localctx, int state, int ruleIndex, int precedence) override;

                protected:
                    virtual atn::ATNState *getATNState();

                    virtual void visitState(atn::ATNState *p);

                    virtual void visitRuleStopState(atn::ATNState *p);
                };

            }
        }
    }
}

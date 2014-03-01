#pragma once

#include "Lexer.h"
#include "atn/ATN.h"
#include "dfa/DFA.h"
#include "atn/PredictionContextCache.h"
#include "CharStream.h"
#include <string>

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

                using org::antlr::v4::runtime::CharStream;
                using org::antlr::v4::runtime::Lexer;
                using org::antlr::v4::runtime::atn::ATN;
                using org::antlr::v4::runtime::atn::ATNType;
                using org::antlr::v4::runtime::atn::LexerATNSimulator;
                using org::antlr::v4::runtime::atn::PredictionContextCache;
                using org::antlr::v4::runtime::dfa::DFA;


                class LexerInterpreter : public Lexer {
                protected:
                    const std::wstring grammarFileName;
                    ATN *const atn;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] tokenNames;
                    const std::wstring *tokenNames;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] ruleNames;
                    const std::wstring *ruleNames;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final String[] modeNames;
                    const std::wstring *modeNames;

//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected final org.antlr.v4.runtime.dfa.DFA[] _decisionToDFA;
                    const DFA *_decisionToDFA;
                    PredictionContextCache *const _sharedContextCache;

                public:
                    LexerInterpreter(const std::wstring &grammarFileName, Collection<std::wstring> *tokenNames, Collection<std::wstring> *ruleNames, Collection<std::wstring> *modeNames, ATN *atn, CharStream *input);

                    virtual ATN *getATN() override;

                    virtual std::wstring getGrammarFileName() override;

                    virtual std::wstring *getTokenNames() override;

                    virtual std::wstring *getRuleNames() override;

                    virtual std::wstring *getModeNames() override;
                };

            }
        }
    }
}

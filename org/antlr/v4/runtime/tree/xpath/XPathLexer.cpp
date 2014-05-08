#include "XPathLexer.h"
#include "PredictionContextCache.h"

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

org::antlr::v4::runtime::atn::PredictionContextCache *const XPathLexer::_sharedContextCache = new org::antlr::v4::runtime::atn::PredictionContextCache();
std::wstring XPathLexer::modeNames[1] = {L"DEFAULT_MODE"};
const std::wstring XPathLexer::tokenNames[9] = {L"<INVALID>", L"TOKEN_REF", L"RULE_REF", L"'//'", L"'/'", L"'*'", L"'!'", L"ID", L"STRING"};
const std::wstring XPathLexer::ruleNames[8] = {L"ANYWHERE", L"ROOT", L"WILDCARD", L"BANG", L"ID", L"NameChar", L"NameStartChar", L"STRING"};

XPathLexer::XPathLexer(org::antlr::v4::runtime::CharStream *input) : org::antlr::v4::runtime::Lexer(input) {
#ifdef TODO
    _interp = new org::antlr::v4::runtime::LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
#endif
}

std::wstring XPathLexer::getGrammarFileName() {
    return L"XPathLexer.g4";
}

#ifdef TODO
// This will all be redone when generation is working
std::vector<std::wstring>  * XPathLexer::getTokenNames() {
    return tokenNames;
}

std::vector<std::wstring> * XPathLexer::getRuleNames() {
    return ruleNames;
}
#endif

std::wstring *XPathLexer::getModeNames() {
    return modeNames;
}

org::antlr::v4::runtime::atn::ATN *XPathLexer::getATN() {
    return _ATN;
}

void XPathLexer::action(org::antlr::v4::runtime::RuleContext *_localctx, int ruleIndex, int actionIndex) {
    switch (ruleIndex) {
    case 4:
        ID_action(static_cast<org::antlr::v4::runtime::RuleContext*>(_localctx), actionIndex);
        break;
    }
}

void XPathLexer::ID_action(org::antlr::v4::runtime::RuleContext *_localctx, int actionIndex) {
    switch (actionIndex) {
    case 0:
                    std::wstring text = getText();
                    if (isupper(text[0])) {
                        setType(TOKEN_REF);
                    } else {
                        setType(RULE_REF);
                    }
                     break;
    }
}
#ifdef TODO
const std::wstring XPathLexer::_serializedATN = std::wstring(L"\3\uacf5\uee8c\u4f5d\u8b0d\u4a45\u78bd\u1b2f\u3378\2\n\64\b\1\4\2\t\2") + std::wstring(L"\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\3\2\3\2\3\2\3") + std::wstring(L"\3\3\3\3\4\3\4\3\5\3\5\3\6\3\6\7\6\37\n\6\f\6\16\6\"\13\6\3\6\3\6\3\7") + std::wstring(L"\3\7\5\7(\n\7\3\b\3\b\3\t\3\t\7\t.\n\t\f\t\16\t\61\13\t\3\t\3\t\3/\n\3") + std::wstring(L"\5\1\5\6\1\7\7\1\t\b\1\13\t\2\r\2\1\17\2\1\21\n\1\3\2\4\7\2\62;aa\u00b9") + std::wstring(L"\u00b9\u0302\u0371\u2041\u2042\17\2C\\c|\u00c2\u00d8\u00da\u00f8\u00fa") + std::wstring(L"\u0301\u0372\u037f\u0381\u2001\u200e\u200f\u2072\u2191\u2c02\u2ff1\u3003") + std::wstring(L"\ud801\uf902\ufdd1\ufdf2\uffff\64\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2") + std::wstring(L"\2\t\3\2\2\2\2\13\3\2\2\2\2\21\3\2\2\2\3\23\3\2\2\2\5\26\3\2\2\2\7\30") + std::wstring(L"\3\2\2\2\t\32\3\2\2\2\13\34\3\2\2\2\r\'\3\2\2\2\17)\3\2\2\2\21+\3\2\2") + std::wstring(L"\2\23\24\7\61\2\2\24\25\7\61\2\2\25\4\3\2\2\2\26\27\7\61\2\2\27\6\3\2") + std::wstring(L"\2\2\30\31\7,\2\2\31\b\3\2\2\2\32\33\7#\2\2\33\n\3\2\2\2\34 \5\17\b\2") + std::wstring(L"\35\37\5\r\7\2\36\35\3\2\2\2\37\"\3\2\2\2 \36\3\2\2\2 !\3\2\2\2!#\3\2") + std::wstring(L"\2\2\" \3\2\2\2#$\b\6\2\2$\f\3\2\2\2%(\5\17\b\2&(\t\2\2\2\'%\3\2\2\2\'") + std::wstring(L"&\3\2\2\2(\16\3\2\2\2)*\t\3\2\2*\20\3\2\2\2+/\7)\2\2,.\13\2\2\2-,\3\2") + std::wstring(L"\2\2.\61\3\2\2\2/\60\3\2\2\2/-\3\2\2\2\60\62\3\2\2\2\61/\3\2\2\2\62\63") + std::wstring(L"\7)\2\2\63\22\3\2\2\2\6\2 \'/");

org::antlr::v4::runtime::atn::ATN *const XPathLexer::_ATN = org::antlr::v4::runtime::atn::ATNSimulator::deserialize(_serializedATN.toCharArray());

//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not allowed in native C++:
XPathLexer::XPathLexer() {
    _decisionToDFA = new DFA[_ATN->getNumberOfDecisions()];
    for (int i = 0; i < _ATN->getNumberOfDecisions(); i++) {
        _decisionToDFA[i] = new DFA(_ATN->getDecisionState(i), i);
    }
}

#endif
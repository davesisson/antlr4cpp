#pragma once

#include <string>
#include <cctype>
#include "Lexer.h"
#include "Declarations.h"

// Generated from XPathLexer.g4 by ANTLR 4.1


class XPathLexer : public org::antlr::v4::runtime::Lexer {
protected:
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected static final org.antlr.v4.runtime.dfa.DFA[] _decisionToDFA;
    static const org::antlr::v4::runtime::dfa::DFA *_decisionToDFA;
    static org::antlr::v4::runtime::atn::PredictionContextCache *const _sharedContextCache;
public:
    static const int TOKEN_REF = 1, RULE_REF = 2, ANYWHERE = 3, ROOT = 4, WILDCARD = 5, BANG = 6, ID = 7, STRING = 8;
    static std::wstring modeNames[1];

    static const std::wstring tokenNames[9];
    static const std::wstring ruleNames[8];


    XPathLexer(org::antlr::v4::runtime::CharStream *input);

    virtual std::wstring getGrammarFileName() override;
    virtual std::wstring *getTokenNames() override;
    virtual std::wstring *getRuleNames() override;
    virtual std::wstring *getModeNames() override;
    virtual org::antlr::v4::runtime::atn::ATN *getATN() override;
    virtual void action(org::antlr::v4::runtime::RuleContext *_localctx, int ruleIndex, int actionIndex) override;
private:
    void ID_action(org::antlr::v4::runtime::RuleContext *_localctx, int actionIndex);

public:
    static const std::wstring _serializedATN;
    static org::antlr::v4::runtime::atn::ATN *const _ATN;
//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not allowed in native C++:
    XPathLexer();
};

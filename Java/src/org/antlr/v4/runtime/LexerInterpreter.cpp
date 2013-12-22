#include "LexerInterpreter.h"
#include "Java/src/org/antlr/v4/runtime/atn/ATNType.h"
#include "Java/src/org/antlr/v4/runtime/atn/LexerATNSimulator.h"

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

                LexerInterpreter::LexerInterpreter(const std::wstring &grammarFileName, Collection<std::wstring> *tokenNames, Collection<std::wstring> *ruleNames, Collection<std::wstring> *modeNames, ATN *atn, CharStream *input) : org::antlr::v4::runtime::Lexer(input), grammarFileName(grammarFileName), atn(atn), tokenNames(tokenNames->toArray(new std::wstring[tokenNames->size()])), ruleNames(ruleNames->toArray(new std::wstring[ruleNames->size()])), modeNames(modeNames->toArray(new std::wstring[modeNames->size()])), _decisionToDFA(new DFA[atn->getNumberOfDecisions()]), _sharedContextCache(new org::antlr::v4::runtime->atn->PredictionContextCache()) {

                    if (atn->grammarType != org::antlr::v4::runtime->atn->ATNType->LEXER) {
                        throw IllegalArgumentException(L"The ATN must be a lexer ATN.");
                    }


                    for (int i = 0; i < _decisionToDFA->length; i++) {
                        _decisionToDFA[i] = new DFA(atn->getDecisionState(i), i);
                    }
                    this->_interp = new org::antlr::v4::runtime->atn->LexerATNSimulator(atn,_decisionToDFA,_sharedContextCache);
                }

                org::antlr::v4::runtime::atn::ATN *LexerInterpreter::getATN() {
                    return atn;
                }

                std::wstring LexerInterpreter::getGrammarFileName() {
                    return grammarFileName;
                }

                std::wstring *LexerInterpreter::getTokenNames() {
                    return tokenNames;
                }

                std::wstring *LexerInterpreter::getRuleNames() {
                    return ruleNames;
                }

                std::wstring *LexerInterpreter::getModeNames() {
                    return modeNames;
                }
            }
        }
    }
}

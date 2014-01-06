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

#include "Recognizer.h"
#include "ConsoleErrorListener.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                
                
//                template<typename Symbol, typename ATNInterpreter>
                static std::map<std::wstring[] , std::map<std::wstring, int>*> *const
                Recognizer::tokenTypeMapCache = new std::map<std::wstring[] , std::map<std::wstring, int>*>();
                
//                template<typename Symbol, typename ATNInterpreter>
                static std::map<std::wstring[] , std::map<std::wstring, int>*> *const Recognizer::ruleIndexMapCache = new std::map<std::wstring[] , std::map<std::wstring, int>*>();
                
                template<typename Symbol, typename ATNInterpreter>
                Recognizer<Symbol, ATNInterpreter>::CopyOnWriteArrayListAnonymousInnerClassHelper::CopyOnWriteArrayListAnonymousInnerClassHelper() {
                }
                
                template<typename Symbol, typename ATNInterpreter>
                virtual std::map<std::wstring, int> *Recognizer::getTokenTypeMap() {
                    //JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                    //ORIGINAL LINE: String[] tokenNames = getTokenNames();
                    std::wstring *tokenNames = getTokenNames();
                    if (tokenNames == nullptr) {
                        throw L"The current recognizer does not provide a list of token names.";
                    }
                    
                    //JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                    synchronized(tokenTypeMapCache) {
                        Map<std::wstring, int> *result = tokenTypeMapCache->get(tokenNames);
                        if (result == nullptr) {
                            result = Utils::toMap(tokenNames);
                            result->put(L"EOF", Token::_EOF);
                            result = Collections::unmodifiableMap(result);
                            tokenTypeMapCache->put(tokenNames, result);
                        }
                        
                        return result;
                    }
                }
                
                template<typename Symbol, typename ATNInterpreter>
                virtual std::map<std::wstring, int> *Recognizer::getRuleIndexMap() {
                    //JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                    //ORIGINAL LINE: String[] ruleNames = getRuleNames();
                    std::wstring *ruleNames = getRuleNames();
                    if (ruleNames == nullptr) {
                        throw L"The current recognizer does not provide a list of rule names.";
                    }
                    
                    //JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                    synchronized(ruleIndexMapCache) {
                        Map<std::wstring, int> *result = ruleIndexMapCache->get(ruleNames);
                        if (result == nullptr) {
                            result = Collections::unmodifiableMap(Utils::toMap(ruleNames));
                            ruleIndexMapCache->put(ruleNames, result);
                        }
                        
                        return result;
                    }
                }
                
                virtual int Recognizer::getTokenType(const std::wstring &tokenName) {
                    int ttype = getTokenTypeMap()->get(tokenName);
                    if (ttype != nullptr) {
                        return ttype;
                    }
                    return Token::INVALID_TYPE;
                }
                virtual std::wstring Recognizer::getErrorHeader(RecognitionException *e) {
                    int line = e->getOffendingToken()->getLine();
                    int charPositionInLine = e->getOffendingToken()->getCharPositionInLine();
                    return std::wstring(L"line ") + line + std::wstring(L":") + charPositionInLine;
                }
                virtual std::wstring Recognizer::getTokenErrorDisplay(Token *t) {
                    if (t == nullptr) {
                        return L"<no token>";
                    }
                    std::wstring s = t->getText();
                    if (s == L"") {
                        if (t->getType() == Token::_EOF) {
                            s = L"<EOF>";
                        } else {
                            s = std::wstring(L"<") + t->getType() + std::wstring(L">");
                        }
                    }
                    //JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\n",L"\\n");
                    //JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\r",L"\\r");
                    //JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\t",L"\\t");
                    return std::wstring(L"'") + s + std::wstring(L"'");
                }
                virtual void Recognizer::addErrorListener(ANTLRErrorListener *listener) {
                    if (listener == nullptr) {
                        throw L"listener cannot be null.";
                    }
                    
                    _listeners->add(listener);
                }
                virtual void Recognizer::removeErrorListener(ANTLRErrorListener *listener) {
                    _listeners->remove(listener);
                }
                
                virtual void Recognizer::removeErrorListeners() {
                    _listeners->clear();
                }
                virtual ANTLRErrorListener *Recognizer::Recognizer::getErrorListenerDispatch() {
                    return new ProxyErrorListener(getErrorListeners());
                }
                virtual bool Recognizer::sempred(RuleContext *_localctx, int ruleIndex, int actionIndex) {
                    return true;
                }
                
                virtual bool Recognizer::precpred(RuleContext *localctx, int precedence) {
                    return true;
                }
                
                virtual void Recognizer::action(RuleContext *_localctx, int ruleIndex, int actionIndex) {
                }
                
                int Recognizer::getState() {
                    return _stateNumber;
                }
                void Recognizer::setState(int atnState) {
                    //		System.err.println("setState "+atnState);
                    _stateNumber = atnState;
                    //		if ( traceATNStates ) _ctx.trace(atnState);
                }
                void Recognizer::InitializeInstanceFields() {
                    _stateNumber = -1;
                }
                
                Recognizer::Recognizer() {
                    InitializeInstanceFields();
                }
                Recognizer::CopyOnWriteArrayListAnonymousInnerClassHelper::CopyOnWriteArrayListAnonymousInnerClassHelper()
                {
                    add(ConsoleErrorListener::INSTANCE);
                }
                
                
                
            }
        }
    }
}

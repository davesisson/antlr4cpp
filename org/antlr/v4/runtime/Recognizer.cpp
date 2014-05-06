#include "Recognizer.h"
#include "ConsoleErrorListener.h"
#include "Token.h"
#include "StringBuilder.h"
#include "ProxyErrorListener.h"

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
                
                template<typename T1, typename T2>
                std::map<std::wstring[] , std::map<std::wstring, int>*> *const
                Recognizer<T1, T2>::tokenTypeMapCache = new std::map<std::wstring[] , std::map<std::wstring, int>*>();
                
               template<typename T1, typename T2>
               std::map<std::wstring[] , std::map<std::wstring, int>*> *const Recognizer<T1, T2>::ruleIndexMapCache = new std::map<std::wstring[] , std::map<std::wstring, int>*>();
                
               template<typename T1, typename T2>
                Recognizer<T1, T2>::CopyOnWriteArrayListAnonymousInnerClassHelper::CopyOnWriteArrayListAnonymousInnerClassHelper()
                {
                }
                
               template<typename T1, typename T2>
                std::map<std::wstring, int> *Recognizer<T1, T2>::getTokenTypeMap() {
                    //JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                    //ORIGINAL LINE: String[] tokenNames = getTokenNames();
                    std::wstring *tokenNames = getTokenNames();
                    if (tokenNames == nullptr) {
                        throw L"The current recognizer does not provide a list of token names.";
                    }
#ifdef TODO
                    //JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                    //synchronized(tokenTypeMapCache) {
                        std::map<std::wstring, int> *result = tokenTypeMapCache->get(tokenNames);
                        if (result == nullptr) {
                            result = misc::Utils::toMap(tokenNames);
                            result->put(L"EOF", Token::_EOF);
                            result = std::vector::unmodifiableMap(result);
                            tokenTypeMapCache->put(tokenNames, result);
                        }

                        return result;
                    //}
#endif
                }

                template<typename T1, typename T2>
                std::map<std::wstring, int> *Recognizer<T1, T2>::getRuleIndexMap() {
                    //JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                    //ORIGINAL LINE: String[] ruleNames = getRuleNames();
                    std::wstring *ruleNames = getRuleNames();
                    if (ruleNames == nullptr) {
                        throw L"The current recognizer does not provide a list of rule names.";
                    }
                    
                    //JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                    //synchronized(ruleIndexMapCache) {
                    std::map<std::wstring, int> *result = ruleIndexMapCache->at(*ruleNames);
                    
                        if (result == nullptr) {
                            result = Collections::unmodifiableMap(Utils::toMap(ruleNames));
                            ruleIndexMapCache->put(ruleNames, result);
                        }
                        
                        return result;
                    //}
                }

                template<typename T1, typename T2>
                int Recognizer<T1, T2>::getTokenType(const std::wstring &tokenName) {
                    int ttype = getTokenTypeMap()->get(tokenName);
                    if (ttype != Token::INVALID_TYPE) {
                        return ttype;
                    }
                    return Token::INVALID_TYPE;
                }
                
                template<typename T1, typename T2>
                std::wstring Recognizer<T1, T2>::getErrorHeader(RecognitionException *e) {
                    int line = e->getOffendingToken()->getLine();
                    int charPositionInLine = e->getOffendingToken()->getCharPositionInLine();
                    return std::wstring(L"line ") + std::to_wstring(line) + std::wstring(L":") + std::to_wstring(charPositionInLine);
                }
                
                template<typename T1, typename T2>
                std::wstring Recognizer<T1, T2>::getTokenErrorDisplay(Token *t) {
                    if (t == nullptr) {
                        return L"<no token>";
                    }
                    std::wstring s = t->getText();
                    if (s == L"") {
                        if (t->getType() == Token::_EOF) {
                            s = L"<EOF>";
                        } else {
                            s = std::wstring(L"<") + std::to_wstring(t->getType()) + std::wstring(L">");
                        }
                    }
                    
                    s = StringBuilder::string_replace(s, L"\n", L"\\n");
                    
                    //JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\r",L"\\r");
                    //JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\t",L"\\t");
                    return std::wstring(L"'") + s + std::wstring(L"'");
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::addErrorListener(ANTLRErrorListener *listener) {
                    if (listener == nullptr) {
                        throw L"listener cannot be null.";
                    }
                    
                    _listeners.insert(listener);
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::removeErrorListener(ANTLRErrorListener *listener) {
                    _listeners.remove(listener);
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::removeErrorListeners() {
                    _listeners.clear();
                }
                
                template<typename T1, typename T2>
                ANTLRErrorListener *Recognizer<T1, T2>::Recognizer::getErrorListenerDispatch() {
                    return new ProxyErrorListener(getErrorListeners());
                }
                
                template<typename T1, typename T2>
                bool Recognizer<T1, T2>::sempred(RuleContext *_localctx, int ruleIndex, int actionIndex) {
                    return true;
                }
                
                
                template<typename T1, typename T2>
                bool Recognizer<T1, T2>::precpred(RuleContext *localctx, int precedence) {
                    return true;
                }
                
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::action(RuleContext *_localctx, int ruleIndex, int actionIndex) {
                }
                
                
                template<typename T1, typename T2>
                int Recognizer<T1, T2>::getState() {
                    return _stateNumber;
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::setState(int atnState) {
                    //		System.err.println("setState "+atnState);
                    _stateNumber = atnState;
                    //		if ( traceATNStates ) _ctx.trace(atnState);
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::InitializeInstanceFields() {
                    _stateNumber = -1;
                }
                
                template<typename T1, typename T2>
                Recognizer<T1, T2>::Recognizer() {
                    InitializeInstanceFields();
                }
                
                template<typename T1, typename T2>
                Recognizer<T1, T2>::CopyOnWriteArrayListAnonymousInnerClassHelper::CopyOnWriteArrayListAnonymousInnerClassHelper()
                {
                    add(ConsoleErrorListener::INSTANCE);
                }
 
                
            }
        }
    }
 
}

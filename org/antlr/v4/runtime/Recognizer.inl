#pragma once


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
                std::map<std::vector<std::wstring>, std::map<std::wstring, int>*> const
                Recognizer<T1, T2>::_tokenTypeMapCache;
                
                template<typename T1, typename T2>
                std::map<std::vector<std::wstring>, std::map<std::wstring, int>*> const
                Recognizer<T1, T2>::_ruleIndexMapCache;
                
                template<typename T1, typename T2>
                std::map<std::wstring, int> *Recognizer<T1, T2>::getTokenTypeMap() {
                    std::vector<std::wstring> tokenNames = getTokenNames();
                    if (tokenNames.empty()) {
                        throw L"The current recognizer does not provide a list of token names.";
                    }


                    {   // mutex lock
                        std::lock_guard<std::mutex> lck(mtx);
                        std::map<std::wstring, int> *result = _tokenTypeMapCache.at(tokenNames);
                        if (result == nullptr) {
                            // From Java - why ? result = misc::Utils::toMap(tokenNames);
                            (*result)[L"EOF"] = Token::_EOF;
                            // From Java - why ? result = std::vector::unmodifiableMap(result);
#ifdef TODO
                            // I'm truly stuck on this - FIXME SOON
                            _tokenTypeMapCache[tokenNames] = result;
#endif
                        }

                        return result;
                    }

                }

                template<typename T1, typename T2>
                std::map<std::wstring, int> *Recognizer<T1, T2>::getRuleIndexMap() {
                    const std::vector<std::wstring>& ruleNames = getRuleNames();
                    if (ruleNames.empty()) {
                        throw L"The current recognizer does not provide a list of rule names.";
                    }

                    {
                        std::lock_guard<std::mutex> lck(mtx);
                        std::map<std::wstring, int> *result = _ruleIndexMapCache.at(ruleNames);
                    
                        if (result == nullptr) {
                            result = Utils::toMap(ruleNames);
                            _ruleIndexMapCache.insert(ruleNames, result);
                        }
                        return result;
                    }
                    return nullptr;
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
                    
                    replaceAll(s, L"\n", L"\\n");
                    
                    replaceAll(s, L"\r",L"\\r");

                    replaceAll(s, L"\t", L"\\t");
                    
                    return std::wstring(L"'") + s + std::wstring(L"'");
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::addErrorListener(ANTLRErrorListener *listener) {
                    if (listener == nullptr) {
                        throw L"listener cannot be null.";
                    }
                    
                    _listeners.insert(_listeners.end(), listener);
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::removeErrorListener(ANTLRErrorListener *listener) {
                    //_listeners.remove(listener); does this work the same way?
                    std::vector<ANTLRErrorListener*>::iterator it;
                    it = std::find(_listeners.begin(), _listeners.end(), listener);
                    _listeners.erase(it);
                }
                
                template<typename T1, typename T2>
                void Recognizer<T1, T2>::removeErrorListeners() {
                    _listeners.clear();
                }
                
                template<typename T1, typename T2>
                ANTLRErrorListener *Recognizer<T1, T2>::getErrorListenerDispatch() {
                    // TODO: This is odd, why do we have to cast here? I think it's a template issue, but it _shouldn't_ matter
                    return (ANTLRErrorListener *)new ProxyErrorListener(getErrorListeners());
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
                    _listeners = std::vector<ANTLRErrorListener*>();
                }
                
#ifdef TODO
                template<typename T1, typename T2>
                Recognizer<T1, T2>::
                CopyOnWriteArrayListAnonymousInnerClassHelper::CopyOnWriteArrayListAnonymousInnerClassHelper()
                {
                    add(&ConsoleErrorListener::INSTANCE)
                }
#endif
                
            }
        }
    }
 
}

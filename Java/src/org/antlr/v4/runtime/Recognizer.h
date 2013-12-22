#pragma once

#include "ANTLRErrorListener.h"
#include "ConsoleErrorListener.h"
#include "misc/Utils.h"
#include "Token.h"
#include "atn/ATN.h"
#include "RecognitionException.h"
#include "ProxyErrorListener.h"
#include "RuleContext.h"
#include "IntStream.h"
#include "TokenFactory.h"
#include <string>
#include <vector>

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

                using org::antlr::v4::runtime::atn::ATN;
                using org::antlr::v4::runtime::atn::ATNSimulator;
                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Nullable;
                using org::antlr::v4::runtime::misc::Utils;


//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                template<typename Symbol, typename ATNInterpreter> where ATNInterpreter : org.antlr.v4.runtime.atn.ATNSimulator
                class Recognizer {
                public:
                    static const int EOF = -1;

                private:
//JAVA TO C++ CONVERTER TODO TASK: Native C++ only allows initialization of static const integral fields in their declarations:
                    static Map<std::wstring[] , Map<std::wstring, int>*> *const tokenTypeMapCache = new WeakHashMap<std::wstring[] , Map<std::wstring, int>*>();
//JAVA TO C++ CONVERTER TODO TASK: Native C++ only allows initialization of static const integral fields in their declarations:
                    static Map<std::wstring[] , Map<std::wstring, int>*> *const ruleIndexMapCache = new WeakHashMap<std::wstring[] , Map<std::wstring, int>*>();

                    std::vector<ANTLRErrorListener*> _listeners;

                private:
                    class CopyOnWriteArrayListAnonymousInnerClassHelper : public CopyOnWriteArrayList<ANTLRErrorListener*> {
                    public:
                        CopyOnWriteArrayListAnonymousInnerClassHelper();

                        {
                            add(ConsoleErrorListener::INSTANCE);
                        }
                    };

                protected:
                    ATNInterpreter _interp;

                private:
                    int _stateNumber;

                    /// <summary>
                    /// Used to print out token names like ID during debugging and
                    ///  error reporting.  The generated parsers implement a method
                    ///  that overrides this to point to their String[] tokenNames.
                    /// </summary>
                public:
                    virtual std::wstring *getTokenNames() = 0;

                    virtual std::wstring *getRuleNames() = 0;

                    /// <summary>
                    /// Get a map from token names to token types.
                    /// <p/>
                    /// Used for XPath and tree pattern compilation.
                    /// </summary>
                    virtual Map<std::wstring, int> *getTokenTypeMap() {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: String[] tokenNames = getTokenNames();
                        std::wstring *tokenNames = getTokenNames();
                        if (tokenNames == nullptr) {
                            throw UnsupportedOperationException(L"The current recognizer does not provide a list of token names.");
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(tokenTypeMapCache) {
                            Map<std::wstring, int> *result = tokenTypeMapCache->get(tokenNames);
                            if (result == nullptr) {
                                result = Utils::toMap(tokenNames);
                                result->put(L"EOF", Token::EOF);
                                result = Collections::unmodifiableMap(result);
                                tokenTypeMapCache->put(tokenNames, result);
                            }

                            return result;
                        }
                    }

                    /// <summary>
                    /// Get a map from rule names to rule indexes.
                    /// <p/>
                    /// Used for XPath and tree pattern compilation.
                    /// </summary>
                    virtual Map<std::wstring, int> *getRuleIndexMap() {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: String[] ruleNames = getRuleNames();
                        std::wstring *ruleNames = getRuleNames();
                        if (ruleNames == nullptr) {
                            throw UnsupportedOperationException(L"The current recognizer does not provide a list of rule names.");
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

                    virtual int getTokenType(const std::wstring &tokenName) {
                        int ttype = getTokenTypeMap()->get(tokenName);
                        if (ttype != nullptr) {
                            return ttype;
                        }
                        return Token::INVALID_TYPE;
                    }

                    /// <summary>
                    /// If this recognizer was generated, it will have a serialized ATN
                    /// representation of the grammar.
                    /// <p/>
                    /// For interpreters, we don't know their serialized ATN despite having
                    /// created the interpreter from it.
                    /// </summary>
                    virtual std::wstring getSerializedATN() {
                        throw UnsupportedOperationException(L"there is no serialized ATN");
                    }

                    /// <summary>
                    /// For debugging and other purposes, might want the grammar name.
                    ///  Have ANTLR generate an implementation for this method.
                    /// </summary>
                    virtual std::wstring getGrammarFileName() = 0;

                    /// <summary>
                    /// Get the <seealso cref="ATN"/> used by the recognizer for prediction.
                    /// </summary>
                    /// <returns> The <seealso cref="ATN"/> used by the recognizer for prediction. </returns>
                    virtual ATN *getATN() = 0;

                    /// <summary>
                    /// Get the ATN interpreter used by the recognizer for prediction.
                    /// </summary>
                    /// <returns> The ATN interpreter used by the recognizer for prediction. </returns>
                    virtual ATNInterpreter getInterpreter() {
                        return _interp;
                    }

                    /// <summary>
                    /// Set the ATN interpreter used by the recognizer for prediction.
                    /// </summary>
                    /// <param name="interpreter"> The ATN interpreter used by the recognizer for
                    /// prediction. </param>
                    virtual void setInterpreter(ATNInterpreter interpreter) {
                        _interp = interpreter;
                    }

                    /// <summary>
                    /// What is the error header, normally line/character position information? </summary>
                    virtual std::wstring getErrorHeader(RecognitionException *e) {
                        int line = e->getOffendingToken()->getLine();
                        int charPositionInLine = e->getOffendingToken()->getCharPositionInLine();
                        return std::wstring(L"line ") + line + std::wstring(L":") + charPositionInLine;
                    }

                    /// <summary>
                    /// How should a token be displayed in an error message? The default
                    ///  is to display just the text, but during development you might
                    ///  want to have a lot of information spit out.  Override in that case
                    ///  to use t.toString() (which, for CommonToken, dumps everything about
                    ///  the token). This is better than forcing you to override a method in
                    ///  your token objects because you don't have to go modify your lexer
                    ///  so that it creates a new Java type.
                    /// </summary>
                    virtual std::wstring getTokenErrorDisplay(Token *t) {
                        if (t == nullptr) {
                            return L"<no token>";
                        }
                        std::wstring s = t->getText();
                        if (s == L"") {
                            if (t->getType() == Token::EOF) {
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

                    /// <exception cref="NullPointerException"> if {@code listener} is {@code null}. </exception>
                    virtual void addErrorListener(ANTLRErrorListener *listener) {
                        if (listener == nullptr) {
                            throw NullPointerException(L"listener cannot be null.");
                        }

                        _listeners->add(listener);
                    }

                    virtual void removeErrorListener(ANTLRErrorListener *listener) {
                        _listeners->remove(listener);
                    }

                    virtual void removeErrorListeners() {
                        _listeners->clear();
                    }

                    virtual std::vector<? extends ANTLRErrorListener> getErrorListeners() {
                        return _listeners;
                    }

                    virtual ANTLRErrorListener *getErrorListenerDispatch() {
                        return new ProxyErrorListener(getErrorListeners());
                    }

                    // subclass needs to override these if there are sempreds or actions
                    // that the ATN interp needs to execute
                    virtual bool sempred(RuleContext *_localctx, int ruleIndex, int actionIndex) {
                        return true;
                    }

                    virtual bool precpred(RuleContext *localctx, int precedence) {
                        return true;
                    }

                    virtual void action(RuleContext *_localctx, int ruleIndex, int actionIndex) {
                    }

                    int getState() {
                        return _stateNumber;
                    }

                    /// <summary>
                    /// Indicate that the recognizer has changed internal state that is
                    ///  consistent with the ATN state passed in.  This way we always know
                    ///  where we are in the ATN as the parser goes along. The rule
                    ///  context objects form a stack that lets us see the stack of
                    ///  invoking rules. Combine this and we have complete ATN
                    ///  configuration information.
                    /// </summary>
                    void setState(int atnState) {
                //		System.err.println("setState "+atnState);
                        _stateNumber = atnState;
                //		if ( traceATNStates ) _ctx.trace(atnState);
                    }

                    virtual IntStream *getInputStream() = 0;

                    virtual void setInputStream(IntStream *input) = 0;

                    virtual TokenFactory<?> *getTokenFactory() = 0;

                    template<typename T1>
                    virtual void setTokenFactory(TokenFactory<T1> *input) = 0;

                private:
                    void InitializeInstanceFields() {
                        _stateNumber = -1;
                    }

public:
                    Recognizer() {
                        InitializeInstanceFields();
                    }
                };

            }
        }
    }
}

#include "DefaultErrorStrategy.h"
#include "NoViableAltException.h"

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

                void DefaultErrorStrategy::reset(Parser *recognizer) {
                    endErrorCondition(recognizer);
                }

                void DefaultErrorStrategy::beginErrorCondition(Parser *recognizer) {
                    errorRecoveryMode = true;
                }

                bool DefaultErrorStrategy::inErrorRecoveryMode(Parser *recognizer) {
                    return errorRecoveryMode;
                }

                void DefaultErrorStrategy::endErrorCondition(Parser *recognizer) {
                    errorRecoveryMode = false;
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                    delete lastErrorStates;
                    lastErrorIndex = -1;
                }

                void DefaultErrorStrategy::reportMatch(Parser *recognizer) {
                    endErrorCondition(recognizer);
                }

                void DefaultErrorStrategy::reportError(Parser *recognizer, RecognitionException *e) {
                    // if we've already reported an error and have not matched a token
                    // yet successfully, don't report any errors.
                    if (inErrorRecoveryMode(recognizer)) {
                                //			System.err.print("[SPURIOUS] ");
                        return; // don't report spurious errors
                    }
                    beginErrorCondition(recognizer);
                    if (dynamic_cast<NoViableAltException*>(e) != nullptr) {
                        reportNoViableAlternative(recognizer, static_cast<NoViableAltException*>(e));
                    } else if (dynamic_cast<InputMismatchException*>(e) != nullptr) {
                        reportInputMismatch(recognizer, static_cast<InputMismatchException*>(e));
                    } else if (dynamic_cast<FailedPredicateException*>(e) != nullptr) {
                        reportFailedPredicate(recognizer, static_cast<FailedPredicateException*>(e));
                    } else {
                        System::err::println(std::wstring(L"unknown recognition error type: ") + e->getClass()->getName());
                        recognizer->notifyErrorListeners(e->getOffendingToken(), e->what(), e);
                    }
                }

                void DefaultErrorStrategy::recover(Parser *recognizer, RecognitionException *e) {
                                //		System.out.println("recover in "+recognizer.getRuleInvocationStack()+
                                //						   " index="+recognizer.getInputStream().index()+
                                //						   ", lastErrorIndex="+
                                //						   lastErrorIndex+
                                //						   ", states="+lastErrorStates);
                    if (lastErrorIndex == recognizer->getInputStream()->index() && lastErrorStates != nullptr && lastErrorStates->contains(recognizer->getState())) {
                        // uh oh, another error at same token index and previously-visited
                        // state in ATN; must be a case where LT(1) is in the recovery
                        // token set so nothing got consumed. Consume a single token
                        // at least to prevent an infinite loop; this is a failsafe.
                                //			System.err.println("seen error condition before index="+
                                //							   lastErrorIndex+", states="+lastErrorStates);
                                //			System.err.println("FAILSAFE consumes "+recognizer.getTokenNames()[recognizer.getInputStream().LA(1)]);
                        recognizer->consume();
                    }
                    lastErrorIndex = recognizer->getInputStream()->index();
                    if (lastErrorStates == nullptr) {
                        lastErrorStates = new IntervalSet();
                    }
                    lastErrorStates->add(recognizer->getState());
                    IntervalSet *followSet = getErrorRecoverySet(recognizer);
                    consumeUntil(recognizer, followSet);
                }

                void DefaultErrorStrategy::sync(Parser *recognizer) throw(RecognitionException) {
                    ATNState *s = recognizer->getInterpreter()->atn->states[recognizer->getState()];
                                //		System.err.println("sync @ "+s.stateNumber+"="+s.getClass().getSimpleName());
                    // If already recovering, don't try to sync
                    if (inErrorRecoveryMode(recognizer)) {
                        return;
                    }

                    TokenStream *tokens = recognizer->getInputStream();
                    int la = tokens->LA(1);

                    // try cheaper subset first; might get lucky. seems to shave a wee bit off
                    if (recognizer->getATN()->nextTokens(s)->contains(la) || la == Token::EOF) {
                        return;
                    }

                    // Return but don't end recovery. only do that upon valid token match
                    if (recognizer->isExpectedToken(la)) {
                        return;
                    }

                    switch (s->getStateType()) {
                    case ATNState::BLOCK_START:
                    case ATNState::STAR_BLOCK_START:
                    case ATNState::PLUS_BLOCK_START:
                    case ATNState::STAR_LOOP_ENTRY:
                        // report error and recover if possible
                        if (singleTokenDeletion(recognizer) != nullptr) {
                            return;
                        }

                        throw InputMismatchException(recognizer);

                    case ATNState::PLUS_LOOP_BACK:
                    case ATNState::STAR_LOOP_BACK:
                                //			System.err.println("at loop back: "+s.getClass().getSimpleName());
                        reportUnwantedToken(recognizer);
                        IntervalSet *expecting = recognizer->getExpectedTokens();
                        IntervalSet *whatFollowsLoopIterationOrRule = expecting->or(getErrorRecoverySet(recognizer));
                        consumeUntil(recognizer, whatFollowsLoopIterationOrRule);
                        break;

                    default:
                        // do nothing if we can't identify the exact kind of ATN state
                        break;
                    }
                }

                void DefaultErrorStrategy::reportNoViableAlternative(Parser *recognizer, NoViableAltException *e) {
                    TokenStream *tokens = recognizer->getInputStream();
                    std::wstring input;
                    if (tokens != nullptr) {
                        if (e->getStartToken()->getType() == Token::EOF) {
                            input = L"<EOF>";
                        } else {
                            input = tokens->getText(e->getStartToken(), e->getOffendingToken());
                        }
                    } else {
                        input = L"<unknown input>";
                    }
                    std::wstring msg = std::wstring(L"no viable alternative at input ") + escapeWSAndQuote(input);
                    recognizer->notifyErrorListeners(e->getOffendingToken(), msg, e);
                }

                void DefaultErrorStrategy::reportInputMismatch(Parser *recognizer, InputMismatchException *e) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    std::wstring msg = std::wstring(L"mismatched input ") + getTokenErrorDisplay(e->getOffendingToken()) + std::wstring(L" expecting ") + e->getExpectedTokens()->toString(recognizer->getTokenNames());
                    recognizer->notifyErrorListeners(e->getOffendingToken(), msg, e);
                }

                void DefaultErrorStrategy::reportFailedPredicate(Parser *recognizer, FailedPredicateException *e) {
                    std::wstring ruleName = recognizer->getRuleNames()[recognizer->_ctx->getRuleIndex()];
                    std::wstring msg = std::wstring(L"rule ") + ruleName + std::wstring(L" ") + e->getMessage();
                    recognizer->notifyErrorListeners(e->getOffendingToken(), msg, e);
                }

                void DefaultErrorStrategy::reportUnwantedToken(Parser *recognizer) {
                    if (inErrorRecoveryMode(recognizer)) {
                        return;
                    }

                    beginErrorCondition(recognizer);

                    Token *t = recognizer->getCurrentToken();
                    std::wstring tokenName = getTokenErrorDisplay(t);
                    IntervalSet *expecting = getExpectedTokens(recognizer);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    std::wstring msg = std::wstring(L"extraneous input ") + tokenName + std::wstring(L" expecting ") + expecting->toString(recognizer->getTokenNames());
                    recognizer->notifyErrorListeners(t, msg, nullptr);
                }

                void DefaultErrorStrategy::reportMissingToken(Parser *recognizer) {
                    if (inErrorRecoveryMode(recognizer)) {
                        return;
                    }

                    beginErrorCondition(recognizer);

                    Token *t = recognizer->getCurrentToken();
                    IntervalSet *expecting = getExpectedTokens(recognizer);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    std::wstring msg = std::wstring(L"missing ") + expecting->toString(recognizer->getTokenNames()) + std::wstring(L" at ") + getTokenErrorDisplay(t);

                    recognizer->notifyErrorListeners(t, msg, nullptr);
                }

                org::antlr::v4::runtime::Token *DefaultErrorStrategy::recoverInline(Parser *recognizer) throw(RecognitionException) {
                    // SINGLE TOKEN DELETION
                    Token *matchedSymbol = singleTokenDeletion(recognizer);
                    if (matchedSymbol != nullptr) {
                        // we have deleted the extra token.
                        // now, move past ttype token as if all were ok
                        recognizer->consume();
                        return matchedSymbol;
                    }

                    // SINGLE TOKEN INSERTION
                    if (singleTokenInsertion(recognizer)) {
                        return getMissingSymbol(recognizer);
                    }

                    // even that didn't work; must throw the exception
                    throw InputMismatchException(recognizer);
                }

                bool DefaultErrorStrategy::singleTokenInsertion(Parser *recognizer) {
                    int currentSymbolType = recognizer->getInputStream()->LA(1);
                    // if current token is consistent with what could come after current
                    // ATN state, then we know we're missing a token; error recovery
                    // is free to conjure up and insert the missing token
                    ATNState *currentState = recognizer->getInterpreter()->atn->states[recognizer->getState()];
                    ATNState *next = currentState->transition(0)->target;
                    ATN *atn = recognizer->getInterpreter()->atn;
                    IntervalSet *expectingAtLL2 = atn->nextTokens(next, recognizer->_ctx);
                                //		System.out.println("LT(2) set="+expectingAtLL2.toString(recognizer.getTokenNames()));
                    if (expectingAtLL2->contains(currentSymbolType)) {
                        reportMissingToken(recognizer);
                        return true;
                    }
                    return false;
                }

                org::antlr::v4::runtime::Token *DefaultErrorStrategy::singleTokenDeletion(Parser *recognizer) {
                    int nextTokenType = recognizer->getInputStream()->LA(2);
                    IntervalSet *expecting = getExpectedTokens(recognizer);
                    if (expecting->contains(nextTokenType)) {
                        reportUnwantedToken(recognizer);
                        /*
                        System.err.println("recoverFromMismatchedToken deleting "+
                        				   ((TokenStream)recognizer.getInputStream()).LT(1)+
                        				   " since "+((TokenStream)recognizer.getInputStream()).LT(2)+
                        				   " is what we want");
                        */
                        recognizer->consume(); // simply delete extra token
                        // we want to return the token we're actually matching
                        Token *matchedSymbol = recognizer->getCurrentToken();
                        reportMatch(recognizer); // we know current token is correct
                        return matchedSymbol;
                    }
                    return nullptr;
                }

                org::antlr::v4::runtime::Token *DefaultErrorStrategy::getMissingSymbol(Parser *recognizer) {
                    Token *currentSymbol = recognizer->getCurrentToken();
                    IntervalSet *expecting = getExpectedTokens(recognizer);
                    int expectedTokenType = expecting->getMinElement(); // get any element
                    std::wstring tokenText;
                    if (expectedTokenType == Token::EOF) {
                        tokenText = L"<missing EOF>";
                    } else {
                        tokenText = std::wstring(L"<missing ") + recognizer->getTokenNames()[expectedTokenType] + std::wstring(L">");
                    }
                    Token *current = currentSymbol;
                    Token *lookback = recognizer->getInputStream()->LT(-1);
                    if (current->getType() == Token::EOF && lookback != nullptr) {
                        current = lookback;
                    }
                    return recognizer->getTokenFactory()->create(new Pair<TokenSource*, CharStream*>(current->getTokenSource(), current->getTokenSource()->getInputStream()), expectedTokenType, tokenText, Token::DEFAULT_CHANNEL, -1, -1, current->getLine(), current->getCharPositionInLine());
                }

                org::antlr::v4::runtime::misc::IntervalSet *DefaultErrorStrategy::getExpectedTokens(Parser *recognizer) {
                    return recognizer->getExpectedTokens();
                }

                std::wstring DefaultErrorStrategy::getTokenErrorDisplay(Token *t) {
                    if (t == nullptr) {
                        return L"<no token>";
                    }
                    std::wstring s = getSymbolText(t);
                    if (s == L"") {
                        if (getSymbolType(t) == Token::EOF) {
                            s = L"<EOF>";
                        } else {
                            s = std::wstring(L"<") + getSymbolType(t) + std::wstring(L">");
                        }
                    }
                    return escapeWSAndQuote(s);
                }

                std::wstring DefaultErrorStrategy::getSymbolText(Token *symbol) {
                    return symbol->getText();
                }

                int DefaultErrorStrategy::getSymbolType(Token *symbol) {
                    return symbol->getType();
                }

                std::wstring DefaultErrorStrategy::escapeWSAndQuote(const std::wstring &s) {
                                //		if ( s==null ) return s;
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\n",L"\\n");
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\r",L"\\r");
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                    s = s.replace(L"\t",L"\\t");
                    return std::wstring(L"'") + s + std::wstring(L"'");
                }

                org::antlr::v4::runtime::misc::IntervalSet *DefaultErrorStrategy::getErrorRecoverySet(Parser *recognizer) {
                    ATN *atn = recognizer->getInterpreter()->atn;
                    RuleContext *ctx = recognizer->_ctx;
                    IntervalSet *recoverSet = new IntervalSet();
                    while (ctx != nullptr && ctx->invokingState >= 0) {
                        // compute what follows who invoked us
                        ATNState *invokingState = atn->states[ctx->invokingState];
                        RuleTransition *rt = static_cast<RuleTransition*>(invokingState->transition(0));
                        IntervalSet *follow = atn->nextTokens(rt->followState);
                        recoverSet->addAll(follow);
                        ctx = ctx->parent;
                    }
                    recoverSet->remove(Token::EPSILON);
                                //		System.out.println("recover set "+recoverSet.toString(recognizer.getTokenNames()));
                    return recoverSet;
                }

                void DefaultErrorStrategy::consumeUntil(Parser *recognizer, IntervalSet *set) {
                                //		System.err.println("consumeUntil("+set.toString(recognizer.getTokenNames())+")");
                    int ttype = recognizer->getInputStream()->LA(1);
                    while (ttype != Token::EOF && !set->contains(ttype)) {
                        //System.out.println("consume during recover LA(1)="+getTokenNames()[input.LA(1)]);
                                //			recognizer.getInputStream().consume();
                        recognizer->consume();
                        ttype = recognizer->getInputStream()->LA(1);
                    }
                }

                void DefaultErrorStrategy::InitializeInstanceFields() {
                    errorRecoveryMode = false;
                    lastErrorIndex = -1;
                }
            }
        }
    }
}

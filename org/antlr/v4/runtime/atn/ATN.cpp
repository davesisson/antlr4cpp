#include "ATN.h"
#include "LL1Analyzer.h"
#include "Token.h"
#include "RuleTransition.h"

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
                namespace atn {

                    ATN::ATN(ATNType grammarType, int maxTokenType) : grammarType(grammarType), maxTokenType(maxTokenType), states(new java::util::ArrayList<ATNState>()), decisionToState(new java::util::ArrayList<DecisionState>()), modeNameToStartState(new java::util::LinkedHashMap<String, TokensStartState>()), modeToStartState(new java::util::ArrayList<TokensStartState>()) {
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *ATN::nextTokens(ATNState *s, RuleContext *ctx) {
                        LL1Analyzer *anal = new LL1Analyzer(this);
                        IntervalSet *next = anal->LOOK(s, ctx);
                        return next;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *ATN::nextTokens(ATNState *s) {
                        if (s->nextTokenWithinRule != nullptr) {
                            return s->nextTokenWithinRule;
                        }
                        s->nextTokenWithinRule = nextTokens(s, nullptr);
                        s->nextTokenWithinRule->setReadonly(true);
                        return s->nextTokenWithinRule;
                    }

                    void ATN::addState(ATNState *state) {
                        if (state != nullptr) {
                            state->atn = this;
                            state->stateNumber = states.size();
                        }

                        states.push_back(state);
                    }

                    void ATN::removeState(ATNState *state) {
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                        delete states[state->stateNumber]; // just free mem, don't shift states in list
                    }

                    int ATN::defineDecisionState(DecisionState *s) {
                        decisionToState.push_back(s);
                        s->decision = decisionToState.size() - 1;
                        return s->decision;
                    }

                    org::antlr::v4::runtime::atn::DecisionState *ATN::getDecisionState(int decision) {
                        if (!decisionToState.empty()) {
                            return decisionToState[decision];
                        }
                        return nullptr;
                    }

                    int ATN::getNumberOfDecisions() {
                        return decisionToState.size();
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *ATN::getExpectedTokens(int stateNumber, RuleContext *context) {
                        if (stateNumber < 0 || stateNumber >= states.size()) {
                            throw IllegalArgumentException(L"Invalid state number.");
                        }

                        RuleContext *ctx = context;
                        ATNState *s = states[stateNumber];
                        IntervalSet *following = nextTokens(s);
                        if (!following->contains(Token::EPSILON)) {
                            return following;
                        }

                        IntervalSet *expected = new IntervalSet();
                        expected->addAll(following);
                        expected->remove(Token::EPSILON);
                        while (ctx != nullptr && ctx->invokingState >= 0 && following->contains(Token::EPSILON)) {
                            ATNState *invokingState = states[ctx->invokingState];
                            RuleTransition *rt = static_cast<RuleTransition*>(invokingState->transition(0));
                            following = nextTokens(rt->followState);
                            expected->addAll(following);
                            expected->remove(Token::EPSILON);
                            ctx = ctx->parent;
                        }

                        if (following->contains(Token::EPSILON)) {
                            expected->add(Token::EOF);
                        }

                        return expected;
                    }
                }
            }
        }
    }
}

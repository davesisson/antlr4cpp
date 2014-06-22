#include "RecognitionException.h"
#include "ATN.h"

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
                RecognitionException::RecognitionException(Recognizer<T1, T2> *recognizer, IntStream *input, ParserRuleContext *ctx) : recognizer(recognizer), ctx((RuleContext*)ctx), input(input) {
                    InitializeInstanceFields();
                    if (recognizer != nullptr) {
                        this->offendingState = recognizer->getState();
                    }
                }

                template<typename T1, typename T2>
                RecognitionException::RecognitionException(const std::wstring &message, Recognizer<T1, T2> *recognizer, IntStream *input, ParserRuleContext *ctx) : RuntimeException(message), recognizer(recognizer), ctx((RuleContext*)ctx), input(input) {
                    InitializeInstanceFields();
                    if (recognizer != nullptr) {
                        this->offendingState = recognizer->getState();
                    }
                }

                int RecognitionException::getOffendingState() {
                    return offendingState;
                }

                void RecognitionException::setOffendingState(int offendingState) {
                    this->offendingState = offendingState;
                }

                org::antlr::v4::runtime::misc::IntervalSet *RecognitionException::getExpectedTokens() {
                    if (recognizer != nullptr) {
                        return recognizer->getATN()->getExpectedTokens(offendingState, ctx);
                    }

                    return nullptr;
                }

                org::antlr::v4::runtime::RuleContext *RecognitionException::getCtx() {
                    return ctx;
                }

                org::antlr::v4::runtime::IntStream *RecognitionException::getInputStream() {
                    return input;
                }

                org::antlr::v4::runtime::Token *RecognitionException::getOffendingToken() {
                    return offendingToken;
                }

                void RecognitionException::setOffendingToken(Token *offendingToken) {
                    this->offendingToken = offendingToken;
                }

                org::antlr::v4::runtime::Recognizer<void*, void*> *RecognitionException::getRecognizer() {
                    return recognizer;
                }

                void RecognitionException::InitializeInstanceFields() {
                    offendingState = -1;
                }
            }
        }
    }
}

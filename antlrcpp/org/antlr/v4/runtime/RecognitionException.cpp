#include "RecognitionException.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::DecisionState;
                using org::antlr::v4::runtime::misc::IntervalSet;
                using org::antlr::v4::runtime::misc::Nullable;

template<typename T1, typename T1>
                RecognitionException::RecognitionException(Recognizer<T1> *recognizer, IntStream *input, ParserRuleContext *ctx) : recognizer(recognizer), ctx(ctx), input(input) {
                    InitializeInstanceFields();
                    if (recognizer != nullptr) {
                        this->offendingState = recognizer->getState();
                    }
                }

template<typename T1, typename T1>
                RecognitionException::RecognitionException(const std::wstring &message, Recognizer<T1> *recognizer, IntStream *input, ParserRuleContext *ctx) : RuntimeException(message), recognizer(recognizer), ctx(ctx), input(input) {
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

                org::antlr::v4::runtime::Recognizer<?, ?> *RecognitionException::getRecognizer() {
                    return recognizer;
                }

                void RecognitionException::InitializeInstanceFields() {
                    offendingState = -1;
                }
            }
        }
    }
}

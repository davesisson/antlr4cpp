#include "BailErrorStrategy.h"
#include "Java/src/org/antlr/v4/runtime/ParserRuleContext.h"
#include "Java/src/org/antlr/v4/runtime/misc/ParseCancellationException.h"
#include "Java/src/org/antlr/v4/runtime/InputMismatchException.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::ParseCancellationException;

                void BailErrorStrategy::recover(Parser *recognizer, RecognitionException *e) {
                    for (ParserRuleContext context = recognizer->getContext(); context != nullptr; context = context->getParent()) {
                        context->exception = e;
                    }

                    throw ParseCancellationException(e);
                }

                org::antlr::v4::runtime::Token *BailErrorStrategy::recoverInline(Parser *recognizer) throw(RecognitionException) {
                    InputMismatchException *e = new InputMismatchException(recognizer);
                    for (ParserRuleContext context = recognizer->getContext(); context != nullptr; context = context->getParent()) {
                        context->exception = e;
                    }

                    throw ParseCancellationException(e);
                }

                void BailErrorStrategy::sync(Parser *recognizer) {
                }
            }
        }
    }
}

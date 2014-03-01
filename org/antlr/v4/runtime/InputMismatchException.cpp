#include "InputMismatchException.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::NotNull;

                InputMismatchException::InputMismatchException(Parser *recognizer) : RecognitionException(recognizer, recognizer->getInputStream(), recognizer->_ctx) {
                    this->setOffendingToken(recognizer->getCurrentToken());
                }
            }
        }
    }
}

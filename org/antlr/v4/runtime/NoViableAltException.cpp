#include "NoViableAltException.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::ATNConfigSet;
                using org::antlr::v4::runtime::misc::NotNull;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                NoViableAltException::NoViableAltException(Parser *recognizer) :deadEndConfigs(nullptr), startToken(nullptr) {
                }

                NoViableAltException::NoViableAltException(Parser *recognizer, TokenStream *input, Token *startToken, Token *offendingToken, ATNConfigSet *deadEndConfigs, ParserRuleContext *ctx) : RecognitionException(recognizer, input, ctx), deadEndConfigs(deadEndConfigs), startToken(startToken) {
                    this->setOffendingToken(offendingToken);
                }

                org::antlr::v4::runtime::Token *NoViableAltException::getStartToken() {
                    return startToken;
                }

                org::antlr::v4::runtime::atn::ATNConfigSet *NoViableAltException::getDeadEndConfigs() {
                    return deadEndConfigs;
                }
            }
        }
    }
}

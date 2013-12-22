#include "LexerNoViableAltException.h"
#include "Java/src/org/antlr/v4/runtime/misc/Interval.h"
#include "Java/src/org/antlr/v4/runtime/misc/Utils.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::atn::ATNConfigSet;
                using org::antlr::v4::runtime::misc::Interval;
                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Nullable;
                using org::antlr::v4::runtime::misc::Utils;

                LexerNoViableAltException::LexerNoViableAltException(Lexer *lexer, CharStream *input, int startIndex, ATNConfigSet *deadEndConfigs) : RecognitionException(lexer, input, nullptr), startIndex(startIndex), deadEndConfigs(deadEndConfigs) {
                }

                int LexerNoViableAltException::getStartIndex() {
                    return startIndex;
                }

                org::antlr::v4::runtime::atn::ATNConfigSet *LexerNoViableAltException::getDeadEndConfigs() {
                    return deadEndConfigs;
                }

                org::antlr::v4::runtime::CharStream *LexerNoViableAltException::getInputStream() {
                    return static_cast<CharStream*>(RecognitionException::getInputStream());
                }

                std::wstring LexerNoViableAltException::toString() {
                    std::wstring symbol = L"";
                    if (startIndex >= 0 && startIndex < getInputStream()->size()) {
                        symbol = getInputStream()->getText(Interval::of(startIndex,startIndex));
                        symbol = Utils::escapeWhitespace(symbol, false);
                    }

                    return std::wstring::format(Locale::getDefault(), L"%s('%s')", LexerNoViableAltException::typeid::getSimpleName(), symbol);
                }
            }
        }
    }
}

#include "CommonToken.h"
#include "Java/src/org/antlr/v4/runtime/misc/Interval.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Interval;
                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Pair;
org::antlr::v4::runtime::misc::Pair<TokenSource*, CharStream*> *const CommonToken::EMPTY_SOURCE = new org::antlr::v4::runtime::misc::Pair<TokenSource*, CharStream*>(nullptr, nullptr);

                CommonToken::CommonToken(int type) {
                    InitializeInstanceFields();
                    this->type = type;
                }

                CommonToken::CommonToken(Pair<TokenSource*, CharStream*> *source, int type, int channel, int start, int stop) {
                    InitializeInstanceFields();
                    this->source = source;
                    this->type = type;
                    this->channel = channel;
                    this->start = start;
                    this->stop = stop;
                    if (source->a != nullptr) {
                        this->line = source->a->getLine();
                        this->charPositionInLine = source->a->getCharPositionInLine();
                    }
                }

                CommonToken::CommonToken(int type, const std::wstring &text) {
                    InitializeInstanceFields();
                    this->type = type;
                    this->channel = DEFAULT_CHANNEL;
                    this->text = text;
                    this->source = EMPTY_SOURCE;
                }

                CommonToken::CommonToken(Token *oldToken) {
                    InitializeInstanceFields();
                    text = oldToken->getText();
                    type = oldToken->getType();
                    line = oldToken->getLine();
                    index = oldToken->getTokenIndex();
                    charPositionInLine = oldToken->getCharPositionInLine();
                    channel = oldToken->getChannel();
                    start = oldToken->getStartIndex();
                    stop = oldToken->getStopIndex();

                    if (dynamic_cast<CommonToken*>(oldToken) != nullptr) {
                        source = (static_cast<CommonToken*>(oldToken))->source;
                    } else {
                        source = new Pair<TokenSource*, CharStream*>(oldToken->getTokenSource(), oldToken->getInputStream());
                    }
                }

                int CommonToken::getType() {
                    return type;
                }

                void CommonToken::setLine(int line) {
                    this->line = line;
                }

                std::wstring CommonToken::getText() {
                    if (text != L"") {
                        return text;
                    }

                    CharStream *input = getInputStream();
                    if (input == nullptr) {
                        return L"";
                    }
                    int n = input->size();
                    if (start < n && stop < n) {
                        return input->getText(Interval::of(start,stop));
                    } else {
                        return L"<EOF>";
                    }
                }

                void CommonToken::setText(const std::wstring &text) {
                    this->text = text;
                }

                int CommonToken::getLine() {
                    return line;
                }

                int CommonToken::getCharPositionInLine() {
                    return charPositionInLine;
                }

                void CommonToken::setCharPositionInLine(int charPositionInLine) {
                    this->charPositionInLine = charPositionInLine;
                }

                int CommonToken::getChannel() {
                    return channel;
                }

                void CommonToken::setChannel(int channel) {
                    this->channel = channel;
                }

                void CommonToken::setType(int type) {
                    this->type = type;
                }

                int CommonToken::getStartIndex() {
                    return start;
                }

                void CommonToken::setStartIndex(int start) {
                    this->start = start;
                }

                int CommonToken::getStopIndex() {
                    return stop;
                }

                void CommonToken::setStopIndex(int stop) {
                    this->stop = stop;
                }

                int CommonToken::getTokenIndex() {
                    return index;
                }

                void CommonToken::setTokenIndex(int index) {
                    this->index = index;
                }

                org::antlr::v4::runtime::TokenSource *CommonToken::getTokenSource() {
                    return source->a;
                }

                org::antlr::v4::runtime::CharStream *CommonToken::getInputStream() {
                    return source->b;
                }

                std::wstring CommonToken::toString() {
                    std::wstring channelStr = L"";
                    if (channel > 0) {
                        channelStr = std::wstring(L",channel=") + channel;
                    }
                    std::wstring txt = getText();
                    if (txt != L"") {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                        txt = txt.replace(L"\n",L"\\n");
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                        txt = txt.replace(L"\r",L"\\r");
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                        txt = txt.replace(L"\t",L"\\t");
                    } else {
                        txt = L"<no text>";
                    }
                    return std::wstring(L"[@") + getTokenIndex() + std::wstring(L",") + start + std::wstring(L":") + stop + std::wstring(L"='") + txt + std::wstring(L"',<") + type + std::wstring(L">") + channelStr + std::wstring(L",") + line + std::wstring(L":") + getCharPositionInLine() + std::wstring(L"]");
                }

                void CommonToken::InitializeInstanceFields() {
                    type = 0;
                    line = 0;
                    charPositionInLine = -1;
                    channel = DEFAULT_CHANNEL;
                    index = -1;
                    start = 0;
                    stop = 0;
                }
            }
        }
    }
}

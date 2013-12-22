#include "RuleTagToken.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::CharStream;
                        using org::antlr::v4::runtime::Token;
                        using org::antlr::v4::runtime::TokenSource;
                        using org::antlr::v4::runtime::misc::NotNull;
                        using org::antlr::v4::runtime::misc::Nullable;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        RuleTagToken::RuleTagToken(const std::wstring &ruleName, int bypassTokenType) {
                        }

                        RuleTagToken::RuleTagToken(const std::wstring &ruleName, int bypassTokenType, const std::wstring &label) : ruleName(ruleName), bypassTokenType(bypassTokenType), label(label) {
                            if (ruleName == L"" || ruleName.isEmpty()) {
                                throw IllegalArgumentException(L"ruleName cannot be null or empty.");
                            }

                        }

                        std::wstring RuleTagToken::getRuleName() {
                            return ruleName;
                        }

                        std::wstring RuleTagToken::getLabel() {
                            return label;
                        }

                        int RuleTagToken::getChannel() {
                            return DEFAULT_CHANNEL;
                        }

                        std::wstring RuleTagToken::getText() {
                            if (label != L"") {
                                return std::wstring(L"<") + label + std::wstring(L":") + ruleName + std::wstring(L">");
                            }

                            return std::wstring(L"<") + ruleName + std::wstring(L">");
                        }

                        int RuleTagToken::getType() {
                            return bypassTokenType;
                        }

                        int RuleTagToken::getLine() {
                            return 0;
                        }

                        int RuleTagToken::getCharPositionInLine() {
                            return -1;
                        }

                        int RuleTagToken::getTokenIndex() {
                            return -1;
                        }

                        int RuleTagToken::getStartIndex() {
                            return -1;
                        }

                        int RuleTagToken::getStopIndex() {
                            return -1;
                        }

                        org::antlr::v4::runtime::TokenSource *RuleTagToken::getTokenSource() {
                            return nullptr;
                        }

                        org::antlr::v4::runtime::CharStream *RuleTagToken::getInputStream() {
                            return nullptr;
                        }

                        std::wstring RuleTagToken::toString() {
                            return ruleName + std::wstring(L":") + bypassTokenType;
                        }
                    }
                }
            }
        }
    }
}

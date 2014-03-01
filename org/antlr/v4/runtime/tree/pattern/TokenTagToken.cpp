#include "TokenTagToken.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::CommonToken;
                        using org::antlr::v4::runtime::Token;
                        using org::antlr::v4::runtime::misc::NotNull;
                        using org::antlr::v4::runtime::misc::Nullable;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        TokenTagToken::TokenTagToken(const std::wstring &tokenName, int type) {
                        }

                        TokenTagToken::TokenTagToken(const std::wstring &tokenName, int type, const std::wstring &label) : org::antlr::v4::runtime::CommonToken(type), tokenName(tokenName), label(label) {
                        }

                        std::wstring TokenTagToken::getTokenName() {
                            return tokenName;
                        }

                        std::wstring TokenTagToken::getLabel() {
                            return label;
                        }

                        std::wstring TokenTagToken::getText() {
                            if (label != L"") {
                                return std::wstring(L"<") + label + std::wstring(L":") + tokenName + std::wstring(L">");
                            }

                            return std::wstring(L"<") + tokenName + std::wstring(L">");
                        }

                        std::wstring TokenTagToken::toString() {
                            return tokenName + std::wstring(L":") + type;
                        }
                    }
                }
            }
        }
    }
}

#include "TerminalNodeImpl.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    using org::antlr::v4::runtime::Parser;
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::misc::Interval;

                    TerminalNodeImpl::TerminalNodeImpl(Token *symbol) {
                        this->symbol = symbol;
                    }

                    org::antlr::v4::runtime::tree::ParseTree *TerminalNodeImpl::getChild(int i) {
                        return nullptr;
                    }

                    org::antlr::v4::runtime::Token *TerminalNodeImpl::getSymbol() {
                        return symbol;
                    }

                    org::antlr::v4::runtime::tree::ParseTree *TerminalNodeImpl::getParent() {
                        return parent;
                    }

                    org::antlr::v4::runtime::Token *TerminalNodeImpl::getPayload() {
                        return symbol;
                    }

                    org::antlr::v4::runtime::misc::Interval *TerminalNodeImpl::getSourceInterval() {
                        if (symbol == nullptr) {
                            return Interval::INVALID;
                        }

                        int tokenIndex = symbol->getTokenIndex();
                        return new Interval(tokenIndex, tokenIndex);
                    }

                    int TerminalNodeImpl::getChildCount() {
                        return 0;
                    }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T, typename T1> where T1 : T
                    T TerminalNodeImpl::accept(ParseTreeVisitor<T1> *visitor) {
                        return visitor->visitTerminal(this);
                    }

                    std::wstring TerminalNodeImpl::getText() {
                        return symbol->getText();
                    }

                    std::wstring TerminalNodeImpl::toStringTree(Parser *parser) {
                        return toString();
                    }

                    std::wstring TerminalNodeImpl::toString() {
                            if (symbol->getType() == Token::EOF) {
                                return L"<EOF>";
                            }
                            return symbol->getText();
                    }

                    std::wstring TerminalNodeImpl::toStringTree() {
                        return toString();
                    }
                }
            }
        }
    }
}

#include "ListTokenSource.h"
#include "Pair.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Pair;

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T1> where T1 : Token
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                ListTokenSource::ListTokenSource(std::vector<T1> tokens) {
                }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T1> where T1 : Token
                ListTokenSource::ListTokenSource(std::vector<T1> tokens, const std::wstring &sourceName) : tokens(tokens), sourceName(sourceName) {
                    InitializeInstanceFields();
                    if (tokens.empty()) {
                        throw NullPointerException(L"tokens cannot be null");
                    }

                }

                int ListTokenSource::getCharPositionInLine() {
                    if (i < tokens.size()) {
                        return tokens[i]->getCharPositionInLine();
                    } else if (eofToken != nullptr) {
                        return eofToken->getCharPositionInLine();
                    } else if (tokens.size() > 0) {
                        // have to calculate the result from the line/column of the previous
                        // token, along with the text of the token.
                        Token *lastToken = tokens[tokens.size() - 1];
                        std::wstring tokenText = lastToken->getText();
                        if (tokenText != L"") {
                            int lastNewLine = tokenText.rfind(L'\n');
                            if (lastNewLine >= 0) {
                                return tokenText.length() - lastNewLine - 1;
                            }
                        }

                        return lastToken->getCharPositionInLine() + lastToken->getStopIndex() - lastToken->getStartIndex() + 1;
                    }

                    // only reach this if tokens is empty, meaning EOF occurs at the first
                    // position in the input
                    return 0;
                }

                org::antlr::v4::runtime::Token *ListTokenSource::nextToken() {
                    if (i >= tokens.size()) {
                        if (eofToken == nullptr) {
                            int start = -1;
                            if (tokens.size() > 0) {
                                int previousStop = tokens[tokens.size() - 1]->getStopIndex();
                                if (previousStop != -1) {
                                    start = previousStop + 1;
                                }
                            }

                            int stop = std::max(-1, start - 1);
                            eofToken = _factory->create(new Pair<TokenSource*, CharStream*>(this, getInputStream()), Token::EOF, L"EOF", Token::DEFAULT_CHANNEL, start, stop, getLine(), getCharPositionInLine());
                        }

                        return eofToken;
                    }

                    Token *t = tokens[i];
                    if (i == tokens.size() - 1 && t->getType() == Token::EOF) {
                        eofToken = t;
                    }

                    i++;
                    return t;
                }

                int ListTokenSource::getLine() {
                    if (i < tokens.size()) {
                        return tokens[i]->getLine();
                    } else if (eofToken != nullptr) {
                        return eofToken->getLine();
                    } else if (tokens.size() > 0) {
                        // have to calculate the result from the line/column of the previous
                        // token, along with the text of the token.
                        Token *lastToken = tokens[tokens.size() - 1];
                        int line = lastToken->getLine();

                        std::wstring tokenText = lastToken->getText();
                        if (tokenText != L"") {
                            for (int i = 0; i < tokenText.length(); i++) {
                                if (tokenText[i] == L'\n') {
                                    line++;
                                }
                            }
                        }

                        // if no text is available, assume the token did not contain any newline characters.
                        return line;
                    }

                    // only reach this if tokens is empty, meaning EOF occurs at the first
                    // position in the input
                    return 1;
                }

                org::antlr::v4::runtime::CharStream *ListTokenSource::getInputStream() {
                    if (i < tokens.size()) {
                        return tokens[i]->getInputStream();
                    } else if (eofToken != nullptr) {
                        return eofToken->getInputStream();
                    } else if (tokens.size() > 0) {
                        return tokens[tokens.size() - 1]->getInputStream();
                    }

                    // no input stream information is available
                    return nullptr;
                }

                std::wstring ListTokenSource::getSourceName() {
                    if (sourceName != L"") {
                        return sourceName;
                    }

                    CharStream *inputStream = getInputStream();
                    if (inputStream != nullptr) {
                        return inputStream->getSourceName();
                    }

                    return L"List";
                }

template<typename T1>
                void ListTokenSource::setTokenFactory(TokenFactory<T1> *factory) {
                    this->_factory = factory;
                }

                org::antlr::v4::runtime::TokenFactory<?> *ListTokenSource::getTokenFactory() {
                    return _factory;
                }

                void ListTokenSource::InitializeInstanceFields() {
                    i = 0;
                    _factory = CommonTokenFactory::DEFAULT;
                }
            }
        }
    }
}

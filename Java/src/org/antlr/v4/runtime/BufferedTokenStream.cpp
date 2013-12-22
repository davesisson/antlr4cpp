#include "BufferedTokenStream.h"
#include "Java/src/org/antlr/v4/runtime/WritableToken.h"
#include "Java/src/org/antlr/v4/runtime/Lexer.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Interval;
                using org::antlr::v4::runtime::misc::NotNull;

                BufferedTokenStream::BufferedTokenStream(TokenSource *tokenSource) {
                    InitializeInstanceFields();
                    if (tokenSource == nullptr) {
                        throw NullPointerException(L"tokenSource cannot be null");
                    }
                    this->tokenSource = tokenSource;
                }

                org::antlr::v4::runtime::TokenSource *BufferedTokenStream::getTokenSource() {
                    return tokenSource;
                }

                int BufferedTokenStream::index() {
                    return p;
                }

                int BufferedTokenStream::mark() {
                    return 0;
                }

                void BufferedTokenStream::release(int marker) {
                    // no resources to release
                }

                void BufferedTokenStream::reset() {
                    seek(0);
                }

                void BufferedTokenStream::seek(int index) {
                    lazyInit();
                    p = adjustSeekIndex(index);
                }

                int BufferedTokenStream::size() {
                    return tokens.size();
                }

                void BufferedTokenStream::consume() {
                    if (LA(1) == EOF) {
                        throw IllegalStateException(L"cannot consume EOF");
                    }

                    if (sync(p + 1)) {
                        p = adjustSeekIndex(p + 1);
                    }
                }

                bool BufferedTokenStream::sync(int i) {
                    assert(i >= 0);
                    int n = i - tokens.size() + 1; // how many more elements we need?
                    //System.out.println("sync("+i+") needs "+n);
                    if (n > 0) {
                        int fetched = fetch(n);
                        return fetched >= n;
                    }

                    return true;
                }

                int BufferedTokenStream::fetch(int n) {
                    if (fetchedEOF) {
                        return 0;
                    }

                    for (int i = 0; i < n; i++) {
                        Token *t = tokenSource->nextToken();
                        if (dynamic_cast<WritableToken*>(t) != nullptr) {
                            (static_cast<WritableToken*>(t))->setTokenIndex(tokens.size());
                        }
                        tokens.push_back(t);
                        if (t->getType() == Token::EOF) {
                            fetchedEOF = true;
                            return i + 1;
                        }
                    }

                    return n;
                }

                org::antlr::v4::runtime::Token *BufferedTokenStream::get(int i) {
                    if (i < 0 || i >= tokens.size()) {
                        throw IndexOutOfBoundsException(std::wstring(L"token index ") + i + std::wstring(L" out of range 0..") + (tokens.size() - 1));
                    }
                    return tokens[i];
                }

                std::vector<Token*> BufferedTokenStream::get(int start, int stop) {
                    if (start < 0 || stop < 0) {
                        return nullptr;
                    }
                    lazyInit();
                    std::vector<Token*> subset = std::vector<Token*>();
                    if (stop >= tokens.size()) {
                        stop = tokens.size() - 1;
                    }
                    for (int i = start; i <= stop; i++) {
                        Token *t = tokens[i];
                        if (t->getType() == Token::EOF) {
                            break;
                        }
                        subset.push_back(t);
                    }
                    return subset;
                }

                int BufferedTokenStream::LA(int i) {
                    return LT(i)->getType();
                }

                org::antlr::v4::runtime::Token *BufferedTokenStream::LB(int k) {
                    if ((p - k) < 0) {
                        return nullptr;
                    }
                    return tokens[p - k];
                }

                org::antlr::v4::runtime::Token *BufferedTokenStream::LT(int k) {
                    lazyInit();
                    if (k == 0) {
                        return nullptr;
                    }
                    if (k < 0) {
                        return LB(-k);
                    }

                    int i = p + k - 1;
                    sync(i);
                    if (i >= tokens.size()) { // return EOF token
                        // EOF must be last token
                        return tokens[tokens.size() - 1];
                    }
                                //		if ( i>range ) range = i;
                    return tokens[i];
                }

                int BufferedTokenStream::adjustSeekIndex(int i) {
                    return i;
                }

                void BufferedTokenStream::lazyInit() {
                    if (p == -1) {
                        setup();
                    }
                }

                void BufferedTokenStream::setup() {
                    sync(0);
                    p = adjustSeekIndex(0);
                }

                void BufferedTokenStream::setTokenSource(TokenSource *tokenSource) {
                    this->tokenSource = tokenSource;
                    tokens.clear();
                    p = -1;
                }

                std::vector<Token*> BufferedTokenStream::getTokens() {
                    return tokens;
                }

                std::vector<Token*> BufferedTokenStream::getTokens(int start, int stop) {
                    return getTokens(start, stop, nullptr);
                }

                std::vector<Token*> BufferedTokenStream::getTokens(int start, int stop, Set<int> *types) {
                    lazyInit();
                    if (start < 0 || stop >= tokens.size() || stop < 0 || start >= tokens.size()) {
                        throw IndexOutOfBoundsException(std::wstring(L"start ") + start + std::wstring(L" or stop ") + stop + std::wstring(L" not in 0..") + (tokens.size() - 1));
                    }
                    if (start > stop) {
                        return nullptr;
                    }

                    // list = tokens[start:stop]:{T t, t.getType() in types}
                    std::vector<Token*> filteredTokens = std::vector<Token*>();
                    for (int i = start; i <= stop; i++) {
                        Token *t = tokens[i];
                        if (types == nullptr || types->contains(t->getType())) {
                            filteredTokens.push_back(t);
                        }
                    }
                    if (filteredTokens.empty()) {
                        filteredTokens.clear();
                    }
                    return filteredTokens;
                }

                std::vector<Token*> BufferedTokenStream::getTokens(int start, int stop, int ttype) {
                    std::set<int> s = std::set<int>(ttype);
                    s.insert(ttype);
                    return getTokens(start,stop, s);
                }

                int BufferedTokenStream::nextTokenOnChannel(int i, int channel) {
                    sync(i);
                    Token *token = tokens[i];
                    if (i >= size()) {
                        return -1;
                    }
                    while (token->getChannel() != channel) {
                        if (token->getType() == Token::EOF) {
                            return -1;
                        }
                        i++;
                        sync(i);
                        token = tokens[i];
                    }
                    return i;
                }

                int BufferedTokenStream::previousTokenOnChannel(int i, int channel) {
                    while (i >= 0 && tokens[i]->getChannel() != channel) {
                        i--;
                    }
                    return i;
                }

                std::vector<Token*> BufferedTokenStream::getHiddenTokensToRight(int tokenIndex, int channel) {
                    lazyInit();
                    if (tokenIndex < 0 || tokenIndex >= tokens.size()) {
                        throw IndexOutOfBoundsException(tokenIndex + std::wstring(L" not in 0..") + (tokens.size() - 1));
                    }

                    int nextOnChannel = nextTokenOnChannel(tokenIndex + 1, Lexer::DEFAULT_TOKEN_CHANNEL);
                    int to;
                    int from = tokenIndex + 1;
                    // if none onchannel to right, nextOnChannel=-1 so set to = last token
                    if (nextOnChannel == -1) {
                        to = size() - 1;
                    } else {
                        to = nextOnChannel;
                    }

                    return filterForChannel(from, to, channel);
                }

                std::vector<Token*> BufferedTokenStream::getHiddenTokensToRight(int tokenIndex) {
                    return getHiddenTokensToRight(tokenIndex, -1);
                }

                std::vector<Token*> BufferedTokenStream::getHiddenTokensToLeft(int tokenIndex, int channel) {
                    lazyInit();
                    if (tokenIndex < 0 || tokenIndex >= tokens.size()) {
                        throw IndexOutOfBoundsException(tokenIndex + std::wstring(L" not in 0..") + (tokens.size() - 1));
                    }

                    int prevOnChannel = previousTokenOnChannel(tokenIndex - 1, Lexer::DEFAULT_TOKEN_CHANNEL);
                    if (prevOnChannel == tokenIndex - 1) {
                        return nullptr;
                    }
                    // if none onchannel to left, prevOnChannel=-1 then from=0
                    int from = prevOnChannel + 1;
                    int to = tokenIndex - 1;

                    return filterForChannel(from, to, channel);
                }

                std::vector<Token*> BufferedTokenStream::getHiddenTokensToLeft(int tokenIndex) {
                    return getHiddenTokensToLeft(tokenIndex, -1);
                }

                std::vector<Token*> BufferedTokenStream::filterForChannel(int from, int to, int channel) {
                    std::vector<Token*> hidden = std::vector<Token*>();
                    for (int i = from; i <= to; i++) {
                        Token *t = tokens[i];
                        if (channel == -1) {
                            if (t->getChannel() != Lexer::DEFAULT_TOKEN_CHANNEL) {
                                hidden.push_back(t);
                            }
                        } else {
                            if (t->getChannel() == channel) {
                                hidden.push_back(t);
                            }
                        }
                    }
                    if (hidden.empty()) {
                        return nullptr;
                    }
                    return hidden;
                }

                std::wstring BufferedTokenStream::getSourceName()
                /// <summary>
                /// Get the text of all tokens in this buffer. </summary>
                {
                    return tokenSource->getSourceName();
                }

                std::wstring BufferedTokenStream::getText() {
                    lazyInit();
                    fill();
                    return getText(Interval::of(0,size() - 1));
                }

                std::wstring BufferedTokenStream::getText(Interval *interval) {
                    int start = interval->a;
                    int stop = interval->b;
                    if (start < 0 || stop < 0) {
                        return L"";
                    }
                    lazyInit();
                    if (stop >= tokens.size()) {
                        stop = tokens.size() - 1;
                    }

                    StringBuilder *buf = new StringBuilder();
                    for (int i = start; i <= stop; i++) {
                        Token *t = tokens[i];
                        if (t->getType() == Token::EOF) {
                            break;
                        }
                        buf->append(t->getText());
                    }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    return buf->toString();
                }

                std::wstring BufferedTokenStream::getText(RuleContext *ctx) {
                    return getText(ctx->getSourceInterval());
                }

                std::wstring BufferedTokenStream::getText(Token *start, Token *stop) {
                    if (start != nullptr && stop != nullptr) {
                        return getText(Interval::of(start->getTokenIndex(), stop->getTokenIndex()));
                    }

                    return L"";
                }

                void BufferedTokenStream::fill() {
                    lazyInit();
                    const int blockSize = 1000;
                    while (true) {
                        int fetched = fetch(blockSize);
                        if (fetched < blockSize) {
                            return;
                        }
                    }
                }

                void BufferedTokenStream::InitializeInstanceFields() {
                    tokens = VectorHelper::VectorWithReservedSize<Token*>(100);
                    p = -1;
                    fetchedEOF = false;
                }
            }
        }
    }
}

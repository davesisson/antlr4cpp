#include "CommonTokenStream.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {

                CommonTokenStream::CommonTokenStream(TokenSource *tokenSource) : BufferedTokenStream(tokenSource) {
                    InitializeInstanceFields();
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                CommonTokenStream::CommonTokenStream(TokenSource *tokenSource, int channel) {
                    this->channel = channel;
                }

                int CommonTokenStream::adjustSeekIndex(int i) {
                    return nextTokenOnChannel(i, channel);
                }

                org::antlr::v4::runtime::Token *CommonTokenStream::LB(int k) {
                    if (k == 0 || (p - k) < 0) {
                        return nullptr;
                    }

                    int i = p;
                    int n = 1;
                    // find k good tokens looking backwards
                    while (n <= k) {
                        // skip off-channel tokens
                        i = previousTokenOnChannel(i - 1, channel);
                        n++;
                    }
                    if (i < 0) {
                        return nullptr;
                    }
                    return tokens[i];
                }

                org::antlr::v4::runtime::Token *CommonTokenStream::LT(int k) {
                    //System.out.println("enter LT("+k+")");
                    lazyInit();
                    if (k == 0) {
                        return nullptr;
                    }
                    if (k < 0) {
                        return LB(-k);
                    }
                    int i = p;
                    int n = 1; // we know tokens[p] is a good one
                    // find k good tokens
                    while (n < k) {
                        // skip off-channel tokens, but make sure to not look past EOF
                        if (sync(i + 1)) {
                            i = nextTokenOnChannel(i + 1, channel);
                        }
                        n++;
                    }
                                //		if ( i>range ) range = i;
                    return tokens[i];
                }

                int CommonTokenStream::getNumberOfOnChannelTokens() {
                    int n = 0;
                    fill();
                    for (int i = 0; i < tokens.size(); i++) {
                        Token *t = tokens[i];
                        if (t->getChannel() == channel) {
                            n++;
                        }
                        if (t->getType() == Token::EOF) {
                            break;
                        }
                    }
                    return n;
                }

                void CommonTokenStream::InitializeInstanceFields() {
                    channel = Token::DEFAULT_CHANNEL;
                }
            }
        }
    }
}

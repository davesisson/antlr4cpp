﻿#include "CommonTokenStream.h"

/*
 * [The "BSD license"]
 *  Copyright (c) 2013 Terence Parr
 *  Copyright (c) 2013 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {

                CommonTokenStream::CommonTokenStream(TokenSource *tokenSource) : BufferedTokenStream(tokenSource) {
                    InitializeInstanceFields();
                }

                CommonTokenStream::CommonTokenStream(TokenSource *tokenSource, int channel)
                : BufferedTokenStream(tokenSource)
                {
                    this->channel = channel;
                }

                int CommonTokenStream::adjustSeekIndex(int i) {
                    return nextTokenOnChannel(i, channel);
                }

                Token *CommonTokenStream::LB(int k) {
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

                Token *CommonTokenStream::LT(int k) {
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
                    for (size_t i = 0; i < tokens.size(); i++) {
                        Token *t = tokens[i];
                        if (t->getChannel() == channel) {
                            n++;
                        }
                        if (t->getType() == Token::_EOF) {
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

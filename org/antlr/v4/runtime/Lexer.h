﻿#pragma once

#include "Declarations.h"
#include "Recognizer.h"
#include "Token.h"
#include "TokenSource.h"
#include "stringconverter.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
                /// <summary>
                /// A lexer is recognizer that draws input symbols from a character stream.
                ///  lexer grammars result in a subclass of this object. A Lexer object
                ///  uses simplified match() and error recovery mechanisms in the interest
                ///  of speed.
                /// </summary>
                class Lexer : public Recognizer<int, atn::LexerATNSimulator*>, public TokenSource {
                public:
                    static const int DEFAULT_MODE = 0;
                    static const int MORE = -2;
                    static const int SKIP = -3;

                    static const int DEFAULT_TOKEN_CHANNEL = Token::DEFAULT_CHANNEL;
                    static const int HIDDEN = Token::HIDDEN_CHANNEL;
                    static const wchar_t MIN_CHAR_VALUE = L'\0';
                    static const wchar_t MAX_CHAR_VALUE = L'\uFFFE';

                    CharStream *_input;
                protected:
                    std::pair<TokenSource*, CharStream*> *_tokenFactorySourcePair;
                    
                    std::vector<std::wstring> _modeNames;
                    std::vector<std::wstring> _tokenNames;

                    /// <summary>
                    /// How to create token objects </summary>
                    TokenFactory<CommonToken*> *_factory;

                    /// <summary>
                    /// The goal of all lexer rules/methods is to create a token object.
                    ///  This is an instance variable as multiple rules may collaborate to
                    ///  create a single token.  nextToken will return this object after
                    ///  matching lexer rule(s).  If you subclass to allow multiple token
                    ///  emissions, then set this to the last token to be matched or
                    ///  something nonnull so that the auto token emit mechanism will not
                    ///  emit another token.
                    /// </summary>
                public:
                    Token *_token;

                    /// <summary>
                    /// What character index in the stream did the current token start at?
                    ///  Needed, for example, to get the text for current token.  Set at
                    ///  the start of nextToken.
                    /// </summary>
                    int _tokenStartCharIndex;

                    /// <summary>
                    /// The line on which the first character of the token resides </summary>
                    int _tokenStartLine;

                    /// <summary>
                    /// The character position of first character within the line </summary>
                    int _tokenStartCharPositionInLine;

                    /// <summary>
                    /// Once we see EOF on char stream, next token will be EOF.
                    ///  If you have DONE : EOF ; then you see DONE EOF.
                    /// </summary>
                    bool _hitEOF;

                    /// <summary>
                    /// The channel number for the current token </summary>
                    int _channel;

                    /// <summary>
                    /// The token type for the current token </summary>
                    int _type;

                    // Use the vector as a stack.
                    std::vector<int> _modeStack;
                    int _mode;

                    /// <summary>
                    /// You can set the text for the current token to override what is in
                    ///  the input char buffer.  Use setText() or can set this instance var.
                    /// </summary>
                    std::wstring _text;

                    Lexer();

                    Lexer(CharStream *input);

                    virtual void reset();

                    /// <summary>
                    /// Return a token from this source; i.e., match a token on the char
                    ///  stream.
                    /// </summary>
                    virtual Token *nextToken();

                    /// <summary>
                    /// Instruct the lexer to skip creating a token for current lexer rule
                    ///  and look for another token.  nextToken() knows to keep looking when
                    ///  a lexer rule finishes with token set to SKIP_TOKEN.  Recall that
                    ///  if token==null at end of any token rule, it creates one for you
                    ///  and emits it.
                    /// </summary>
                    virtual void skip();

                    virtual void more();

                    virtual void mode(int m);

                    virtual void pushMode(int m);

                    virtual int popMode();

                    template<typename T1>
                    void setTokenFactory(TokenFactory<T1> *factory)  {
                        this->_factory = factory;
                    }

                    virtual TokenFactory<CommonToken*> *getTokenFactory() override;

                    /// <summary>
                    /// Set the char stream and reset the lexer </summary>
                    virtual void setInputStream(IntStream *input);

                    virtual std::string getSourceName() override;

                    virtual CharStream *getInputStream() override;

                    /// <summary>
                    /// By default does not support multiple emits per nextToken invocation
                    ///  for efficiency reasons.  Subclass and override this method, nextToken,
                    ///  and getToken (to push tokens into a list and pull from that list
                    ///  rather than a single variable as this implementation does).
                    /// </summary>
                    virtual void emit(Token *token);

                    /// <summary>
                    /// The standard method called to automatically emit a token at the
                    ///  outermost lexical rule.  The token object should point into the
                    ///  char buffer start..stop.  If there is a text override in 'text',
                    ///  use that to set the token's text.  Override this method to emit
                    ///  custom Token objects or provide a new factory.
                    /// </summary>
                    virtual Token *emit();

                    virtual Token *emitEOF();

                    virtual int getLine() override;

                    virtual int getCharPositionInLine() override;

                    virtual void setLine(int line);

                    virtual void setCharPositionInLine(int charPositionInLine);

                    /// <summary>
                    /// What is the index of the current character of lookahead? </summary>
                    virtual int getCharIndex();

                    /// <summary>
                    /// Return the text matched so far for the current token or any
                    ///  text override.
                    /// </summary>
                    virtual std::wstring getText();

                    /// <summary>
                    /// Set the complete text of this token; it wipes any previous
                    ///  changes to the text.
                    /// </summary>
                    virtual void setText(const std::wstring &text);

                    /// <summary>
                    /// Override if emitting multiple tokens. </summary>
                    virtual Token *getToken();

                    virtual void setToken(Token *_token);

                    virtual void setType(int ttype);

                    virtual int getType();

                    virtual void setChannel(int channel);

                    virtual int getChannel();

                    virtual const std::vector<std::wstring>& getModeNames();

                    /// <summary>
                    /// Used to print out token names like ID during debugging and
                    ///  error reporting.  The generated parsers implement a method
                    ///  that overrides this to point to their String[] tokenNames.
                    /// </summary>
                    virtual const std::vector<std::wstring>& getTokenNames() override;

                    /// <summary>
                    /// Return a list of all Token objects in input char stream.
                    ///  Forces load of all tokens. Does not include EOF token.
                    /// </summary>
                    virtual std::vector<Token*> getAllTokens();

                    virtual void recover(LexerNoViableAltException *e);

                    virtual void notifyListeners(LexerNoViableAltException *e);

                    virtual std::wstring getErrorDisplay(const std::wstring &s);

                    virtual std::wstring getErrorDisplay(int c);

                    virtual std::wstring getCharErrorDisplay(int c);

                    /// <summary>
                    /// Lexers can normally match any char in it's vocabulary after matching
                    ///  a token, so do the easy thing and just kill a character and hope
                    ///  it all works out.  You can instead use the rule invocation stack
                    ///  to do sophisticated error recovery if you are in a fragment rule.
                    /// </summary>
                    virtual void recover(RecognitionException *re);

                private:
                    void InitializeInstanceFields();
                };

            }
        }
    }
}

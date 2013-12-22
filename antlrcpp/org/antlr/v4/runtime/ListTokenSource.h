#pragma once

#include "TokenSource.h"
#include "Token.h"
#include "TokenFactory.h"
#include "CharStream.h"
#include <string>
#include <vector>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {

                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Pair;


                /// <summary>
                /// Provides an implementation of <seealso cref="TokenSource"/> as a wrapper around a list
                /// of <seealso cref="Token"/> objects.
                /// <p/>
                /// If the final token in the list is an <seealso cref="Token#EOF"/> token, it will be used
                /// as the EOF token for every call to <seealso cref="#nextToken"/> after the end of the
                /// list is reached. Otherwise, an EOF token will be created.
                /// </summary>
                class ListTokenSource : public TokenSource {
                    /// <summary>
                    /// The wrapped collection of <seealso cref="Token"/> objects to return.
                    /// </summary>
                protected:
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: protected final java.util.List<? extends Token> tokens;
                    const std::vector<? extends Token> tokens;

                    /// <summary>
                    /// The name of the input source. If this value is {@code null}, a call to
                    /// <seealso cref="#getSourceName"/> should return the source name used to create the
                    /// the next token in <seealso cref="#tokens"/> (or the previous token if the end of
                    /// the input has been reached).
                    /// </summary>
                private:
                    const std::wstring sourceName;

                    /// <summary>
                    /// The index into <seealso cref="#tokens"/> of token to return by the next call to
                    /// <seealso cref="#nextToken"/>. The end of the input is indicated by this value
                    /// being greater than or equal to the number of items in <seealso cref="#tokens"/>.
                    /// </summary>
                protected:
                    int i;

                    /// <summary>
                    /// This field caches the EOF token for the token source.
                    /// </summary>
                    Token *eofToken;

                    /// <summary>
                    /// This is the backing field for <seealso cref="#getTokenFactory"/> and
                    /// <seealso cref="setTokenFactory"/>.
                    /// </summary>
                private:
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: private TokenFactory<?> _factory = CommonTokenFactory.DEFAULT;
                    TokenFactory<?> *_factory;

                    /// <summary>
                    /// Constructs a new <seealso cref="ListTokenSource"/> instance from the specified
                    /// collection of <seealso cref="Token"/> objects.
                    /// </summary>
                    /// <param name="tokens"> The collection of <seealso cref="Token"/> objects to provide as a
                    /// <seealso cref="TokenSource"/>. </param>
                    /// <exception cref="NullPointerException"> if {@code tokens} is {@code null} </exception>
                public:
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                    template<typename T1> where T1 : Token
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ListTokenSource(std::vector<T1> tokens); //this(tokens, nullptr);

                    /// <summary>
                    /// Constructs a new <seealso cref="ListTokenSource"/> instance from the specified
                    /// collection of <seealso cref="Token"/> objects and source name.
                    /// </summary>
                    /// <param name="tokens"> The collection of <seealso cref="Token"/> objects to provide as a
                    /// <seealso cref="TokenSource"/>. </param>
                    /// <param name="sourceName"> The name of the <seealso cref="TokenSource"/>. If this value is
                    /// {@code null}, <seealso cref="#getSourceName"/> will attempt to infer the name from
                    /// the next <seealso cref="Token"/> (or the previous token if the end of the input has
                    /// been reached).
                    /// </param>
                    /// <exception cref="NullPointerException"> if {@code tokens} is {@code null} </exception>
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                    template<typename T1> where T1 : Token
                    ListTokenSource(std::vector<T1> tokens, const std::wstring &sourceName);

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    virtual int getCharPositionInLine() override;

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    virtual Token *nextToken() override;

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    virtual int getLine() override;

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    virtual CharStream *getInputStream() override;

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    virtual std::wstring getSourceName() override;

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    template<typename T1>
                    virtual void setTokenFactory(TokenFactory<T1> *factory) override;

                    /// <summary>
                    /// @inheritDoc
                    /// </summary>
                    virtual TokenFactory<?> *getTokenFactory() override;

                private:
                    void InitializeInstanceFields();
                };

            }
        }
    }
}

#pragma once

#include "RecognitionException.h"
#include "Parser.h"
#include <string>

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

                using org::antlr::v4::runtime::atn::ATNState;
                using org::antlr::v4::runtime::atn::AbstractPredicateTransition;
                using org::antlr::v4::runtime::atn::PredicateTransition;
                using org::antlr::v4::runtime::misc::NotNull;
                using org::antlr::v4::runtime::misc::Nullable;


                /// <summary>
                /// A semantic predicate failed during validation.  Validation of predicates
                ///  occurs when normally parsing the alternative just like matching a token.
                ///  Disambiguating predicate evaluation occurs when we test a predicate during
                ///  prediction.
                /// </summary>
                class FailedPredicateException : public RecognitionException {
                private:
                    const int ruleIndex;
                    const int predicateIndex;
                    const std::wstring predicate;

                public:
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    FailedPredicateException(Parser *recognizer); //this(recognizer, nullptr);

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    FailedPredicateException(Parser *recognizer, const std::wstring &predicate); //this(recognizer, predicate, nullptr);

                    FailedPredicateException(Parser *recognizer, const std::wstring &predicate, const std::wstring &message);

                    virtual int getRuleIndex();

                    virtual int getPredIndex();

                    virtual std::wstring getPredicate();

                private:
                    static std::wstring formatMessage(const std::wstring &predicate, const std::wstring &message);
                };

            }
        }
    }
}

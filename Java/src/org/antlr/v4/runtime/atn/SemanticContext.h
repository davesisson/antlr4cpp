#pragma once

#include "Java/src/org/antlr/v4/runtime/Recognizer.h"
#include "Java/src/org/antlr/v4/runtime/RuleContext.h"
#include <string>
#include <vector>
#include <set>

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
                namespace atn {

                    using org::antlr::v4::runtime::Recognizer;
                    using org::antlr::v4::runtime::RuleContext;
                    using org::antlr::v4::runtime::misc::MurmurHash;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Utils;


                    /// <summary>
                    /// A tree structure used to record the semantic context in which
                    ///  an ATN configuration is valid.  It's either a single predicate,
                    ///  a conjunction {@code p1&&p2}, or a sum of products {@code p1||p2}.
                    /// <p/>
                    ///  I have scoped the <seealso cref="AND"/>, <seealso cref="OR"/>, and <seealso cref="Predicate"/> subclasses of
                    ///  <seealso cref="SemanticContext"/> within the scope of this outer class.
                    /// </summary>
                    class SemanticContext {
                    public:
                        class Predicate : public SemanticContext {
                        public:
                            const int ruleIndex;
                               const int predIndex;
                               const bool isCtxDependent; // e.g., $i ref in pred

                        protected:
                            Predicate();

                        public:
                            Predicate(int ruleIndex, int predIndex, bool isCtxDependent);

                            template<typename T1, typename T1>
                            virtual bool eval(Recognizer<T1> *parser, RuleContext *outerContext) override;

                            virtual int hashCode() override;

                            virtual bool equals(void *obj) override;

                            virtual std::wstring toString() override;
                        };

                    public:
                        class PrecedencePredicate : public SemanticContext, public Comparable<PrecedencePredicate*> {
                        public:
                            const int precedence;

                        protected:
                            PrecedencePredicate();

                        public:
                            PrecedencePredicate(int precedence);

                            template<typename T1, typename T1>
                            virtual bool eval(Recognizer<T1> *parser, RuleContext *outerContext) override;

                            virtual int compareTo(PrecedencePredicate *o) override;

                            virtual int hashCode() override;

                            virtual bool equals(void *obj) override;

                            virtual std::wstring toString() override;
                        };

                    public:
                        class AND : public SemanticContext {
                        public:
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: @NotNull public final SemanticContext[] opnds;
                            const SemanticContext *opnds;

                            AND(SemanticContext *a, SemanticContext *b);

                            virtual bool equals(void *obj) override;

                            virtual int hashCode() override;

                            template<typename T1, typename T1>
                            virtual bool eval(Recognizer<T1> *parser, RuleContext *outerContext) override;

                            virtual std::wstring toString() override;
                        };

                    public:
                        class OR : public SemanticContext {
                        public:
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: @NotNull public final SemanticContext[] opnds;
                            const SemanticContext *opnds;

                            OR(SemanticContext *a, SemanticContext *b);

                            virtual bool equals(void *obj) override;

                            virtual int hashCode() override;

                            template<typename T1, typename T1>
                            virtual bool eval(Recognizer<T1> *parser, RuleContext *outerContext) override;

                            virtual std::wstring toString() override;
                        };

                    public:
                        static SemanticContext *const NONE;

                        SemanticContext *parent;

                        /// <summary>
                        /// For context independent predicates, we evaluate them without a local
                        /// context (i.e., null context). That way, we can evaluate them without
                        /// having to create proper rule-specific context during prediction (as
                        /// opposed to the parser, which creates them naturally). In a practical
                        /// sense, this avoids a cast exception from RuleContext to myruleContext.
                        /// <p/>
                        /// For context dependent predicates, we must pass in a local context so that
                        /// references such as $arg evaluate properly as _localctx.arg. We only
                        /// capture context dependent predicates in the context in which we begin
                        /// prediction, so we passed in the outer context here in case of context
                        /// dependent predicate evaluation.
                        /// </summary>
                        template<typename T1, typename T1>
                        virtual bool eval(Recognizer<T1> *parser, RuleContext *outerContext) = 0;

                        static SemanticContext *and(SemanticContext *a, SemanticContext *b);

                        /// 
                        ///  <seealso cref= ParserATNSimulator#getPredsForAmbigAlts </seealso>
                        static SemanticContext *or(SemanticContext *a, SemanticContext *b);

                    private:
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                        template<typename T1> where T1 : SemanticContext
                        static std::vector<PrecedencePredicate*> filterPrecedencePredicates(Collection<T1> *collection);
                    };

                }
            }
        }
    }
}

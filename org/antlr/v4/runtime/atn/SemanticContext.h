#pragma once

#include "Recognizer.h"
#include "SemanticContext.h"
#include <string>
#include <vector>
#include <set>

#include "Declarations.h"


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
                        static SemanticContext *const NONE;
                        
                        class Predicate;
                        class PrecedencePredicate;
                        class AND;
                        class OR;
                        
                        SemanticContext *parent;

                        std::wstring toString();

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
                        
                        
                        template<typename T1, typename T2>
                        bool eval(Recognizer<T1, T2> *parser, RuleContext *outerContext);

                        static SemanticContext *And(SemanticContext *a, SemanticContext *b);

                        /// 
                        ///  <seealso cref= ParserATNSimulator#getPredsForAmbigAlts </seealso>
                        static SemanticContext *Or(SemanticContext *a, SemanticContext *b);

                    private:

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                        template<typename T1>
                        static std::vector<PrecedencePredicate*> filterPrecedencePredicates(std::set<T1> *collection);

                    };
                    

                    class SemanticContext::Predicate : public SemanticContext {
                        friend class SemanticContext;

                    public:
                        const int ruleIndex;
                        const int predIndex;
                        const bool isCtxDependent; // e.g., $i ref in pred
                        
                    protected:
                        Predicate();
                        
                    public:
                        Predicate(int ruleIndex, int predIndex, bool isCtxDependent);

                        template<typename T1, typename T2>
                        bool eval(Recognizer<T1, T2> *parser, RuleContext *outerContext);

                        virtual int hashCode() ;
                        
                        virtual bool equals(void *obj) ;
                        
                        virtual std::wstring toString() ;
                    };
                    
                    class SemanticContext::PrecedencePredicate : public SemanticContext /*, public Comparable<PrecedencePredicate*>*/ {
                    public:
                        const int precedence;
                        
                    protected:
                        PrecedencePredicate();
                        
                    public:
                        PrecedencePredicate(int precedence);

                        template<typename T1, typename T2>
                        bool eval(Recognizer<T1, T2> *parser, RuleContext *outerContext);

                        
                        virtual int compareTo(PrecedencePredicate *o);
                        
                        virtual int hashCode();
                        
                        virtual bool equals(void *obj);
                        
                        virtual std::wstring toString();
                    };
                    
                    class SemanticContext::AND : public SemanticContext {
                    public:
                        //JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                        //ORIGINAL LINE: @NotNull public final SemanticContext[] opnds;
                        const std::vector<SemanticContext*> opnds;
                        
                        AND(SemanticContext *a, SemanticContext *b);
                        
                        virtual bool equals(void *obj);
                        
                        virtual int hashCode();

                        template<typename T1, typename T2>
                        bool eval(Recognizer<T1, T2> *parser, RuleContext *outerContext);

                        
                        virtual std::wstring toString();
                    };
                    
                    class SemanticContext::OR : public SemanticContext {
                    public:
                        //JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
                        //ORIGINAL LINE: @NotNull public final SemanticContext[] opnds;
                        const SemanticContext *opnds;
                        
                        OR(SemanticContext *a, SemanticContext *b);
                        
                        virtual bool equals(SemanticContext *obj);
                        
                        virtual int hashCode();

                        template<typename T1, typename T2>
                        bool eval(Recognizer<T1, T2> *parser, RuleContext *outerContext);

                        virtual std::wstring toString();
                    };

                }
            }
        }
    }
}

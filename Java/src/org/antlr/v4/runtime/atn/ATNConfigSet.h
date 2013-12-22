#pragma once

#include "ATNConfig.h"
#include "Java/src/org/antlr/v4/runtime/misc/Array2DHashSet.h"
#include "Java/src/org/antlr/v4/runtime/misc/AbstractEqualityComparator.h"
#include "PredictionContext.h"
#include "Java/src/org/antlr/v4/runtime/misc/DoubleKeyMap.h"
#include "ATNState.h"
#include "SemanticContext.h"
#include "ATNSimulator.h"
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

                    using org::antlr::v4::runtime::misc::AbstractEqualityComparator;
                    using org::antlr::v4::runtime::misc::Array2DHashSet;
                    using org::antlr::v4::runtime::misc::DoubleKeyMap;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;


                    /// <summary>
                    /// Specialized <seealso cref="Set"/>{@code <}<seealso cref="ATNConfig"/>{@code >} that can track
                    /// info about the set, with support for combining similar configurations using a
                    /// graph-structured stack.
                    /// </summary>
                    class ATNConfigSet : public Set<ATNConfig*> {
                    public:
                        class AbstractConfigHashSet : public Array2DHashSet<ATNConfig*> {

                        public:
                            template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public AbstractConfigHashSet(org.antlr.v4.runtime.misc.AbstractEqualityComparator<? super ATNConfig> comparator)
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                            AbstractConfigHashSet(AbstractEqualityComparator<T1> *comparator); //this(comparator, 16, 2);

                            template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public AbstractConfigHashSet(org.antlr.v4.runtime.misc.AbstractEqualityComparator<? super ATNConfig> comparator, int initialCapacity, int initialBucketCapacity)
                            AbstractConfigHashSet(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity);

                        protected:
                            ATNConfig *asElementType(void *o) override;

                            ATNConfig **createBuckets(int capacity) override;

                            ATNConfig *createBucket(int capacity) override;

                        };
                        /// <summary>
                        /// The reason that we need this is because we don't want the hash map to use
                        /// the standard hash code and equals. We need all configurations with the same
                        /// {@code (s,i,_,semctx)} to be equal. Unfortunately, this key effectively doubles
                        /// the number of objects associated with ATNConfigs. The other solution is to
                        /// use a hash table that lets us specify the equals/hashcode operation.
                        /// </summary>
                    public:
                        class ConfigHashSet : public AbstractConfigHashSet {
                        public:
                            ConfigHashSet();
                        };

                    public:
                        class ConfigEqualityComparator final : public AbstractEqualityComparator<ATNConfig*> {
                        public:
                            static ConfigEqualityComparator *const INSTANCE;

                        private:
                            ConfigEqualityComparator();

                        public:
                            int hashCode(ATNConfig *o) override;

                            bool equals(ATNConfig *a, ATNConfig *b) override;
                        };

                        /// <summary>
                        /// Indicates that the set of configurations is read-only. Do not
                        ///  allow any code to manipulate the set; DFA states will point at
                        ///  the sets and they must not change. This does not protect the other
                        ///  fields; in particular, conflictingAlts is set after
                        ///  we've made this readonly.
                        /// </summary>
                    protected:
                        bool readonly;

                        /// <summary>
                        /// All configs but hashed by (s, i, _, pi) not including context. Wiped out
                        /// when we go readonly as this set becomes a DFA state.
                        /// </summary>
                    public:
                        AbstractConfigHashSet *configLookup;

                        /// <summary>
                        /// Track the elements as they are added to the set; supports get(i) </summary>
                        const std::vector<ATNConfig*> configs;

                        // TODO: these fields make me pretty uncomfortable but nice to pack up info together, saves recomputation
                        // TODO: can we track conflicts as they are added to save scanning configs later?
                        int uniqueAlt;
                    protected:
                        BitSet *conflictingAlts;

                        // Used in parser and lexer. In lexer, it indicates we hit a pred
                        // while computing a closure operation.  Don't make a DFA state from this.
                    public:
                        bool hasSemanticContext;
                        bool dipsIntoOuterContext;

                        /// <summary>
                        /// Indicates that this configuration set is part of a full context
                        ///  LL prediction. It will be used to determine how to merge $. With SLL
                        ///  it's a wildcard whereas it is not for LL context merge.
                        /// </summary>
                        const bool fullCtx;

                    private:
                        int cachedHashCode;

                    public:
                        ATNConfigSet(bool fullCtx);
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        ATNConfigSet(); //this(true);

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        ATNConfigSet(ATNConfigSet *old); //this(old.fullCtx);

                        virtual bool add(ATNConfig *config) override;

                        /// <summary>
                        /// Adding a new config means merging contexts with existing configs for
                        /// {@code (s, i, pi, _)}, where {@code s} is the
                        /// <seealso cref="ATNConfig#state"/>, {@code i} is the <seealso cref="ATNConfig#alt"/>, and
                        /// {@code pi} is the <seealso cref="ATNConfig#semanticContext"/>. We use
                        /// {@code (s,i,pi)} as key.
                        /// <p/>
                        /// This method updates <seealso cref="#dipsIntoOuterContext"/> and
                        /// <seealso cref="#hasSemanticContext"/> when necessary.
                        /// </summary>
                        virtual bool add(ATNConfig *config, DoubleKeyMap<PredictionContext*, PredictionContext*, PredictionContext*> *mergeCache);

                        /// <summary>
                        /// Return a List holding list of configs </summary>
                        virtual std::vector<ATNConfig*> elements();

                        virtual Set<ATNState*> *getStates();

                        virtual std::vector<SemanticContext*> getPredicates();

                        virtual ATNConfig *get(int i);

                        virtual void optimizeConfigs(ATNSimulator *interpreter);

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
                        template<typename T1> where T1 : ATNConfig
                        virtual bool addAll(Collection<T1> *coll) override;

                        virtual bool equals(void *o) override;

                        virtual int hashCode() override;

                        virtual int size() override;

                        virtual bool isEmpty() override;

                        virtual bool contains(void *o) override;

                        virtual bool containsFast(ATNConfig *obj);

                        virtual Iterator<ATNConfig*> *iterator() override;

                        virtual void clear() override;

                        virtual bool isReadonly();

                        virtual void setReadonly(bool readonly);

                        virtual std::wstring toString() override;

                        // satisfy interface

                        virtual ATNConfig *toArray() override;

                        template<typename T>
                        virtual T *toArray(T a[]) override;

                        virtual bool remove(void *o) override;

                        template<typename T1>
                        virtual bool containsAll(Collection<T1> *c) override;

                        template<typename T1>
                        virtual bool retainAll(Collection<T1> *c) override;

                        template<typename T1>
                        virtual bool removeAll(Collection<T1> *c) override;


                    private:
                        void InitializeInstanceFields();
                    };

                }
            }
        }
    }
}

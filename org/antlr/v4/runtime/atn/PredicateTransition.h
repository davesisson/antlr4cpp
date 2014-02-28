﻿#pragma once

#include "AbstractPredicateTransition.h"
#include "ATNState.h"
#include "SemanticContext.h"
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
                namespace atn {

                    using org::antlr::v4::runtime::misc::NotNull;

                    /// <summary>
                    /// TODO: this is old comment:
                    ///  A tree of semantic predicates from the grammar AST if label==SEMPRED.
                    ///  In the ATN, labels will always be exactly one predicate, but the DFA
                    ///  may have to combine a bunch of them as it collects predicates from
                    ///  multiple ATN configurations into a single DFA state.
                    /// </summary>
                    class PredicateTransition final : public AbstractPredicateTransition {
                    public:
                        const int ruleIndex;
                        const int predIndex;
                        const bool isCtxDependent; // e.g., $i ref in pred

                        PredicateTransition(ATNState *target, int ruleIndex, int predIndex, bool isCtxDependent);

                        virtual int getSerializationType() override;

                        virtual bool isEpsilon() override;
                        virtual bool matches(int symbol, int minVocabSymbol, int maxVocabSymbol) override;

                        SemanticContext::Predicate *getPredicate();

                        virtual std::wstring toString();

                    };

                }
            }
        }
    }
}

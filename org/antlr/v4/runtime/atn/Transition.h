﻿#pragma once

#include "ATNState.h"
#include "Declarations.h"


#include <string>
#include <vector>
#include <map>

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
                    /// An ATN transition between any two ATN states.  Subclasses define
                    ///  atom, set, epsilon, action, predicate, rule transitions.
                    /// <p/>
                    ///  This is a one way link.  It emanates from a state (usually via a list of
                    ///  transitions) and has a target state.
                    /// <p/>
                    ///  Since we never have to change the ATN transitions once we construct it,
                    ///  we can fix these transitions as specific classes. The DFA transitions
                    ///  on the other hand need to update the labels as it adds transitions to
                    ///  the states. We'll use the term Edge for the DFA to distinguish them from
                    ///  ATN transitions.
                    /// </summary>
                    class Transition {
                        // constants for serialization
                    public:
                        static const int EPSILON = 1;
                        static const int RANGE = 2;
                        static const int RULE = 3;
                        static const int PREDICATE = 4; // e.g., {isType(input.LT(1))}?
                        static const int ATOM = 5;
                        static const int ACTION = 6;
                        static const int SET = 7; // ~(A|B) or ~atom, wildcard, which convert to next 2
                        static const int NOT_SET = 8;
                        static const int WILDCARD = 9;
                        static const int PRECEDENCE = 10;


                        static const std::vector<std::wstring> serializationNames;

                        static std::map<Transition *, int> *const serializationTypes;

                        /// <summary>
                        /// The target of this transition. </summary>
                        ATNState *target;

                    protected:
                        Transition(ATNState *target);

                    public:
                        virtual int getSerializationType() = 0;

                        /// <summary>
                        /// Are we epsilon, action, sempred? </summary>
                        virtual bool isEpsilon();

                        virtual misc::IntervalSet *label();

                        virtual bool matches(int symbol, int minVocabSymbol, int maxVocabSymbol) = 0;
                    };

                }
            }
        }
    }
}

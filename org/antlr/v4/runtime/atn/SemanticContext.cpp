﻿#include <typeinfo>

#include "SemanticContext.h"
#include "MurmurHash.h"
#include "Utils.h"
#include "Arrays.h"

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
                    SemanticContext::Predicate::Predicate() : ruleIndex(-1), predIndex(-1), isCtxDependent(false) {
                    }

                    SemanticContext::Predicate::Predicate(int ruleIndex, int predIndex, bool isCtxDependent) : ruleIndex(ruleIndex), predIndex(predIndex), isCtxDependent(isCtxDependent) {
                    }

                    std::wstring SemanticContext::toString() {
#ifdef TODO 
                        WRITE ME
#endif
                        return nullptr;
                    }
                    
                    int SemanticContext::hashCode() {
#ifdef TODO
                        WRITE ME
#endif
                        return 0;
                    }
                    
                    bool SemanticContext::equals(void *obj) {
                        // "SemanticContext::equals should have been called on a daughter class"
                        throw new std::exception();
                    }
                    

                    int SemanticContext::Predicate::hashCode() {
                        int hashCode = misc::MurmurHash::initialize();
                        hashCode = misc::MurmurHash::update(hashCode, ruleIndex);
                        hashCode = misc::MurmurHash::update(hashCode, predIndex);
                        hashCode = misc::MurmurHash::update(hashCode, isCtxDependent ? 1 : 0);
                        hashCode = misc::MurmurHash::finish(hashCode, 3);
                        return hashCode;
                    }

                    bool SemanticContext::Predicate::equals(void *obj) {
                        // TODO: this is wrong
                        if (!((Predicate*)obj/*dynamic_cast<Predicate*>(obj)*/ != nullptr)) {
                            return false;
                        }
                        if (this == obj) {
                            return true;
                        }
                        Predicate *p = static_cast<Predicate*>(obj);
                        return this->ruleIndex == p->ruleIndex && this->predIndex == p->predIndex && this->isCtxDependent == p->isCtxDependent;
                    }

                    std::wstring SemanticContext::Predicate::toString() {
                        return std::wstring(L"{") + std::to_wstring(ruleIndex) + std::wstring(L":") + std::to_wstring(predIndex) + std::wstring(L"}?");
                    }

                    SemanticContext::PrecedencePredicate::PrecedencePredicate() : precedence(0) {
                    }

                    SemanticContext::PrecedencePredicate::PrecedencePredicate(int precedence) : precedence(precedence) {
                    }

                    int SemanticContext::PrecedencePredicate::compareTo(PrecedencePredicate *o) {
                        return precedence - o->precedence;
                    }

                    int SemanticContext::PrecedencePredicate::hashCode() {
                        int hashCode = 1;
                        hashCode = 31 * hashCode + precedence;
                        return hashCode;
                    }

                    bool SemanticContext::PrecedencePredicate::equals(void *obj) {
                        // TODO: this is wrong
                        if (!((Predicate*)obj/*dynamic_cast<PrecedencePredicate*>(obj)*/ != nullptr)) {
                            return false;
                        }

                        if (this == obj) {
                            return true;
                        }

                        PrecedencePredicate *other = static_cast<PrecedencePredicate*>(obj);
                        return this->precedence == other->precedence;
                    }

                    std::wstring SemanticContext::PrecedencePredicate::toString() {
                        return SemanticContext::toString();
                    }

#ifdef TODO
                    // I can't make heads or tails of this
                    SemanticContext::AND::AND(SemanticContext *a, SemanticContext *b) : opnds(operands::toArray(new SemanticContext[operands->size()])) {
                        std::vector<SemanticContext*> *operands = new std::vector<SemanticContext*>();
                        if ((AND*)a/*dynamic_cast<AND*>(a)*/ != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<AND*>(a))->opnds));
                        } else {
                            operands->add(a);
                        }
                        if ((AND*)/*dynamic_cast<AND*>*/(b) != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<AND*>(b))->opnds));
                        } else {
                            operands->add(b);
                        }

                        std::vector<PrecedencePredicate*> precedencePredicates = filterPrecedencePredicates(operands);
                        if (!precedencePredicates.empty()) {
                            // interested in the transition with the lowest precedence
                            PrecedencePredicate *reduced = Collections::min(precedencePredicates);
                            operands->add(reduced);
                        }

                    }
#endif
                    bool SemanticContext::AND::equals(void *obj) {
                        if (this == obj) {
                            return true;
                        }
                        // TODO: this is wrong
                        if (!((AND*)obj/*dynamic_cast<AND*>(obj)*/ != nullptr)) {
                            return false;
                        }
                        AND *other = static_cast<AND*>(obj);
                        return (this->opnds == other->opnds);//Arrays::equals(this->opnds, other->opnds);
                    }

                    
                    int SemanticContext::AND::hashCode() {
                        return misc::MurmurHash::hashCode(opnds.data(), 1234 /*TODO: AND::typeid::hashCode()*/);
                    }

                

                    std::wstring SemanticContext::AND::toString() {
                        // TODO: Utils class has not been declared
                        // return Utils::join(Arrays::asList(opnds)->begin(), L"&&");

                        return L"";
                    }

                    // TODO: operands is not defined.  In SemanticContext.java
                    // it's declared as Set<SemanticContext> and initialized to
                    // an instance of HashSet.
                    SemanticContext::OR::OR(SemanticContext *a, SemanticContext *b){
                        
#ifdef TODO 
                        std::vector<SemanticContext*> *operands = new std::vector<SemanticContext*>();
                        // this is a mess
                        opnds = operands::toArray(new SemanticContext[operands->size()]);
                        if ((OR*)/*dynamic_cast<OR*>*/(a) != nullptr) {
                            operands->addAll(Arrays::asList( ((OR*)(a))->opnds) );
                        } else {
                            operands->add(a);
                        }
                        if (dynamic_cast<OR*>(b) != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<OR*>(b))->opnds));
                        } else {
                            operands->add(b);
                        }

                        std::vector<PrecedencePredicate*> precedencePredicates = filterPrecedencePredicates(operands);
                        if (!precedencePredicates.empty()) {
                            // interested in the transition with the highest precedence
                            PrecedencePredicate *reduced = Collections::max(precedencePredicates);
                            operands->add(reduced);
                        }
#endif
                    }

                    // TODO: changed obj from void* to SemanticContext*
                    bool SemanticContext::OR::equals(SemanticContext *obj) {
                        if (this == obj) {
                            return true;
                        }

                        if (obj == nullptr || typeid(*obj) != typeid(*this)) {
                            return false;
                        }

                        // TODO: fix Array::equals
                        //OR *other = static_cast<OR*>(obj);
                        //return Arrays::equals(this->opnds, other->opnds);
                        return false;
                    }

                    int SemanticContext::OR::hashCode() {
                        // TODO: seed? (OR::typeid::hashCode()???)
                        return misc::MurmurHash::hashCode<std::vector<SemanticContext*>>(opnds, 0);
                    }


                    std::wstring SemanticContext::OR::toString() {
                        // TODO: Utils
                        //return misc::Utils::join(Arrays::asList(opnds)->begin(), L"||");
                        return L"";
                    }

                    SemanticContext *const SemanticContext::NONE = new Predicate();

                    atn::SemanticContext *SemanticContext::And(SemanticContext *a, SemanticContext *b) {
                        if (a == nullptr || a == NONE) {
                            return b;
                        }
                        if (b == nullptr || b == NONE) {
                            return a;
                        }
                        AND *result = new AND(a, b);
                        if (result->opnds.size() == 1) {
                            return result->opnds[0];
                        }

                        return result;
                    }

                    SemanticContext *SemanticContext::Or(SemanticContext *a, SemanticContext *b) {
                        if (a == nullptr) {
                            return b;
                        }
                        if (b == nullptr) {
                            return a;
                        }
                        if (a == NONE || b == NONE) {
                            return NONE;
                        }
                        OR *result = new OR(a, b);
                        if (result->opnds.size() == 1) {
                            return result->opnds[0];
                        }

                        return result;
                    }

                }
            }
        }
    }
}





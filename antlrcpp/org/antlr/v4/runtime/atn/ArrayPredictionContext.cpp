#include "ArrayPredictionContext.h"
#include "StringBuilder.h"

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
                    ArrayPredictionContext::ArrayPredictionContext(SingletonPredictionContext *a) : PredictionContext {
                    }

                    ArrayPredictionContext::ArrayPredictionContext(PredictionContext parents[], int returnStates[]) : PredictionContext(calculateHashCode(parents, returnStates)), parents(parents), returnStates(returnStates) {
                        assert(parents != nullptr && sizeof(parents) / sizeof(parents[0]) > 0);
                        assert(returnStates != nullptr && sizeof(returnStates) / sizeof(returnStates[0]) > 0);
                                        //		System.err.println("CREATE ARRAY: "+Arrays.toString(parents)+", "+Arrays.toString(returnStates));
                    }

                    bool ArrayPredictionContext::isEmpty() {
                        // since EMPTY_RETURN_STATE can only appear in the last position, we
                        // don't need to verify that size==1
                        return returnStates[0] == EMPTY_RETURN_STATE;
                    }

                    int ArrayPredictionContext::size() {
                        return returnStates->length;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *ArrayPredictionContext::getParent(int index) {
                        return parents[index];
                    }

                    int ArrayPredictionContext::getReturnState(int index) {
                        return returnStates[index];
                    }

                    bool ArrayPredictionContext::equals(void *o) {
                        if (this == o) {
                            return true;
                        } else if (!(dynamic_cast<ArrayPredictionContext*>(o) != nullptr)) {
                            return false;
                        }

                        if (this->hashCode() != o->hashCode()) {
                            return false; // can't be same if hash is different
                        }

                        ArrayPredictionContext *a = static_cast<ArrayPredictionContext*>(o);
                        return Arrays::equals(returnStates, a->returnStates) && Arrays::equals(parents, a->parents);
                    }

                    std::wstring ArrayPredictionContext::toString() {
                        if (isEmpty()) {
                            return L"[]";
                        }
                        StringBuilder *buf = new StringBuilder();
                        buf->append(L"[");
                        for (int i = 0; i < returnStates->length; i++) {
                            if (i > 0) {
                                buf->append(L", ");
                            }
                            if (returnStates[i] == EMPTY_RETURN_STATE) {
                                buf->append(L"$");
                                continue;
                            }
                            buf->append(returnStates[i]);
                            if (parents[i] != nullptr) {
                                buf->append(L' ');
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                buf->append(parents[i]->toString());
                            } else {
                                buf->append(L"null");
                            }
                        }
                        buf->append(L"]");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }
                }
            }
        }
    }
}

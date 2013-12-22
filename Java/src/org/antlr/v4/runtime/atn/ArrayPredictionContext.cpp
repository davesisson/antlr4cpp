#include "ArrayPredictionContext.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ArrayPredictionContext::ArrayPredictionContext(SingletonPredictionContext *a) {
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

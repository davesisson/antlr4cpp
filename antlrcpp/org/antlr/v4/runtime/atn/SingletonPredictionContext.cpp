#include "SingletonPredictionContext.h"
#include "Java/src/org/antlr/v4/runtime/atn/ATNState.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    SingletonPredictionContext::SingletonPredictionContext(PredictionContext *parent, int returnState) : PredictionContext(parent != nullptr ? calculateHashCode(parent, returnState) : calculateEmptyHashCode()), parent(parent), returnState(returnState) {
                        assert(returnState != ATNState::INVALID_STATE_NUMBER);
                    }

                    org::antlr::v4::runtime::atn::SingletonPredictionContext *SingletonPredictionContext::create(PredictionContext *parent, int returnState) {
                        if (returnState == EMPTY_RETURN_STATE && parent == nullptr) {
                            // someone can pass in the bits of an array ctx that mean $
                            return EMPTY;
                        }
                        return new SingletonPredictionContext(parent, returnState);
                    }

                    int SingletonPredictionContext::size() {
                        return 1;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *SingletonPredictionContext::getParent(int index) {
                        assert(index == 0);
                        return parent;
                    }

                    int SingletonPredictionContext::getReturnState(int index) {
                        assert(index == 0);
                        return returnState;
                    }

                    bool SingletonPredictionContext::equals(void *o) {
                        if (this == o) {
                            return true;
                        } else if (!(dynamic_cast<SingletonPredictionContext*>(o) != nullptr)) {
                            return false;
                        }

                        if (this->hashCode() != o->hashCode()) {
                            return false; // can't be same if hash is different
                        }

                        SingletonPredictionContext *s = static_cast<SingletonPredictionContext*>(o);
                        return returnState == s->returnState && (parent != nullptr && parent->equals(s->parent));
                    }

                    std::wstring SingletonPredictionContext::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        std::wstring up = parent != nullptr ? parent->toString() : L"";
                        if (up.length() == 0) {
                            if (returnState == EMPTY_RETURN_STATE) {
                                return L"$";
                            }
                            return StringConverterHelper::toString(returnState);
                        }
                        return StringConverterHelper::toString(returnState) + std::wstring(L" ") + up;
                    }
                }
            }
        }
    }
}

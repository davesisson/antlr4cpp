#include "EmptyPredictionContext.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    EmptyPredictionContext::EmptyPredictionContext() : SingletonPredictionContext(nullptr, EMPTY_RETURN_STATE) {
                    }

                    bool EmptyPredictionContext::isEmpty() {
                        return true;
                    }

                    int EmptyPredictionContext::size() {
                        return 1;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *EmptyPredictionContext::getParent(int index) {
                        return nullptr;
                    }

                    int EmptyPredictionContext::getReturnState(int index) {
                        return returnState;
                    }

                    bool EmptyPredictionContext::equals(void *o) {
                        return this == o;
                    }

                    std::wstring EmptyPredictionContext::toString() {
                        return L"$";
                    }
                }
            }
        }
    }
}

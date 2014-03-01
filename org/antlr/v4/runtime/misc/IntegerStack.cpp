#include "IntegerStack.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

                    IntegerStack::IntegerStack() {
                    }

                    IntegerStack::IntegerStack(int capacity) : IntegerList(capacity) {
                    }

                    IntegerStack::IntegerStack(IntegerStack *list) : IntegerList(list) {
                    }

                    void IntegerStack::push(int value) {
                        add(value);
                    }

                    int IntegerStack::pop() {
                        return removeAt(size() - 1);
                    }

                    int IntegerStack::peek() {
                        return get(size() - 1);
                    }
                }
            }
        }
    }
}

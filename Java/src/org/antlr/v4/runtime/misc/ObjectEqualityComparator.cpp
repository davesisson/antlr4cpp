#include "ObjectEqualityComparator.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

ObjectEqualityComparator *const ObjectEqualityComparator::INSTANCE = new ObjectEqualityComparator();

                    int ObjectEqualityComparator::hashCode(void *obj) {
                        if (obj == nullptr) {
                            return 0;
                        }

                        return obj->hashCode();
                    }

                    bool ObjectEqualityComparator::equals(void *a, void *b) {
                        if (a == nullptr) {
                            return b == nullptr;
                        }

                        return a->equals(b);
                    }
                }
            }
        }
    }
}

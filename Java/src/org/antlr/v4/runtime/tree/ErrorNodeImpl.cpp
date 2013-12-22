#include "ErrorNodeImpl.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    using org::antlr::v4::runtime::Token;

                    ErrorNodeImpl::ErrorNodeImpl(Token *token) : TerminalNodeImpl(token) {
                    }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T, typename T1> where T1 : T
                    T ErrorNodeImpl::accept(ParseTreeVisitor<T1> *visitor) {
                        return visitor->visitErrorNode(this);
                    }
                }
            }
        }
    }
}

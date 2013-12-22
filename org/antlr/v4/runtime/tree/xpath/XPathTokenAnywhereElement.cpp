#include "XPathTokenAnywhereElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/Trees.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Trees;

                        XPathTokenAnywhereElement::XPathTokenAnywhereElement(const std::wstring &tokenName, int tokenType) : XPathElement(tokenName) {
                            InitializeInstanceFields();
                            this->tokenType = tokenType;
                        }

                        Collection<ParseTree*> *XPathTokenAnywhereElement::evaluate(ParseTree *t) {
                            return Trees::findAllTokenNodes(t, tokenType);
                        }

                        void XPathTokenAnywhereElement::InitializeInstanceFields() {
                            tokenType = 0;
                        }
                    }
                }
            }
        }
    }
}

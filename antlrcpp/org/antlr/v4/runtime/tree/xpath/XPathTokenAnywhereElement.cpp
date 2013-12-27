#include "XPathTokenAnywhereElement.h"
#include "Trees.h"


namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
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

#include "XPathWildcardAnywhereElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/Trees.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Trees;

                        XPathWildcardAnywhereElement::XPathWildcardAnywhereElement() : XPathElement(XPath::WILDCARD) {
                        }

                        Collection<ParseTree*> *XPathWildcardAnywhereElement::evaluate(ParseTree *t) {
                            if (invert) { // !* is weird but valid (empty)
                                return std::vector<ParseTree*>();
                            }
                            return Trees::descendants(t);
                        }
                    }
                }
            }
        }
    }
}

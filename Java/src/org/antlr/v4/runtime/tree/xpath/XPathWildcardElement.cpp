#include "XPathWildcardElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/Trees.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Tree;
                        using org::antlr::v4::runtime::tree::Trees;

                        XPathWildcardElement::XPathWildcardElement() : XPathElement(XPath::WILDCARD) {
                        }

                        Collection<ParseTree*> *XPathWildcardElement::evaluate(ParseTree *const t) {
                            if (invert) { // !* is weird but valid (empty)
                                return std::vector<ParseTree*>();
                            }
                            std::vector<ParseTree*> kids = std::vector<ParseTree*>();
                            for (auto c : Trees::getChildren(t)) {
                                kids.push_back(static_cast<ParseTree*>(c));
                            }
                            return kids;
                        }
                    }
                }
            }
        }
    }
}

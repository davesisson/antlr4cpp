#include "XPathWildcardElement.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {

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

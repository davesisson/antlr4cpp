#include "XPathTokenElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/Trees.h"
#include "Java/src/org/antlr/v4/runtime/tree/TerminalNode.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::TerminalNode;
                        using org::antlr::v4::runtime::tree::Tree;
                        using org::antlr::v4::runtime::tree::Trees;

                        XPathTokenElement::XPathTokenElement(const std::wstring &tokenName, int tokenType) : XPathElement(tokenName) {
                            InitializeInstanceFields();
                            this->tokenType = tokenType;
                        }

                        Collection<ParseTree*> *XPathTokenElement::evaluate(ParseTree *t) {
                            // return all children of t that match nodeName
                            std::vector<ParseTree*> nodes = std::vector<ParseTree*>();
                            for (auto c : Trees::getChildren(t)) {
                                if (dynamic_cast<TerminalNode*>(c) != nullptr) {
                                    TerminalNode *tnode = static_cast<TerminalNode*>(c);
                                    if ((tnode->getSymbol()->getType() == tokenType && !invert) || (tnode->getSymbol()->getType() != tokenType && invert)) {
                                        nodes.push_back(tnode);
                                    }
                                }
                            }
                            return nodes;
                        }

                        void XPathTokenElement::InitializeInstanceFields() {
                            tokenType = 0;
                        }
                    }
                }
            }
        }
    }
}

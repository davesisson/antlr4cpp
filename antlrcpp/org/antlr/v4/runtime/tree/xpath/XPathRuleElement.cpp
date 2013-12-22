#include "XPathRuleElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/Trees.h"
#include "Java/src/org/antlr/v4/runtime/ParserRuleContext.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::ParserRuleContext;
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Tree;
                        using org::antlr::v4::runtime::tree::Trees;

                        XPathRuleElement::XPathRuleElement(const std::wstring &ruleName, int ruleIndex) : XPathElement(ruleName) {
                            InitializeInstanceFields();
                            this->ruleIndex = ruleIndex;
                        }

                        Collection<ParseTree*> *XPathRuleElement::evaluate(ParseTree *t) {
                                    // return all children of t that match nodeName
                            std::vector<ParseTree*> nodes = std::vector<ParseTree*>();
                            for (auto c : Trees::getChildren(t)) {
                                if (dynamic_cast<ParserRuleContext*>(c) != nullptr) {
                                    ParserRuleContext *ctx = static_cast<ParserRuleContext*>(c);
                                    if ((ctx->getRuleIndex() == ruleIndex && !invert) || (ctx->getRuleIndex() != ruleIndex && invert)) {
                                        nodes.push_back(ctx);
                                    }
                                }
                            }
                            return nodes;
                        }

                        void XPathRuleElement::InitializeInstanceFields() {
                            ruleIndex = 0;
                        }
                    }
                }
            }
        }
    }
}

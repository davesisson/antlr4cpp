#include "XPathRuleAnywhereElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/Trees.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Trees;

                        XPathRuleAnywhereElement::XPathRuleAnywhereElement(const std::wstring &ruleName, int ruleIndex) : XPathElement(ruleName) {
                            InitializeInstanceFields();
                            this->ruleIndex = ruleIndex;
                        }

                        Collection<ParseTree*> *XPathRuleAnywhereElement::evaluate(ParseTree *t) {
                            return Trees::findAllRuleNodes(t, ruleIndex);
                        }

                        void XPathRuleAnywhereElement::InitializeInstanceFields() {
                            ruleIndex = 0;
                        }
                    }
                }
            }
        }
    }
}

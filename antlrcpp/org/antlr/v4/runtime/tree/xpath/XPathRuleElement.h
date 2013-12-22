#pragma once

#include "XPathElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/ParseTree.h"
#include <string>
#include <vector>

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


                        class XPathRuleElement : public XPathElement {
                        protected:
                            int ruleIndex;
                        public:
                            XPathRuleElement(const std::wstring &ruleName, int ruleIndex);

                            virtual Collection<ParseTree*> *evaluate(ParseTree *t) override;

                        private:
                            void InitializeInstanceFields();
                        };

                    }
                }
            }
        }
    }
}

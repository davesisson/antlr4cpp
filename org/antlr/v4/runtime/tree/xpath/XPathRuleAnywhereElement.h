#pragma once

#include "XPathElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/ParseTree.h"
#include <string>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {

                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Trees;


                        /// <summary>
                        /// Either ID at start of path or ...//ID in middle of path </summary>
                        class XPathRuleAnywhereElement : public XPathElement {
                        protected:
                            int ruleIndex;
                        public:
                            XPathRuleAnywhereElement(const std::wstring &ruleName, int ruleIndex);

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

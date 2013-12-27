#pragma once

#include "XPathElement.h"
#include <string>
#include <set>

class ParseTree;

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {

                        class XPathTokenAnywhereElement : public XPathElement {
                        protected:
                            int tokenType;
                        public:
                            XPathTokenAnywhereElement(const std::wstring &tokenName, int tokenType);

                            virtual std::set<ParseTree*> *evaluate(ParseTree *t) override;

                        private:
                            void InitializeInstanceFields();
                        };

                    }
                }
            }
        }
    }
}

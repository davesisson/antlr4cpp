#pragma once

#include "XPathElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/ParseTree.h"
#include <vector>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {

                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::Tree;
                        using org::antlr::v4::runtime::tree::Trees;


                        class XPathWildcardElement : public XPathElement {
                        public:
                            XPathWildcardElement();

                            virtual Collection<ParseTree*> *evaluate(ParseTree *const t) override;
                        };

                    }
                }
            }
        }
    }
}

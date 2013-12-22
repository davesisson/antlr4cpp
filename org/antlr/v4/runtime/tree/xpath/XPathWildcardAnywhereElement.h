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
                        using org::antlr::v4::runtime::tree::Trees;


                        class XPathWildcardAnywhereElement : public XPathElement {
                        public:
                            XPathWildcardAnywhereElement();

                            virtual Collection<ParseTree*> *evaluate(ParseTree *t) override;
                        };

                    }
                }
            }
        }
    }
}

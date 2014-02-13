#pragma once

#include "XPathElement.h"
#include "Declarations.h"
#include <vector>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {


                        class XPathWildcardElement : public XPathElement {
                        public:
                            XPathWildcardElement();

                            virtual std::vector<ParseTree*> *evaluate(ParseTree *const t) override;
                        };

                    }
                }
            }
        }
    }
}

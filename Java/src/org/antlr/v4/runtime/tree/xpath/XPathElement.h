#pragma once

#include "Java/src/org/antlr/v4/runtime/tree/ParseTree.h"
#include <string>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {

                        using org::antlr::v4::runtime::tree::ParseTree;


                        class XPathElement {
                        protected:
                            std::wstring nodeName;
                            bool invert;

                            /// <summary>
                            /// Construct element like /ID or or ID or "/*" etc...
                            ///  op is null if just node
                            /// </summary>
                        public:
                            XPathElement(const std::wstring &nodeName);

                            /// <summary>
                            /// Given tree rooted at t return all nodes matched by this path element </summary>
                            virtual Collection<ParseTree*> *evaluate(ParseTree *t) = 0;

                            virtual std::wstring toString() override;

                        private:
                            void InitializeInstanceFields();
                        };

                    }
                }
            }
        }
    }
}

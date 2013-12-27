#pragma once

#include <string>
#include <set>

class ParseTree;

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {



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
                            virtual std::set<ParseTree*> *evaluate(ParseTree *t) = 0;

                            virtual std::wstring toString();

                        private:
                            void InitializeInstanceFields();
                        };

                    }
                }
            }
        }
    }
}

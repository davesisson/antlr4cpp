#include "XPathElement.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::tree::ParseTree;

                        XPathElement::XPathElement(const std::wstring &nodeName) {
                            InitializeInstanceFields();
                            this->nodeName = nodeName;
                        }

                        std::wstring XPathElement::toString() {
                            std::wstring inv = invert ? L"!" : L"";
                            return getClass()->getSimpleName() + std::wstring(L"[") + inv + nodeName + std::wstring(L"]");
                        }

                        void XPathElement::InitializeInstanceFields() {
                            invert = false;
                        }
                    }
                }
            }
        }
    }
}

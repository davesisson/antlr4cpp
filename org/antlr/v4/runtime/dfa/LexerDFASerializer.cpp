#include "LexerDFASerializer.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace dfa {
                    using org::antlr::v4::runtime::misc::NotNull;

                    LexerDFASerializer::LexerDFASerializer(DFA *dfa) : DFASerializer(dfa, nullptr) {
                    }

                    std::wstring LexerDFASerializer::getEdgeLabel(int i) {
                        return std::wstring(L"'") + static_cast<wchar_t>(i) + std::wstring(L"'");
                    }
                }
            }
        }
    }
}

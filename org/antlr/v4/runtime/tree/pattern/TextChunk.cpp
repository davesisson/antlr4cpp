#include "TextChunk.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::misc::NotNull;

                        TextChunk::TextChunk(const std::wstring &text) : text(text) {
                            if (text == L"") {
                                throw IllegalArgumentException(L"text cannot be null");
                            }

                        }

                        std::wstring TextChunk::getText() {
                            return text;
                        }

                        std::wstring TextChunk::toString() {
                            return std::wstring(L"'") + text + std::wstring(L"'");
                        }
                    }
                }
            }
        }
    }
}

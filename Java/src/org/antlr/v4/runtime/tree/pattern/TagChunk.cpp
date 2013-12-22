#include "TagChunk.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::misc::NotNull;
                        using org::antlr::v4::runtime::misc::Nullable;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                        TagChunk::TagChunk(const std::wstring &tag) {
                        }

                        TagChunk::TagChunk(const std::wstring &label, const std::wstring &tag) : tag(tag), label(label) {
                            if (tag == L"" || tag.isEmpty()) {
                                throw IllegalArgumentException(L"tag cannot be null or empty");
                            }

                        }

                        std::wstring TagChunk::getTag() {
                            return tag;
                        }

                        std::wstring TagChunk::getLabel() {
                            return label;
                        }

                        std::wstring TagChunk::toString() {
                            if (label != L"") {
                                return label + std::wstring(L":") + tag;
                            }

                            return tag;
                        }
                    }
                }
            }
        }
    }
}

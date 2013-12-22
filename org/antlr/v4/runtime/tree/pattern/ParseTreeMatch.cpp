#include "ParseTreeMatch.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::misc::MultiMap;
                        using org::antlr::v4::runtime::misc::NotNull;
                        using org::antlr::v4::runtime::misc::Nullable;
                        using org::antlr::v4::runtime::tree::ParseTree;

                        ParseTreeMatch::ParseTreeMatch(ParseTree *tree, ParseTreePattern *pattern, MultiMap<std::wstring, ParseTree*> *labels, ParseTree *mismatchedNode) : tree(tree), pattern(pattern), labels(labels), mismatchedNode(mismatchedNode) {
                            if (tree == nullptr) {
                                throw IllegalArgumentException(L"tree cannot be null");
                            }

                            if (pattern == nullptr) {
                                throw IllegalArgumentException(L"pattern cannot be null");
                            }

                            if (labels == nullptr) {
                                throw IllegalArgumentException(L"labels cannot be null");
                            }

                        }

                        org::antlr::v4::runtime::tree::ParseTree *ParseTreeMatch::get(const std::wstring &label) {
                            std::vector<ParseTree*> parseTrees = labels->get(label);
                            if (parseTrees.empty()) {
                                return nullptr;
                            }

                            return parseTrees[parseTrees.size() - 1]; // return last if multiple
                        }

                        std::vector<ParseTree*> ParseTreeMatch::getAll(const std::wstring &label) {
                            std::vector<ParseTree*> nodes = labels->get(label);
                            if (nodes.empty()) {
                                return Collections::emptyList();
                            }

                            return nodes;
                        }

                        org::antlr::v4::runtime::misc::MultiMap<std::wstring, ParseTree*> *ParseTreeMatch::getLabels() {
                            return labels;
                        }

                        org::antlr::v4::runtime::tree::ParseTree *ParseTreeMatch::getMismatchedNode() {
                            return mismatchedNode;
                        }

                        bool ParseTreeMatch::succeeded() {
                            return mismatchedNode == nullptr;
                        }

                        org::antlr::v4::runtime::tree::pattern::ParseTreePattern *ParseTreeMatch::getPattern() {
                            return pattern;
                        }

                        org::antlr::v4::runtime::tree::ParseTree *ParseTreeMatch::getTree() {
                            return tree;
                        }

                        std::wstring ParseTreeMatch::toString() {
                            return std::wstring::format(L"Match %s; found %d labels", succeeded() ? L"succeeded" : L"failed", getLabels()->size());
                        }
                    }
                }
            }
        }
    }
}

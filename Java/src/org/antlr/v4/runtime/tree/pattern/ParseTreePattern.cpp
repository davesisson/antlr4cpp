#include "ParseTreePattern.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPath.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::misc::NotNull;
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::xpath::XPath;

                        ParseTreePattern::ParseTreePattern(ParseTreePatternMatcher *matcher, const std::wstring &pattern, int patternRuleIndex, ParseTree *patternTree) : patternRuleIndex(patternRuleIndex), pattern(pattern), patternTree(patternTree), matcher(matcher) {
                        }

                        org::antlr::v4::runtime::tree::pattern::ParseTreeMatch *ParseTreePattern::match(ParseTree *tree) {
                            return matcher->match(tree, this);
                        }

                        bool ParseTreePattern::matches(ParseTree *tree) {
                            return matcher->match(tree, this)->succeeded();
                        }

                        std::vector<ParseTreeMatch*> ParseTreePattern::findAll(ParseTree *tree, const std::wstring &xpath) {
                            Collection<ParseTree*> *subtrees = XPath::findAll(tree, xpath, matcher->getParser());
                            std::vector<ParseTreeMatch*> matches = std::vector<ParseTreeMatch*>();
                            for (auto t : subtrees) {
                                ParseTreeMatch *match = match(t);
                                if (match->succeeded()) {
                                    matches.push_back(match);
                                }
                            }
                            return matches;
                        }

                        org::antlr::v4::runtime::tree::pattern::ParseTreePatternMatcher *ParseTreePattern::getMatcher() {
                            return matcher;
                        }

                        std::wstring ParseTreePattern::getPattern() {
                            return pattern;
                        }

                        int ParseTreePattern::getPatternRuleIndex() {
                            return patternRuleIndex;
                        }

                        org::antlr::v4::runtime::tree::ParseTree *ParseTreePattern::getPatternTree() {
                            return patternTree;
                        }
                    }
                }
            }
        }
    }
}

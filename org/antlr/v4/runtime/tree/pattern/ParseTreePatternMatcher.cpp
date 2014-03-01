#include "ParseTreePatternMatcher.h"
#include "Java/src/org/antlr/v4/runtime/ListTokenSource.h"
#include "Java/src/org/antlr/v4/runtime/CommonTokenStream.h"
#include "Java/src/org/antlr/v4/runtime/ParserInterpreter.h"
#include "Java/src/org/antlr/v4/runtime/tree/TerminalNode.h"
#include "Java/src/org/antlr/v4/runtime/tree/pattern/TokenTagToken.h"
#include "Java/src/org/antlr/v4/runtime/ParserRuleContext.h"
#include "Java/src/org/antlr/v4/runtime/tree/RuleNode.h"
#include "Java/src/org/antlr/v4/runtime/tree/pattern/TagChunk.h"
#include "Java/src/org/antlr/v4/runtime/tree/pattern/TextChunk.h"
#include "Java/src/org/antlr/v4/runtime/ANTLRInputStream.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace pattern {
                        using org::antlr::v4::runtime::ANTLRInputStream;
                        using org::antlr::v4::runtime::CommonTokenStream;
                        using org::antlr::v4::runtime::Lexer;
                        using org::antlr::v4::runtime::ListTokenSource;
                        using org::antlr::v4::runtime::Parser;
                        using org::antlr::v4::runtime::ParserInterpreter;
                        using org::antlr::v4::runtime::ParserRuleContext;
                        using org::antlr::v4::runtime::Token;
                        using org::antlr::v4::runtime::atn::ATN;
                        using org::antlr::v4::runtime::misc::MultiMap;
                        using org::antlr::v4::runtime::misc::NotNull;
                        using org::antlr::v4::runtime::misc::Nullable;
                        using org::antlr::v4::runtime::tree::ParseTree;
                        using org::antlr::v4::runtime::tree::RuleNode;
                        using org::antlr::v4::runtime::tree::TerminalNode;

                        ParseTreePatternMatcher::CannotInvokeStartRule::CannotInvokeStartRule(std::exception e) : RuntimeException(e) {
                        }

                        ParseTreePatternMatcher::ParseTreePatternMatcher(Lexer *lexer, Parser *parser) : lexer(lexer), parser(parser) {
                            InitializeInstanceFields();
                        }

                        void ParseTreePatternMatcher::setDelimiters(const std::wstring &start, const std::wstring &stop, const std::wstring &escapeLeft) {
                            if (start == L"" || start.isEmpty()) {
                                throw IllegalArgumentException(L"start cannot be null or empty");
                            }

                            if (stop == L"" || stop.isEmpty()) {
                                throw IllegalArgumentException(L"stop cannot be null or empty");
                            }

                            this->start = start;
                            this->stop = stop;
                            this->escape = escapeLeft;
                        }

                        bool ParseTreePatternMatcher::matches(ParseTree *tree, const std::wstring &pattern, int patternRuleIndex) {
                            ParseTreePattern *p = compile(pattern, patternRuleIndex);
                            return matches(tree, p);
                        }

                        bool ParseTreePatternMatcher::matches(ParseTree *tree, ParseTreePattern *pattern) {
                            MultiMap<std::wstring, ParseTree*> *labels = new MultiMap<std::wstring, ParseTree*>();
                            ParseTree *mismatchedNode = matchImpl(tree, pattern->getPatternTree(), labels);
                            return mismatchedNode == nullptr;
                        }

                        org::antlr::v4::runtime::tree::pattern::ParseTreeMatch *ParseTreePatternMatcher::match(ParseTree *tree, const std::wstring &pattern, int patternRuleIndex) {
                            ParseTreePattern *p = compile(pattern, patternRuleIndex);
                            return match(tree, p);
                        }

                        org::antlr::v4::runtime::tree::pattern::ParseTreeMatch *ParseTreePatternMatcher::match(ParseTree *tree, ParseTreePattern *pattern) {
                            MultiMap<std::wstring, ParseTree*> *labels = new MultiMap<std::wstring, ParseTree*>();
                            ParseTree *mismatchedNode = matchImpl(tree, pattern->getPatternTree(), labels);
                            return new ParseTreeMatch(tree, pattern, labels, mismatchedNode);
                        }

                        org::antlr::v4::runtime::tree::pattern::ParseTreePattern *ParseTreePatternMatcher::compile(const std::wstring &pattern, int patternRuleIndex) {
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: java.util.List<? extends org.antlr.v4.runtime.Token> tokenList = tokenize(pattern);
                            std::vector<? extends Token> tokenList = tokenize(pattern);
                            ListTokenSource *tokenSrc = new ListTokenSource(tokenList);
                            CommonTokenStream *tokens = new CommonTokenStream(tokenSrc);

                            ParserInterpreter *parserInterp = new ParserInterpreter(parser->getGrammarFileName(), Arrays::asList(parser->getTokenNames()), Arrays::asList(parser->getRuleNames()), parser->getATNWithBypassAlts(), tokens);

                            ParseTree *tree = nullptr;
                            try {
                                tree = parserInterp->parse(patternRuleIndex);
                                                //			System.out.println("pattern tree = "+tree.toStringTree(parserInterp));
                            } catch (std::exception &e) {
                                throw CannotInvokeStartRule(e);
                            }

                            return new ParseTreePattern(this, pattern, patternRuleIndex, tree);
                        }

                        org::antlr::v4::runtime::Lexer *ParseTreePatternMatcher::getLexer() {
                            return lexer;
                        }

                        org::antlr::v4::runtime::Parser *ParseTreePatternMatcher::getParser() {
                            return parser;
                        }

                        org::antlr::v4::runtime::tree::ParseTree *ParseTreePatternMatcher::matchImpl(ParseTree *tree, ParseTree *patternTree, MultiMap<std::wstring, ParseTree*> *labels) {
                            if (tree == nullptr) {
                                throw IllegalArgumentException(L"tree cannot be null");
                            }

                            if (patternTree == nullptr) {
                                throw IllegalArgumentException(L"patternTree cannot be null");
                            }

                            // x and <ID>, x and y, or x and x; or could be mismatched types
                            if (dynamic_cast<TerminalNode*>(tree) != nullptr && dynamic_cast<TerminalNode*>(patternTree) != nullptr) {
                                TerminalNode *t1 = static_cast<TerminalNode*>(tree);
                                TerminalNode *t2 = static_cast<TerminalNode*>(patternTree);
                                ParseTree *mismatchedNode = nullptr;
                                // both are tokens and they have same type
                                if (t1->getSymbol()->getType() == t2->getSymbol()->getType()) {
                                    if (dynamic_cast<TokenTagToken*>(t2->getSymbol()) != nullptr) { // x and <ID>
                                        TokenTagToken *tokenTagToken = static_cast<TokenTagToken*>(t2->getSymbol());
                                        // track label->list-of-nodes for both token name and label (if any)
                                        labels->map(tokenTagToken->getTokenName(), tree);
                                        if (tokenTagToken->getLabel() != L"") {
                                            labels->map(tokenTagToken->getLabel(), tree);
                                        }
                                    } else if (t1->getText() == t2->getText()) {
                                        // x and x
                                    } else {
                                        // x and y
                                        if (mismatchedNode == nullptr) {
                                            mismatchedNode = t1;
                                        }
                                    }
                                } else {
                                    if (mismatchedNode == nullptr) {
                                        mismatchedNode = t1;
                                    }
                                }

                                return mismatchedNode;
                            }

                            if (dynamic_cast<ParserRuleContext*>(tree) != nullptr && dynamic_cast<ParserRuleContext*>(patternTree) != nullptr) {
                                ParserRuleContext *r1 = static_cast<ParserRuleContext*>(tree);
                                ParserRuleContext *r2 = static_cast<ParserRuleContext*>(patternTree);
                                ParseTree *mismatchedNode = nullptr;
                                // (expr ...) and <expr>
                                RuleTagToken *ruleTagToken = getRuleTagToken(r2);
                                if (ruleTagToken != nullptr) {
                                    ParseTreeMatch *m = nullptr;
                                    if (r1->getRuleContext()->getRuleIndex() == r2->getRuleContext()->getRuleIndex()) {
                                        // track label->list-of-nodes for both rule name and label (if any)
                                        labels->map(ruleTagToken->getRuleName(), tree);
                                        if (ruleTagToken->getLabel() != L"") {
                                            labels->map(ruleTagToken->getLabel(), tree);
                                        }
                                    } else {
                                        if (mismatchedNode == nullptr) {
                                            mismatchedNode = r1;
                                        }
                                    }

                                    return mismatchedNode;
                                }

                                // (expr ...) and (expr ...)
                                if (r1->getChildCount() != r2->getChildCount()) {
                                    if (mismatchedNode == nullptr) {
                                        mismatchedNode = r1;
                                    }

                                    return mismatchedNode;
                                }

                                int n = r1->getChildCount();
                                for (int i = 0; i < n; i++) {
                                    ParseTree *childMatch = matchImpl(r1->getChild(i), patternTree->getChild(i), labels);
                                    if (childMatch != nullptr) {
                                        return childMatch;
                                    }
                                }

                                return mismatchedNode;
                            }

                            // if nodes aren't both tokens or both rule nodes, can't match
                            return tree;
                        }

                        org::antlr::v4::runtime::tree::pattern::RuleTagToken *ParseTreePatternMatcher::getRuleTagToken(ParseTree *t) {
                            if (dynamic_cast<RuleNode*>(t) != nullptr) {
                                RuleNode *r = static_cast<RuleNode*>(t);
                                if (r->getChildCount() == 1 && dynamic_cast<TerminalNode*>(r->getChild(0)) != nullptr) {
                                    TerminalNode *c = static_cast<TerminalNode*>(r->getChild(0));
                                    if (dynamic_cast<RuleTagToken*>(c->getSymbol()) != nullptr) {
                                                //					System.out.println("rule tag subtree "+t.toStringTree(parser));
                                        return static_cast<RuleTagToken*>(c->getSymbol());
                                    }
                                }
                            }
                            return nullptr;
                        }

                        std::vector<? extends Token> ParseTreePatternMatcher::tokenize(const std::wstring &pattern) {
                            // split pattern into chunks: sea (raw input) and islands (<ID>, <expr>)
                            std::vector<Chunk*> chunks = split(pattern);

                            // create token stream from text and tags
                            std::vector<Token*> tokens = std::vector<Token*>();
                            for (auto chunk : chunks) {
                                if (dynamic_cast<TagChunk*>(chunk) != nullptr) {
                                    TagChunk *tagChunk = static_cast<TagChunk*>(chunk);
                                    // add special rule token or conjure up new token from name
                                    if (isupper(tagChunk->getTag()[0])) {
                                        int ttype = parser->getTokenType(tagChunk->getTag());
                                        if (ttype == Token::INVALID_TYPE) {
                                            throw IllegalArgumentException(std::wstring(L"Unknown token ") + tagChunk->getTag() + std::wstring(L" in pattern: ") + pattern);
                                        }
                                        TokenTagToken *t = new TokenTagToken(tagChunk->getTag(), ttype, tagChunk->getLabel());
                                        tokens.push_back(t);
                                    } else if (islower(tagChunk->getTag()[0])) {
                                        int ruleIndex = parser->getRuleIndex(tagChunk->getTag());
                                        if (ruleIndex == -1) {
                                            throw IllegalArgumentException(std::wstring(L"Unknown rule ") + tagChunk->getTag() + std::wstring(L" in pattern: ") + pattern);
                                        }
                                        int ruleImaginaryTokenType = parser->getATNWithBypassAlts()->ruleToTokenType[ruleIndex];
                                        tokens.push_back(new RuleTagToken(tagChunk->getTag(), ruleImaginaryTokenType, tagChunk->getLabel()));
                                    } else {
                                        throw IllegalArgumentException(std::wstring(L"invalid tag: ") + tagChunk->getTag() + std::wstring(L" in pattern: ") + pattern);
                                    }
                                } else {
                                    TextChunk *textChunk = static_cast<TextChunk*>(chunk);
                                    ANTLRInputStream *in_Renamed = new ANTLRInputStream(textChunk->getText());
                                    lexer->setInputStream(in_Renamed);
                                    Token *t = lexer->nextToken();
                                    while (t->getType() != Token::EOF) {
                                        tokens.push_back(t);
                                        t = lexer->nextToken();
                                    }
                                }
                            }

                                                //		System.out.println("tokens="+tokens);
                            return tokens;
                        }

                        std::vector<Chunk*> ParseTreePatternMatcher::split(const std::wstring &pattern) {
                            int p = 0;
                            int n = pattern.length();
                            std::vector<Chunk*> chunks = std::vector<Chunk*>();
                            StringBuilder *buf = new StringBuilder();
                            // find all start and stop indexes first, then collect
                            std::vector<int> starts = std::vector<int>();
                            std::vector<int> stops = std::vector<int>();
                            while (p < n) {
                                if (p == pattern.find(escape + start,p)) {
                                    p += escape.length() + start.length();
                                } else if (p == pattern.find(escape + stop,p)) {
                                    p += escape.length() + stop.length();
                                } else if (p == pattern.find(start,p)) {
                                    starts.push_back(p);
                                    p += start.length();
                                } else if (p == pattern.find(stop,p)) {
                                    stops.push_back(p);
                                    p += stop.length();
                                } else {
                                    p++;
                                }
                            }

                                                //		System.out.println("");
                                                //		System.out.println(starts);
                                                //		System.out.println(stops);
                            if (starts.size() > stops.size()) {
                                throw IllegalArgumentException(std::wstring(L"unterminated tag in pattern: ") + pattern);
                            }

                            if (starts.size() < stops.size()) {
                                throw IllegalArgumentException(std::wstring(L"missing start tag in pattern: ") + pattern);
                            }

                            int ntags = starts.size();
                            for (int i = 0; i < ntags; i++) {
                                if (starts[i] >= stops[i]) {
                                    throw IllegalArgumentException(std::wstring(L"tag delimiters out of order in pattern: ") + pattern);
                                }
                            }

                            // collect into chunks now
                            if (ntags == 0) {
                                std::wstring text = pattern.substr(0, n);
                                chunks.push_back(new TextChunk(text));
                            }

                            if (ntags > 0 && starts[0] > 0) { // copy text up to first tag into chunks
                                std::wstring text = pattern.substr(0, starts[0]);
                                chunks.push_back(new TextChunk(text));
                            }
                            for (int i = 0; i < ntags; i++) {
                                // copy inside of <tag>
                                std::wstring tag = pattern.substr(starts[i] + start.length(), stops[i] - (starts[i] + start.length()));
                                std::wstring ruleOrToken = tag;
                                std::wstring label = L"";
                                int colon = tag.find(L':');
                                if (colon >= 0) {
                                    label = tag.substr(0,colon);
                                    ruleOrToken = tag.substr(colon + 1, tag.length() - (colon + 1));
                                }
                                chunks.push_back(new TagChunk(label, ruleOrToken));
                                if (i + 1 < ntags) {
                                    // copy from end of <tag> to start of next
                                    std::wstring text = pattern.substr(stops[i] + stop.length(), starts[i + 1] - (stops[i] + stop.length()));
                                    chunks.push_back(new TextChunk(text));
                                }
                            }
                            if (ntags > 0) {
                                int afterLastTag = stops[ntags - 1] + stop.length();
                                if (afterLastTag < n) { // copy text from end of last tag to end
                                    std::wstring text = pattern.substr(afterLastTag, n - afterLastTag);
                                    chunks.push_back(new TextChunk(text));
                                }
                            }

                            // strip out the escape sequences from text chunks but not tags
                            for (int i = 0; i < chunks.size(); i++) {
                                Chunk *c = chunks[i];
                                if (dynamic_cast<TextChunk*>(c) != nullptr) {
                                    TextChunk *tc = static_cast<TextChunk*>(c);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'replace' method:
                                    std::wstring unescaped = tc->getText().replace(escape, L"");
                                    if (unescaped.length() < tc->getText().length()) {
                                        chunks[i] = new TextChunk(unescaped);
                                    }
                                }
                            }

                            return chunks;
                        }

                        void ParseTreePatternMatcher::InitializeInstanceFields() {
                            start = L"<";
                            stop = L">";
                            escape = L"\\";
                        }
                    }
                }
            }
        }
    }
}

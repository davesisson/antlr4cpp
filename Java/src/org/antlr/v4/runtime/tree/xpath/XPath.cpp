#include "XPath.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathLexerErrorListener.h"
#include "Java/src/org/antlr/v4/runtime/CommonTokenStream.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathWildcardAnywhereElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathWildcardElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathTokenAnywhereElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathTokenElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathRuleAnywhereElement.h"
#include "Java/src/org/antlr/v4/runtime/tree/xpath/XPathRuleElement.h"
#include "Java/src/org/antlr/v4/runtime/ParserRuleContext.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {
                        using org::antlr::v4::runtime::ANTLRInputStream;
                        using org::antlr::v4::runtime::CommonTokenStream;
                        using org::antlr::v4::runtime::LexerNoViableAltException;
                        using org::antlr::v4::runtime::Parser;
                        using org::antlr::v4::runtime::ParserRuleContext;
                        using org::antlr::v4::runtime::Token;
                        using org::antlr::v4::runtime::tree::ParseTree;
const std::wstring XPath::WILDCARD = L"*";
const std::wstring XPath::NOT = L"!";

                        XPath::XPath(Parser *parser, const std::wstring &path) {
                            this->parser = parser;
                            this->path = path;
                            elements = split(path);
                                                //		System.out.println(Arrays.toString(elements));
                        }

                        XPathElement *XPath::split(const std::wstring &path) {
                            ANTLRInputStream *in_Renamed;
                            try {
                                in_Renamed = new ANTLRInputStream(new StringReader(path));
                            } catch (IOException ioe) {
                                throw IllegalArgumentException(std::wstring(L"Could not read path: ") + path, ioe);
                            }
                            XPathLexer *lexer = new XPathLexerAnonymousInnerClassHelper(this, in_Renamed);
                            lexer->removeErrorListeners();
                            lexer->addErrorListener(new XPathLexerErrorListener());
                            CommonTokenStream *tokenStream = new CommonTokenStream(lexer);
                            try {
                                tokenStream->fill();
                            } catch (LexerNoViableAltException e) {
                                int pos = lexer->getCharPositionInLine();
                                std::wstring msg = std::wstring(L"Invalid tokens or characters at index ") + pos + std::wstring(L" in path '") + path + std::wstring(L"'");
                                throw IllegalArgumentException(msg, e);
                            }

                            std::vector<Token*> tokens = tokenStream->getTokens();
                                                //		System.out.println("path="+path+"=>"+tokens);
                            std::vector<XPathElement*> elements = std::vector<XPathElement*>();
                            int n = tokens.size();
                            int i = 0;
                            while (i < n) {
                                Token *el = tokens[i];
                                Token *next = nullptr;
                                switch (el->getType()) {
                                    case XPathLexer::ROOT :
                                    case XPathLexer::ANYWHERE :
                                        bool anywhere = el->getType() == XPathLexer::ANYWHERE;
                                        i++;
                                        next = tokens[i];
                                        bool invert = next->getType() == XPathLexer::BANG;
                                        if (invert) {
                                            i++;
                                            next = tokens[i];
                                        }
                                        XPathElement *pathElement = getXPathElement(next, anywhere);
                                        pathElement->invert = invert;
                                        elements.push_back(pathElement);
                                        i++;
                                        break;

                                    case XPathLexer::TOKEN_REF :
                                    case XPathLexer::RULE_REF :
                                    case XPathLexer::WILDCARD :
                                        elements.push_back(getXPathElement(el, false));
                                        i++;
                                        break;

                                    case Token::EOF :
                                        goto loopBreak;

                                    default :
                                        throw IllegalArgumentException(std::wstring(L"Unknowth path element ") + el);
                                }
                                                    loopContinue:
                            }
                                                loopBreak:
                            return elements.toArray(new XPathElement[0]);
                        }

                        XPath::XPathLexerAnonymousInnerClassHelper::XPathLexerAnonymousInnerClassHelper(XPath *outerInstance, ANTLRInputStream *in_Renamed) : XPathLexer(in_Renamed) {
                            this->outerInstance = outerInstance;
                        }

                        void XPath::XPathLexerAnonymousInnerClassHelper::recover(LexerNoViableAltException *e) {
                            throw e;
                        }

                        org::antlr::v4::runtime::tree::xpath::XPathElement *XPath::getXPathElement(Token *wordToken, bool anywhere) {
                            if (wordToken->getType() == Token::EOF) {
                                throw IllegalArgumentException(L"Missing path element at end of path");
                            }
                            std::wstring word = wordToken->getText();
                            int ttype = parser->getTokenType(word);
                            int ruleIndex = parser->getRuleIndex(word);
                            switch (wordToken->getType()) {
                                case XPathLexer::WILDCARD :
                                    return anywhere ? new XPathWildcardAnywhereElement() : new XPathWildcardElement();
                                case XPathLexer::TOKEN_REF :
                                case XPathLexer::STRING :
                                    if (ttype == Token::INVALID_TYPE) {
                                        throw IllegalArgumentException(word + std::wstring(L" at index ") + wordToken->getStartIndex() + std::wstring(L" isn't a valid token name"));
                                    }
                                    return anywhere ? new XPathTokenAnywhereElement(word, ttype) : new XPathTokenElement(word, ttype);
                                default :
                                    if (ruleIndex == -1) {
                                        throw IllegalArgumentException(word + std::wstring(L" at index ") + wordToken->getStartIndex() + std::wstring(L" isn't a valid rule name"));
                                    }
                                    return anywhere ? new XPathRuleAnywhereElement(word, ruleIndex) : new XPathRuleElement(word, ruleIndex);
                            }
                        }

                        Collection<ParseTree*> *XPath::findAll(ParseTree *tree, const std::wstring &xpath, Parser *parser) {
                            XPath *p = new XPath(parser, xpath);
                            return p->evaluate(tree);
                        }

                        Collection<ParseTree*> *XPath::evaluate(ParseTree *const t) {
                            ParserRuleContext *dummyRoot = new ParserRuleContext();
                            dummyRoot->children = new ArrayListAnonymousInnerClassHelper(this, t); // don't set t's parent.

                            Collection<ParseTree*> *work = std::vector<ParseTree*>();
                            work->add(dummyRoot);

                            int i = 0;
                            while (i < elements->length) {
                                Collection<ParseTree*> *next = std::vector<ParseTree*>();
                                for (auto node : work) {
                                    if (node->getChildCount() > 0) {
                                        // only try to match next element if it has children
                                        // e.g., //func/*/stat might have a token node for which
                                        // we can't go looking for stat nodes.
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: java.util.Collection<? extends org.antlr.v4.runtime.tree.ParseTree> matching = elements[i].evaluate(node);
                                        Collection<? extends ParseTree> *matching = elements[i]->evaluate(node);
                                        next->addAll(matching);
                                    }
                                }
                                i++;
                                work = next;
                            }

                            return work;
                        }

                        XPath::ArrayListAnonymousInnerClassHelper::ArrayListAnonymousInnerClassHelper(XPath *outerInstance, ParseTree *t) {
                            this->outerInstance = outerInstance;
                            this->t = t;
                        }
                    }
                }
            }
        }
    }
}

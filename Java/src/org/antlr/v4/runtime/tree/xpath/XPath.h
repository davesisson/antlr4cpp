#pragma once

#include "XPathElement.h"
#include "Java/src/org/antlr/v4/runtime/Parser.h"
#include "Java/src/org/antlr/v4/runtime/ANTLRInputStream.h"
#include "Java/src/org/antlr/v4/runtime/LexerNoViableAltException.h"
#include "Java/src/org/antlr/v4/runtime/Token.h"
#include "Java/src/org/antlr/v4/runtime/tree/ParseTree.h"
#include <string>
#include <vector>

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


                        /// <summary>
                        /// Represent a subset of XPath XML path syntax for use in identifying nodes in
                        ///  parse trees.
                        /// 
                        ///  Split path into words and separators / and // via ANTLR itself then walk
                        ///  path elements from left to right.  At each separator-word pair, find set
                        ///  of nodes. Next stage uses those as work list.
                        /// 
                        ///  The basic interface is ParseTree.findAll(parser, pathString). But that is
                        ///  just shorthand for:
                        /// 
                        ///  XPath p = new XPath(parser, xpath);
                        ///  return p.evaluate(this);
                        /// 
                        ///  See {@code org.antlr.v4.test.TestXPath} for descriptions. In short, this allows
                        ///  operators:
                        /// 
                        ///      /         root
                        ///      //        anywhere
                        ///      !         invert; this must appear directly after root or anywhere operator
                        /// 
                        ///  and path elements:
                        /// 
                        ///      ID        token name
                        ///      'string'  any string literal token from the grammar
                        ///      expr      rule name
                        ///      *         wildcard matching any node
                        /// 
                        ///  Whitespace is not allowed.
                        /// </summary>
                        class XPath {
                        public:
                            static const std::wstring WILDCARD; // word not operator/separator
                            static const std::wstring NOT; // word for invert operator

                        protected:
                            std::wstring path;
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: protected XPathElement[] elements;
                            XPathElement *elements;
                            Parser *parser;

                        public:
                            XPath(Parser *parser, const std::wstring &path);

                            // TODO: check for invalid token/rule names, bad syntax

                            virtual XPathElement *split(const std::wstring &path);

                        private:
                            class XPathLexerAnonymousInnerClassHelper : public XPathLexer {
                            private:
                                XPath *const outerInstance;

                            public:
                                XPathLexerAnonymousInnerClassHelper(XPath *outerInstance, ANTLRInputStream *in_Renamed);

                                virtual void recover(LexerNoViableAltException *e);
                            };

                            /// <summary>
                            /// Convert word like * or ID or expr to a path element. anywhere is true
                            ///  if // precedes the word.
                            /// </summary>
                        protected:
                            virtual XPathElement *getXPathElement(Token *wordToken, bool anywhere);


                        public:
                            static Collection<ParseTree*> *findAll(ParseTree *tree, const std::wstring &xpath, Parser *parser);

                            /// <summary>
                            /// Return a list of all nodes starting at t as root that satisfy the path.
                            ///  The root / is relative to the node passed to evaluate().
                            /// </summary>
                            virtual Collection<ParseTree*> *evaluate(ParseTree *const t);

                        private:
                            class ArrayListAnonymousInnerClassHelper : public std::vector<ParseTree*> {
                            private:
                                XPath *const outerInstance;

                                ParseTree *t;

                            public:
                                ArrayListAnonymousInnerClassHelper(XPath *outerInstance, ParseTree *t);

                                {
                                    this->add(t);
                                }
                            };
                        };

                    }
                }
            }
        }
    }
}

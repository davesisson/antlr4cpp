#include "Trees.h"
#include "Utils.h"
#include "RuleNode.h"
#include "ErrorNode.h"
#include "TerminalNode.h"
#include "Token.h"
#include "ParserRuleContext.h"
#include "Utils.h"
//#include "TreePostScriptGenerator.h"


/*
* [The "BSD license"]
*  Copyright (c) 2013 Terence Parr
*  Copyright (c) 2013 Dan McLaughlin
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*  3. The name of the author may not be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
*  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
#ifdef TODO
                    std::wstring Trees::getPS(Tree *t, std::vector<std::wstring> &ruleNames, const std::wstring &fontName, int fontSize) {
                        TreePostScriptGenerator *psgen = new TreePostScriptGenerator(ruleNames, t, fontName, fontSize);
                        return psgen->getPS();
                    }

                    std::wstring Trees::getPS(Tree *t, std::vector<std::wstring> &ruleNames) {
                        return getPS(t, ruleNames, L"Helvetica", 11);
                    }

                    void Trees::writePS(Tree *t, std::vector<std::wstring> &ruleNames, const std::wstring &fileName, const std::wstring &fontName, int fontSize) throw(IOException) {
                        std::wstring ps = getPS(t, ruleNames, fontName, fontSize);
                        FileWriter *f = new FileWriter(fileName);
                        BufferedWriter *bw = new BufferedWriter(f);
                        try {
                            bw->write(ps);
                        } finally {
                            bw->close();
                        }
                    }
                    void Trees::writePS(Tree *t, std::vector<std::wstring> &ruleNames, const std::wstring &fileName) {
                        writePS(t, ruleNames, fileName, L"Helvetica", 11);
                    }
#endif
                    std::wstring Trees::toStringTree(Tree *t) {
                        return toStringTree(t, static_cast<std::vector<std::wstring>>(nullptr));
                    }

                    std::wstring Trees::toStringTree(Tree *t, Parser *recog) {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: String[] ruleNames = recog != nullptr ? recog.getRuleNames() : nullptr;
                        std::wstring *ruleNames = recog != nullptr ? recog->getRuleNames() : nullptr;
                        std::vector<std::wstring> ruleNamesList = ruleNames != nullptr ? Arrays::asList(ruleNames) : nullptr;
                        return toStringTree(t, ruleNamesList);
                    }

                    std::wstring Trees::toStringTree(Tree *t, std::vector<std::wstring> &ruleNames) {
                        std::wstring s = Utils::escapeWhitespace(getNodeText(t, ruleNames), false);
                        if (t->getChildCount() == 0) {
                            return s;
                        }
                        StringBuilder *buf = new StringBuilder();
                        buf->append(L"(");
                        s = Utils::escapeWhitespace(getNodeText(t, ruleNames), false);
                        buf->append(s);
                        buf->append(L' ');
                        for (int i = 0; i < t->getChildCount(); i++) {
                            if (i > 0) {
                                buf->append(L' ');
                            }
                            buf->append(toStringTree(t->getChild(i), ruleNames));
                        }
                        buf->append(L")");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    std::wstring Trees::getNodeText(Tree *t, Parser *recog) {
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: String[] ruleNames = recog != nullptr ? recog.getRuleNames() : nullptr;
                        std::wstring *ruleNames = recog != nullptr ? recog->getRuleNames() : nullptr;
                        std::vector<std::wstring> ruleNamesList = ruleNames != nullptr ? Arrays::asList(ruleNames) : nullptr;
                        return getNodeText(t, ruleNamesList);
                    }

                    std::wstring Trees::getNodeText(Tree *t, std::vector<std::wstring> &ruleNames) {
                        if (ruleNames.size() > 0) {
                            if (dynamic_cast<RuleNode*>(t) != nullptr) {
                                int ruleIndex = (static_cast<RuleNode*>(t))->getRuleContext()->getRuleIndex();
                                std::wstring ruleName = ruleNames[ruleIndex];
                                return ruleName;
                            } else if (dynamic_cast<ErrorNode*>(t) != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                return t->toString();
                            } else if (dynamic_cast<TerminalNode*>(t) != nullptr) {
                                Token *symbol = (static_cast<TerminalNode*>(t))->getSymbol();
                                if (symbol != nullptr) {
                                    std::wstring s = symbol->getText();
                                    return s;
                                }
                            }
                        }
                        // no recog for rule names
                        auto payload = t->getPayload();
                        if (dynamic_cast<Token*>(payload) != nullptr) {
                            return (static_cast<Token*>(payload))->getText();
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return t->getPayload()->toString();
                    }

                    std::vector<Tree*> Trees::getChildren(Tree *t) {
                        std::vector<Tree*> kids = std::vector<Tree*>();
                        for (int i = 0; i < t->getChildCount(); i++) {
                            kids.push_back(t->getChild(i));
                        }
                        return kids;
                    }

                    std::vector<Tree*> Trees::getAncestors(Tree *t) {
                        if (t->getParent() == nullptr) {
							return std::vector<Tree*>();// ::emptyList();
                        }
                        std::vector<Tree*> ancestors = std::vector<Tree*>();
                        t = t->getParent();
                        while (t != nullptr) {
                            ancestors.push_back(0, t); // insert at start
                            t = t->getParent();
                        }
                        return ancestors;
                    }

                    std::vector<ParseTree*> *Trees::findAllTokenNodes(ParseTree *t, int ttype) {
                        return findAllNodes(t, ttype, true);
                    }

					std::vector<ParseTree*> *Trees::findAllRuleNodes(ParseTree *t, int ruleIndex) {
                        return findAllNodes(t, ruleIndex, false);
                    }

                    std::vector<ParseTree*> Trees::findAllNodes(ParseTree *t, int index, bool findTokens) {
                        std::vector<ParseTree*> nodes = std::vector<ParseTree*>();
                        _findAllNodes(t, index, findTokens, nodes);
                        return nodes;
                    }

template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public static void _findAllNodes(ParseTree t, int index, boolean findTokens, java.util.List<? super ParseTree> nodes)
                    void Trees::_findAllNodes(ParseTree *t, int index, bool findTokens, std::vector<T1> nodes) {
                        // check this node (the root) first
                        if (findTokens && dynamic_cast<TerminalNode*>(t) != nullptr) {
                            TerminalNode *tnode = static_cast<TerminalNode*>(t);
                            if (tnode->getSymbol()->getType() == index) {
                                nodes.push_back(t);
                            }
                        } else if (!findTokens && dynamic_cast<ParserRuleContext*>(t) != nullptr) {
                            ParserRuleContext *ctx = static_cast<ParserRuleContext*>(t);
                            if (ctx->getRuleIndex() == index) {
                                nodes.push_back(t);
                            }
                        }
                        // check children
                        for (int i = 0; i < t->getChildCount(); i++) {
                            _findAllNodes(t->getChild(i), index, findTokens, nodes);
                        }
                    }

                    std::vector<ParseTree*> Trees::descendants(ParseTree *t) {
                        std::vector<ParseTree*> nodes = std::vector<ParseTree*>();
                        nodes.push_back(t);

                        int n = t->getChildCount();
                        for (int i = 0 ; i < n ; i++) {
                            nodes.addAll(descendants(t->getChild(i)));
                        }
                        return nodes;
                    }

                    Trees::Trees() {
                    }
                }
            }
        }
    }
}

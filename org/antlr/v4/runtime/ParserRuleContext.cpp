#include "ParserRuleContext.h"
#include "Java/src/org/antlr/v4/runtime/tree/TerminalNodeImpl.h"
#include "Java/src/org/antlr/v4/runtime/tree/ErrorNodeImpl.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Interval;
                using org::antlr::v4::runtime::misc::Nullable;
                using org::antlr::v4::runtime::tree::ErrorNode;
                using org::antlr::v4::runtime::tree::ErrorNodeImpl;
                using org::antlr::v4::runtime::tree::ParseTree;
                using org::antlr::v4::runtime::tree::ParseTreeListener;
                using org::antlr::v4::runtime::tree::TerminalNode;
                using org::antlr::v4::runtime::tree::TerminalNodeImpl;
                using org::antlr::v4::runtime::tree::pattern::RuleTagToken;

                ParserRuleContext::ParserRuleContext() {
                }

                void ParserRuleContext::copyFrom(ParserRuleContext *ctx) {
                    // from RuleContext
                    this->parent = ctx->parent;
                    this->invokingState = ctx->invokingState;

                    this->start = ctx->start;
                    this->stop = ctx->stop;
                }

                ParserRuleContext::ParserRuleContext(ParserRuleContext *parent, int invokingStateNumber) : RuleContext(parent, invokingStateNumber) {
                }

                void ParserRuleContext::enterRule(ParseTreeListener *listener) {
                }

                void ParserRuleContext::exitRule(ParseTreeListener *listener) {
                }

                org::antlr::v4::runtime::tree::TerminalNode *ParserRuleContext::addChild(TerminalNode *t) {
                    if (children.empty()) {
                        children = std::vector<ParseTree*>();
                    }
                    children.push_back(t);
                    return t;
                }

                org::antlr::v4::runtime::RuleContext *ParserRuleContext::addChild(RuleContext *ruleInvocation) {
                    if (children.empty()) {
                        children = std::vector<ParseTree*>();
                    }
                    children.push_back(ruleInvocation);
                    return ruleInvocation;
                }

                void ParserRuleContext::removeLastChild() {
                    if (children.size() > 0) {
                        children.pop_back();
                    }
                }

                org::antlr::v4::runtime::tree::TerminalNode *ParserRuleContext::addChild(Token *matchedToken) {
                    TerminalNodeImpl *t = new TerminalNodeImpl(matchedToken);
                    addChild(t);
                    t->parent = this;
                    return t;
                }

                org::antlr::v4::runtime::tree::ErrorNode *ParserRuleContext::addErrorNode(Token *badToken) {
                    ErrorNodeImpl *t = new ErrorNodeImpl(badToken);
                    addChild(t);
                    t->parent = this;
                    return t;
                }

                org::antlr::v4::runtime::ParserRuleContext *ParserRuleContext::getParent()
                /// <summary>
                /// Override to make type more specific </summary>
                {
                    return static_cast<ParserRuleContext*>(RuleContext::getParent());
                }

                org::antlr::v4::runtime::tree::ParseTree *ParserRuleContext::getChild(int i) {
                    return children.size() > 0 && i >= 0 && i < children.size() ? children[i] : nullptr;
                }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T> where T : org.antlr.v4.runtime.tree.ParseTree
                T ParserRuleContext::getChild(Class *ctxType, int i) {
                    if (children.empty() || i < 0 || i >= children.size()) {
                        return nullptr;
                    }

                    int j = -1; // what element have we found with ctxType?
                    for (auto o : children) {
                        if (ctxType->isInstance(o)) {
                            j++;
                            if (j == i) {
                                return ctxType->cast(o);
                            }
                        }
                    }
                    return nullptr;
                }

                org::antlr::v4::runtime::tree::TerminalNode *ParserRuleContext::getToken(int ttype, int i) {
                    if (children.empty() || i < 0 || i >= children.size()) {
                        return nullptr;
                    }

                    int j = -1; // what token with ttype have we found?
                    for (auto o : children) {
                        if (dynamic_cast<TerminalNode*>(o) != nullptr) {
                            TerminalNode *tnode = static_cast<TerminalNode*>(o);
                            Token *symbol = tnode->getSymbol();
                            if (symbol->getType() == ttype) {
                                j++;
                                if (j == i) {
                                    return tnode;
                                }
                            }
                        }
                    }

                    return nullptr;
                }

                std::vector<TerminalNode*> ParserRuleContext::getTokens(int ttype) {
                    if (children.empty()) {
                        return Collections::emptyList();
                    }

                    std::vector<TerminalNode*> tokens;
                    for (auto o : children) {
                        if (dynamic_cast<TerminalNode*>(o) != nullptr) {
                            TerminalNode *tnode = static_cast<TerminalNode*>(o);
                            Token *symbol = tnode->getSymbol();
                            if (symbol->getType() == ttype) {
                                if (tokens.empty()) {
                                    tokens = std::vector<TerminalNode*>();
                                }
                                tokens.push_back(tnode);
                            }
                        }
                    }

                    if (tokens.empty()) {
                        return Collections::emptyList();
                    }

                    return tokens;
                }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T> where T : ParserRuleContext
                T ParserRuleContext::getRuleContext(Class *ctxType, int i) {
                    return getChild(ctxType, i);
                }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T> where T : ParserRuleContext
                std::vector<T> ParserRuleContext::getRuleContexts(Class *ctxType) {
                    if (children.empty()) {
                        return Collections::emptyList();
                    }

                    std::vector<T> contexts;
                    for (auto o : children) {
                        if (ctxType->isInstance(o)) {
                            if (contexts.empty()) {
                                contexts = std::vector<T>();
                            }

                            contexts.push_back(ctxType->cast(o));
                        }
                    }

                    if (contexts.empty()) {
                        return Collections::emptyList();
                    }

                    return contexts;
                }

                int ParserRuleContext::getChildCount() {
                    return children.size() > 0 ? children.size() : 0;
                }

                org::antlr::v4::runtime::misc::Interval *ParserRuleContext::getSourceInterval() {
                    if (start == nullptr || stop == nullptr) {
                        return Interval::INVALID;
                    }
                    return Interval::of(start->getTokenIndex(), stop->getTokenIndex());
                }

                org::antlr::v4::runtime::Token *ParserRuleContext::getStart() {
                    return start;
                }

                org::antlr::v4::runtime::Token *ParserRuleContext::getStop() {
                    return stop;
                }

                std::wstring ParserRuleContext::toInfoString(Parser *recognizer) {
                    std::vector<std::wstring> rules = recognizer->getRuleInvocationStack(this);
                    std::reverse(rules.begin(), rules.end());
                    return std::wstring(L"ParserRuleContext") + rules + std::wstring(L"{") + std::wstring(L"start=") + start + std::wstring(L", stop=") + stop + L'}';
                }
            }
        }
    }
}

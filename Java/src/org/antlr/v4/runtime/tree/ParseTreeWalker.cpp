#include "ParseTreeWalker.h"
#include "Java/src/org/antlr/v4/runtime/tree/ErrorNode.h"
#include "Java/src/org/antlr/v4/runtime/tree/TerminalNode.h"
#include "Java/src/org/antlr/v4/runtime/ParserRuleContext.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    using org::antlr::v4::runtime::ParserRuleContext;
                    using org::antlr::v4::runtime::RuleContext;
ParseTreeWalker *const ParseTreeWalker::DEFAULT = new ParseTreeWalker();

                    void ParseTreeWalker::walk(ParseTreeListener *listener, ParseTree *t) {
                        if (dynamic_cast<ErrorNode*>(t) != nullptr) {
                            listener->visitErrorNode(static_cast<ErrorNode*>(t));
                            return;
                        } else if (dynamic_cast<TerminalNode*>(t) != nullptr) {
                            listener->visitTerminal(static_cast<TerminalNode*>(t));
                            return;
                        }
                        RuleNode *r = static_cast<RuleNode*>(t);
                        enterRule(listener, r);
                        int n = r->getChildCount();
                        for (int i = 0; i < n; i++) {
                            walk(listener, r->getChild(i));
                        }
                        exitRule(listener, r);
                    }

                    void ParseTreeWalker::enterRule(ParseTreeListener *listener, RuleNode *r) {
                        ParserRuleContext *ctx = static_cast<ParserRuleContext*>(r->getRuleContext());
                        listener->enterEveryRule(ctx);
                        ctx->enterRule(listener);
                    }

                    void ParseTreeWalker::exitRule(ParseTreeListener *listener, RuleNode *r) {
                        ParserRuleContext *ctx = static_cast<ParserRuleContext*>(r->getRuleContext());
                        ctx->exitRule(listener);
                        listener->exitEveryRule(ctx);
                    }
                }
            }
        }
    }
}

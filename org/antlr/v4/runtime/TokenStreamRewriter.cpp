﻿#include "TokenStreamRewriter.h"
#include "Interval.h"
#include "vectorhelper.h"
#include "Exceptions.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Interval;

                TokenStreamRewriter::RewriteOperation::RewriteOperation(TokenStreamRewriter *outerInstance, int index) : outerInstance(outerInstance) {

                    InitializeInstanceFields();
                    this->index = index;
                }

                TokenStreamRewriter::RewriteOperation::RewriteOperation(TokenStreamRewriter *outerInstance, int index, const std::wstring& text) : outerInstance(outerInstance) {

                    InitializeInstanceFields();
                    this->index = index;
                    this->text = text;
                }

                int TokenStreamRewriter::RewriteOperation::execute(std::wstring *buf) {
                    return index;
                }

                std::wstring TokenStreamRewriter::RewriteOperation::toString() {
                    std::wstring opName = getClass()->getName();
                    size_t index = opName.find(L'$');
                    opName = opName.substr(index + 1, opName.length() - (index + 1));
                    return L"<" + opName + L"@" + outerInstance->tokens->get(index)->getText() + L":\"" + text + L"\">";
                }

                void TokenStreamRewriter::RewriteOperation::InitializeInstanceFields() {
                    instructionIndex = 0;
                    index = 0;
                }

                TokenStreamRewriter::InsertBeforeOp::InsertBeforeOp(TokenStreamRewriter *outerInstance, int index, const std::wstring& text) : RewriteOperation(outerInstance, index, text), outerInstance(outerInstance) {
                }

                int TokenStreamRewriter::InsertBeforeOp::execute(std::wstring *buf) {
                    buf->append(text);
                    if (outerInstance->tokens->get(index)->getType() != Token::_EOF) {
                        buf->append(outerInstance->tokens->get(index)->getText());
                    }
                    return index + 1;
                }

                TokenStreamRewriter::ReplaceOp::ReplaceOp(TokenStreamRewriter *outerInstance, int from, int to, const std::wstring& text) : RewriteOperation(outerInstance, from,text), outerInstance(outerInstance) {

                    InitializeInstanceFields();
                    lastIndex = to;
                }

                int TokenStreamRewriter::ReplaceOp::execute(std::wstring *buf) {
                    buf->append(text);
                    return lastIndex + 1;
                }

                std::wstring TokenStreamRewriter::ReplaceOp::toString() {
                    if (text.empty()) {
                        return std::wstring(L"<DeleteOp@") + outerInstance->tokens->get(index)->getText() + std::wstring(L"..") + outerInstance->tokens->get(lastIndex)->getText() + std::wstring(L">");
                    }
                    return std::wstring(L"<ReplaceOp@") + outerInstance->tokens->get(index)->getText() + std::wstring(L"..") + outerInstance->tokens->get(lastIndex)->getText() + std::wstring(L":\"") + text + std::wstring(L"\">");
                }

                void TokenStreamRewriter::ReplaceOp::InitializeInstanceFields() {
                    lastIndex = 0;
                }

const std::wstring TokenStreamRewriter::DEFAULT_PROGRAM_NAME = L"default";

                TokenStreamRewriter::TokenStreamRewriter(TokenStream *tokens) : tokens(tokens), programs(std::unordered_map<std::wstring, std::vector<RewriteOperation*>>()), lastRewriteTokenIndexes(std::unordered_map<std::wstring, int>()) {
                    programs->insert(std::pair<std::wstring, std::vector<RewriteOperation*>>(DEFAULT_PROGRAM_NAME, VectorHelper::VectorWithReservedSize<RewriteOperation*>(PROGRAM_INIT_SIZE)));
                }

                org::antlr::v4::runtime::TokenStream *TokenStreamRewriter::getTokenStream() {
                    return tokens;
                }

                void TokenStreamRewriter::rollback(int instructionIndex) {
                    rollback(DEFAULT_PROGRAM_NAME, instructionIndex);
                }

                void TokenStreamRewriter::rollback(const std::wstring &programName, int instructionIndex) {
                    std::vector<RewriteOperation*> is = programs->at(programName);
                    if (is.size() > 0) {
                        programs->insert(std::pair<std::wstring, std::vector<RewriteOperation*> >(programName, VectorHelper::VectorSublist(is, MIN_TOKEN_INDEX, instructionIndex)));
                    }
                }

                void TokenStreamRewriter::deleteProgram() {
                    deleteProgram(DEFAULT_PROGRAM_NAME);
                }

                void TokenStreamRewriter::deleteProgram(const std::wstring &programName) {
                    rollback(programName, MIN_TOKEN_INDEX);
                }

                void TokenStreamRewriter::insertAfter(Token *t, const std::wstring& text) {
                    insertAfter(DEFAULT_PROGRAM_NAME, t, text);
                }

                void TokenStreamRewriter::insertAfter(int index, const std::wstring& text) {
                    insertAfter(DEFAULT_PROGRAM_NAME, index, text);
                }

                void TokenStreamRewriter::insertAfter(const std::wstring &programName, Token *t, const std::wstring& text) {
                    insertAfter(programName,t->getTokenIndex(), text);
                }

                void TokenStreamRewriter::insertAfter(const std::wstring &programName, int index, const std::wstring& text) {
                    // to insert after, just insert before next index (even if past end)
                    insertBefore(programName,index + 1, text);
                }

                void TokenStreamRewriter::insertBefore(Token *t, const std::wstring& text) {
                    insertBefore(DEFAULT_PROGRAM_NAME, t, text);
                }

                void TokenStreamRewriter::insertBefore(int index, const std::wstring& text) {
                    insertBefore(DEFAULT_PROGRAM_NAME, index, text);
                }

                void TokenStreamRewriter::insertBefore(const std::wstring &programName, Token *t, const std::wstring& text) {
                    insertBefore(programName, t->getTokenIndex(), text);
                }

                void TokenStreamRewriter::insertBefore(const std::wstring &programName, int index, const std::wstring& text) {
                    RewriteOperation *op = new InsertBeforeOp(this, index,text);
                    std::vector<RewriteOperation*> rewrites = getProgram(programName);
                    op->instructionIndex = rewrites.size();
                    rewrites.push_back(op);
                }

                void TokenStreamRewriter::replace(int index, const std::wstring& text) {
                    replace(DEFAULT_PROGRAM_NAME, index, index, text);
                }

                void TokenStreamRewriter::replace(int from, int to, const std::wstring& text) {
                    replace(DEFAULT_PROGRAM_NAME, from, to, text);
                }

                void TokenStreamRewriter::replace(Token *indexT, const std::wstring& text) {
                    replace(DEFAULT_PROGRAM_NAME, indexT, indexT, text);
                }

                void TokenStreamRewriter::replace(Token *from, Token *to, const std::wstring& text) {
                    replace(DEFAULT_PROGRAM_NAME, from, to, text);
                }

                void TokenStreamRewriter::replace(const std::wstring &programName, int from, int to, const std::wstring& text) {
                    if (from > to || from < 0 || to < 0 || to >= tokens->size()) {
                        throw IllegalArgumentException(L"replace: range invalid: " + std::to_wstring(from) + L".." + std::to_wstring(to) + L"(size=" + std::to_wstring(tokens->size()) + L")");
                    }
                    RewriteOperation *op = new ReplaceOp(this, from, to, text);
                    std::vector<RewriteOperation*> rewrites = getProgram(programName);
                    op->instructionIndex = rewrites.size();
                    rewrites.push_back(op);
                }

                void TokenStreamRewriter::replace(const std::wstring &programName, Token *from, Token *to, const std::wstring& text) {
                    replace(programName, from->getTokenIndex(), to->getTokenIndex(), text);
                }

                void TokenStreamRewriter::delete_Renamed(int index) {
                    delete_Renamed(DEFAULT_PROGRAM_NAME, index, index);
                }

                void TokenStreamRewriter::delete_Renamed(int from, int to) {
                    delete_Renamed(DEFAULT_PROGRAM_NAME, from, to);
                }

                void TokenStreamRewriter::delete_Renamed(Token *indexT) {
                    delete_Renamed(DEFAULT_PROGRAM_NAME, indexT, indexT);
                }

                void TokenStreamRewriter::delete_Renamed(Token *from, Token *to) {
                    delete_Renamed(DEFAULT_PROGRAM_NAME, from, to);
                }

                void TokenStreamRewriter::delete_Renamed(const std::wstring &programName, int from, int to) {
                    replace(programName,from,to,nullptr);
                }

                void TokenStreamRewriter::delete_Renamed(const std::wstring &programName, Token *from, Token *to) {
                    replace(programName,from,to,nullptr);
                }

                int TokenStreamRewriter::getLastRewriteTokenIndex() {
                    return getLastRewriteTokenIndex(DEFAULT_PROGRAM_NAME);
                }

                int TokenStreamRewriter::getLastRewriteTokenIndex(const std::wstring &programName) {
                    if (lastRewriteTokenIndexes->find(programName) == lastRewriteTokenIndexes->end()) {
                        return -1;
                    }
                    return lastRewriteTokenIndexes->at(programName);
                }

                void TokenStreamRewriter::setLastRewriteTokenIndex(const std::wstring &programName, int i) {
                    lastRewriteTokenIndexes->insert({programName, i});
                }

                std::vector<TokenStreamRewriter::RewriteOperation*> TokenStreamRewriter::getProgram(const std::wstring &name) {
                    std::vector<TokenStreamRewriter::RewriteOperation*> is = programs->at(name);
                    if (is.empty()) {
                        is = initializeProgram(name);
                    }
                    return is;
                }

                std::vector<TokenStreamRewriter::RewriteOperation*> TokenStreamRewriter::initializeProgram(const std::wstring &name) {
                    std::vector<TokenStreamRewriter::RewriteOperation*> is = VectorHelper::VectorWithReservedSize<RewriteOperation*>(PROGRAM_INIT_SIZE);
                    programs->insert({name, is});
                    return is;
                }

                std::wstring TokenStreamRewriter::getText() {
                    return getText(DEFAULT_PROGRAM_NAME, Interval::of(0,tokens->size() - 1));
                }

                std::wstring TokenStreamRewriter::getText(Interval *interval) {
                    return getText(DEFAULT_PROGRAM_NAME, interval);
                }

                std::wstring TokenStreamRewriter::getText(const std::wstring &programName, Interval *interval) {
                    std::vector<TokenStreamRewriter::RewriteOperation*> rewrites = programs->at(programName);
                    int start = interval->a;
                    int stop = interval->b;

                    // ensure start/end are in range
                    if (stop > tokens->size() - 1) {
                        stop = tokens->size() - 1;
                    }
                    if (start < 0) {
                        start = 0;
                    }

                    if (rewrites.empty() || rewrites.empty()) {
                        return tokens->getText(interval); // no instructions to execute
                    }
                    std::wstring buf;

                    // First, optimize instruction stream
                    std::unordered_map<int, TokenStreamRewriter::RewriteOperation*> *indexToOp = reduceToSingleOperationPerIndex(rewrites);

                    // Walk buffer, executing instructions and emitting tokens
                    int i = start;
                    while (i <= stop && i < tokens->size()) {
                        RewriteOperation *op = indexToOp->at(i);
                        indexToOp->erase(i); // remove so any left have index size-1
                        Token *t = tokens->get(i);
                        if (op == nullptr) {
                            // no operation at that index, just dump token
                            if (t->getType() != Token::_EOF) {
                                buf.append(t->getText());
                            }
                            i++; // move to next token
                        } else {
                            i = op->execute(&buf); // execute operation and skip
                        }
                    }

                    // include stuff after end if it's last index in buffer
                    // So, if they did an insertAfter(lastValidIndex, "foo"), include
                    // foo if end==lastValidIndex.
                    if (stop == tokens->size() - 1) {
                        // Scan any remaining operations after last token
                        // should be included (they will be inserts).
                        for (auto op : *indexToOp) {
                            if (op.second->index >= tokens->size() - 1) {
                                buf.append(op.second->text);
                            }
                        }
                    }
                    return buf;
                }

                std::unordered_map<int, TokenStreamRewriter::RewriteOperation*> *TokenStreamRewriter::reduceToSingleOperationPerIndex(std::vector<TokenStreamRewriter::RewriteOperation*> &rewrites) {
                                //		System.out.println("rewrites="+rewrites);

                    // WALK REPLACES
                    for (int i=0 ; i < rewrites.size(); ++i) {
                        TokenStreamRewriter::RewriteOperation *op = rewrites[i];
                        if (op == nullptr) {
                            continue;
                        }
                        if (!(dynamic_cast<ReplaceOp*>(op) != nullptr)) {
                            continue;
                        }
                        ReplaceOp *rop = static_cast<ReplaceOp*>(op);
                        // Wipe prior inserts within range
//ORIGINAL LINE: java.util.List<? extends InsertBeforeOp> inserts = getKindOfOps(rewrites, InsertBeforeOp.class, i);
                        std::vector<InsertBeforeOp> inserts = getKindOfOps(rewrites, InsertBeforeOp::typeid, i);
                        for (auto iop : inserts) {
                            if (iop.index == rop->index) {
                                // E.g., insert before 2, delete 2..2; update replace
                                // text to include insert before, kill insert
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete rewrites[iop.instructionIndex];
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                rop->text = iop.text + (rop->text != nullptr ? rop->text : L"");
                            } else if (iop.index > rop->index && iop.index <= rop->lastIndex) {
                                // delete insert as it's a no-op.
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete rewrites[iop.instructionIndex];
                            }
                        }
                        // Drop any prior replaces contained within
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: java.util.List<? extends ReplaceOp> prevReplaces = getKindOfOps(rewrites, ReplaceOp.class, i);
                        std::vector<? extends ReplaceOp> prevReplaces = getKindOfOps(rewrites, ReplaceOp::typeid, i);
                        for (auto prevRop : prevReplaces) {
                            if (prevRop.index >= rop->index && prevRop.lastIndex <= rop->lastIndex) {
                                // delete replace as it's a no-op.
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete rewrites[prevRop.instructionIndex];
                                continue;
                            }
                            // throw exception unless disjoint or identical
                            bool disjoint = prevRop.lastIndex < rop->index || prevRop.index > rop->lastIndex;
                            bool same = prevRop.index == rop->index && prevRop.lastIndex == rop->lastIndex;
                            // Delete special case of replace (text==null):
                            // D.i-j.u D.x-y.v	| boundaries overlap	combine to max(min)..max(right)
                            if (prevRop.text == nullptr && rop->text == nullptr && !disjoint) {
                                //System.out.println("overlapping deletes: "+prevRop+", "+rop);
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete rewrites[prevRop.instructionIndex]; // kill first delete
                                rop->index = std::min(prevRop.index, rop->index);
                                rop->lastIndex = std::max(prevRop.lastIndex, rop->lastIndex);
                                std::cout << std::wstring(L"new rop ") << rop << std::endl;
                            } else if (!disjoint && !same) {
                                throw IllegalArgumentException(std::wstring(L"replace op boundaries of ") + rop + std::wstring(L" overlap with previous ") + prevRop);
                            }
                        }
                    }

                    // WALK INSERTS
                    for (int i = 0; i < rewrites.size(); i++) {
                        RewriteOperation *op = rewrites[i];
                        if (op == nullptr) {
                            continue;
                        }
                        if (!(dynamic_cast<InsertBeforeOp*>(op) != nullptr)) {
                            continue;
                        }
                        InsertBeforeOp *iop = static_cast<InsertBeforeOp*>(rewrites[i]);
                        // combine current insert with prior if any at same index
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: java.util.List<? extends InsertBeforeOp> prevInserts = getKindOfOps(rewrites, InsertBeforeOp.class, i);
                        std::vector<? extends InsertBeforeOp> prevInserts = getKindOfOps(rewrites, InsertBeforeOp::typeid, i);
                        for (auto prevIop : prevInserts) {
                            if (prevIop.index == iop->index) { // combine objects
                                // convert to strings...we're in process of toString'ing
                                // whole token buffer so no lazy eval issue with any templates
                                iop->text = catOpText(iop->text,prevIop.text);
                                // delete redundant prior insert
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete rewrites[prevIop.instructionIndex];
                            }
                        }
                        // look for replaces where iop.index is in range; error
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: java.util.List<? extends ReplaceOp> prevReplaces = getKindOfOps(rewrites, ReplaceOp.class, i);
                        std::vector<? extends ReplaceOp> prevReplaces = getKindOfOps(rewrites, ReplaceOp::typeid, i);
                        for (auto rop : prevReplaces) {
                            if (iop->index == rop->index) {
                                rop->text = catOpText(iop->text,rop->text);
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                                delete rewrites[i]; // delete current insert
                                continue;
                            }
                            if (iop->index >= rop->index && iop->index <= rop->lastIndex) {
                                throw IllegalArgumentException(std::wstring(L"insert op ") + iop + std::wstring(L" within boundaries of previous ") + rop);
                            }
                        }
                    }
                    // System.out.println("rewrites after="+rewrites);
                   std::unordered_map<int, TokenStreamRewriter::RewriteOperation*> *m = new std::unordered_map<int, TokenStreamRewriter::RewriteOperation*>();
                    for (TokenStreamRewriter::RewriteOperation *op : rewrites) {
                        if (op == nullptr) { // ignore deleted ops
                            continue;
                        }
                        if (m->at(op->index) != nullptr) {
                            throw Error(L"should only be one op per index");
                        }
                        m->emplace(op->index, op);
                    }
                    //System.out.println("index to op: "+m);
                    return m;
                }

                std::wstring TokenStreamRewriter::catOpText(void *a, void *b) {
                    std::wstring x = L"";
                    std::wstring y = L"";
                    if (a != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        x = a->toString();
                    }
                    if (b != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        y = b->toString();
                    }
                    return x + y;
                }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T, typename T1> where T : RewriteOperation where T1 : RewriteOperation
                std::vector<? extends T> TokenStreamRewriter::getKindOfOps(std::vector<T1> rewrites, Class *kind, int before) {
                    std::vector<T> ops = std::vector<T>();
                    for (int i = 0; i < before && i < rewrites.size(); i++) {
                        TokenStreamRewriter::RewriteOperation *op = rewrites[i];
                        if (op == nullptr) { // ignore deleted
                            continue;
                        }
                        if (kind->isInstance(op)) {
                            ops.push_back(kind->cast(op));
                        }
                    }
                    return ops;
                }
            }
        }
    }
}

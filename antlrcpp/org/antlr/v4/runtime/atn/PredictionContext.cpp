#include "PredictionContext.h"
#include "EmptyPredictionContext.h"
#include "MurmurHash.h"
#include "ArrayPredictionContext.h"

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
                using misc::MurmurHash;
                
                namespace atn {
                    EmptyPredictionContext *const PredictionContext::EMPTY = new EmptyPredictionContext();
                    int PredictionContext::globalNodeCount = 0;

                    PredictionContext::PredictionContext(int cachedHashCode) : cachedHashCode(cachedHashCode), id(globalNodeCount++) {
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContext::fromRuleContext(ATN *atn, RuleContext *outerContext) {
                        if (outerContext == nullptr) {
                            outerContext = RuleContext::EMPTY;
                        }

                        // if we are in RuleContext of start rule, s, then PredictionContext
                        // is EMPTY. Nobody called us. (if we are empty, return empty)
                        if (outerContext->parent == nullptr || outerContext == RuleContext::EMPTY) {
                            return PredictionContext::EMPTY;
                        }

                        // If we have a parent, convert it to a PredictionContext graph
                        PredictionContext *parent = EMPTY;
                        parent = PredictionContext::fromRuleContext(atn, outerContext->parent);

                        ATNState *state = atn->states[outerContext->invokingState];
                        RuleTransition *transition = static_cast<RuleTransition*>(state->transition(0));
                        return SingletonPredictionContext::create(parent, transition->followState->stateNumber);
                    }

                    bool PredictionContext::isEmpty() {
                        return this == EMPTY;
                    }

                    bool PredictionContext::hasEmptyPath() {
                        return getReturnState(size() - 1) == EMPTY_RETURN_STATE;
                    }

                    int PredictionContext::hashCode() {
                        return cachedHashCode;
                    }

                    int PredictionContext::calculateEmptyHashCode() {
                        int hash = MurmurHash::initialize(INITIAL_HASH);
                        hash = MurmurHash::finish(hash, 0);
                        return hash;
                    }

                    int PredictionContext::calculateHashCode(PredictionContext *parent, int returnState) {
                        int hash = MurmurHash::initialize(INITIAL_HASH);
                        hash = MurmurHash::update(hash, parent);
                        hash = MurmurHash::update(hash, returnState);
                        hash = MurmurHash::finish(hash, 2);
                        return hash;
                    }

                    int PredictionContext::calculateHashCode(PredictionContext parents[], int returnStates[]) {
                        int hash = MurmurHash::initialize(INITIAL_HASH);

                        for (auto parent : parents) {
                            hash = MurmurHash::update(hash, parent);
                        }

                        for (auto returnState : returnStates) {
                            hash = MurmurHash::update(hash, returnState);
                        }

                        hash = MurmurHash::finish(hash, 2 * sizeof(parents) / sizeof(parents[0]));
                        return hash;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContext::merge(PredictionContext *a, PredictionContext *b, bool rootIsWildcard, DoubleKeyMap<PredictionContext*, PredictionContext*, PredictionContext*> *mergeCache) {
                        assert(a != nullptr && b != nullptr); // must be empty context, never null

                        // share same graph if both same
                        if (a == b || a->equals(b)) {
                            return a;
                        }

                        if (dynamic_cast<SingletonPredictionContext*>(a) != nullptr && dynamic_cast<SingletonPredictionContext*>(b) != nullptr) {
                            return mergeSingletons(static_cast<SingletonPredictionContext*>(a), static_cast<SingletonPredictionContext*>(b), rootIsWildcard, mergeCache);
                        }

                        // At least one of a or b is array
                        // If one is $ and rootIsWildcard, return $ as * wildcard
                        if (rootIsWildcard) {
                            if (dynamic_cast<EmptyPredictionContext*>(a) != nullptr) {
                                return a;
                            }
                            if (dynamic_cast<EmptyPredictionContext*>(b) != nullptr) {
                                return b;
                            }
                        }

                        // convert singleton so both are arrays to normalize
                        if (dynamic_cast<SingletonPredictionContext*>(a) != nullptr) {
                            a = new ArrayPredictionContext(static_cast<SingletonPredictionContext*>(a));
                        }
                        if (dynamic_cast<SingletonPredictionContext*>(b) != nullptr) {
                            b = new ArrayPredictionContext(static_cast<SingletonPredictionContext*>(b));
                        }
                        return mergeArrays(static_cast<ArrayPredictionContext*>(a), static_cast<ArrayPredictionContext*>(b), rootIsWildcard, mergeCache);
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContext::mergeSingletons(SingletonPredictionContext *a, SingletonPredictionContext *b, bool rootIsWildcard, DoubleKeyMap<PredictionContext*, PredictionContext*, PredictionContext*> *mergeCache) {
                        if (mergeCache != nullptr) {
                            PredictionContext *previous = mergeCache->get(a,b);
                            if (previous != nullptr) {
                                return previous;
                            }
                            previous = mergeCache->get(b,a);
                            if (previous != nullptr) {
                                return previous;
                            }
                        }

                        PredictionContext *rootMerge = mergeRoot(a, b, rootIsWildcard);
                        if (rootMerge != nullptr) {
                            if (mergeCache != nullptr) {
                                mergeCache->put(a, b, rootMerge);
                            }
                            return rootMerge;
                        }

                        if (a->returnState == b->returnState) { // a == b
                            PredictionContext *parent = merge(a->parent, b->parent, rootIsWildcard, mergeCache);
                            // if parent is same as existing a or b parent or reduced to a parent, return it
                            if (parent == a->parent) { // ax + bx = ax, if a=b
                                return a;
                            }
                            if (parent == b->parent) { // ax + bx = bx, if a=b
                                return b;
                            }
                            // else: ax + ay = a'[x,y]
                            // merge parents x and y, giving array node with x,y then remainders
                            // of those graphs.  dup a, a' points at merged array
                            // new joined parent so create new singleton pointing to it, a'
                            PredictionContext *a_ = SingletonPredictionContext::create(parent, a->returnState);
                            if (mergeCache != nullptr) {
                                mergeCache->put(a, b, a_);
                            }
                            return a_;
                        }
                        else { // a != b payloads differ
                            // see if we can collapse parents due to $+x parents if local ctx
                            PredictionContext *singleParent = nullptr;
                            if (a == b || (a->parent != nullptr && a->parent->equals(b->parent))) { // ax + bx = [a,b]x
                                singleParent = a->parent;
                            }
                            if (singleParent != nullptr) { // parents are same
                                // sort payloads and use same parent
                                int payloads[2] = {a->returnState, b->returnState};
                                if (a->returnState > b->returnState) {
                                    payloads[0] = b->returnState;
                                    payloads[1] = a->returnState;
                                }
                                PredictionContext parents[2] = {singleParent, singleParent};
                                PredictionContext *a_ = new ArrayPredictionContext(parents, payloads);
                                if (mergeCache != nullptr) {
                                    mergeCache->put(a, b, a_);
                                }
                                return a_;
                            }
                            // parents differ and can't merge them. Just pack together
                            // into array; can't merge.
                            // ax + by = [ax,by]
                            int payloads[2] = {a->returnState, b->returnState};
                            PredictionContext parents[2] = {a->parent, b->parent};
                            if (a->returnState > b->returnState) { // sort by payload
                                payloads[0] = b->returnState;
                                payloads[1] = a->returnState;
                                parents = new PredictionContext[] {b->parent, a->parent};
                            }
                            PredictionContext *a_ = new ArrayPredictionContext(parents, payloads);
                            if (mergeCache != nullptr) {
                                mergeCache->put(a, b, a_);
                            }
                            return a_;
                        }
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContext::mergeRoot(SingletonPredictionContext *a, SingletonPredictionContext *b, bool rootIsWildcard) {
                        if (rootIsWildcard) {
                            if (a == EMPTY) { // * + b = *
                                return EMPTY;
                            }
                            if (b == EMPTY) { // a + * = *
                                return EMPTY;
                            }
                        } else {
                            if (a == EMPTY && b == EMPTY) { // $ + $ = $
                                return EMPTY;
                            }
                            if (a == EMPTY) { // $ + x = [$,x]
                                int payloads[2] = {b->returnState, EMPTY_RETURN_STATE};
                                PredictionContext parents[2] = {b->parent, nullptr};
                                PredictionContext *joined = new ArrayPredictionContext(parents, payloads);
                                return joined;
                            }
                            if (b == EMPTY) { // x + $ = [$,x] ($ is always first if present)
                                int payloads[2] = {a->returnState, EMPTY_RETURN_STATE};
                                PredictionContext parents[2] = {a->parent, nullptr};
                                PredictionContext *joined = new ArrayPredictionContext(parents, payloads);
                                return joined;
                            }
                        }
                        return nullptr;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContext::mergeArrays(ArrayPredictionContext *a, ArrayPredictionContext *b, bool rootIsWildcard, DoubleKeyMap<PredictionContext*, PredictionContext*, PredictionContext*> *mergeCache) {
                        if (mergeCache != nullptr) {
                            PredictionContext *previous = mergeCache->get(a,b);
                            if (previous != nullptr) {
                                return previous;
                            }
                            previous = mergeCache->get(b,a);
                            if (previous != nullptr) {
                                return previous;
                            }
                        }

                        // merge sorted payloads a + b => M
                        int i = 0; // walks a
                        int j = 0; // walks b
                        int k = 0; // walks target M array

                        int mergedReturnStates[a->returnStates->length + b->returnStates->length];
                        PredictionContext mergedParents[a->returnStates->length + b->returnStates->length];
                        // walk and merge to yield mergedParents, mergedReturnStates
                        while (i < a->returnStates->length && j < b->returnStates->length) {
                            PredictionContext *a_parent = a->parents[i];
                            PredictionContext *b_parent = b->parents[j];
                            if (a->returnStates[i] == b->returnStates[j]) {
                                // same payload (stack tops are equal), must yield merged singleton
                                int payload = a->returnStates[i];
                                // $+$ = $
                                bool both$ = payload == EMPTY_RETURN_STATE && a_parent == nullptr && b_parent == nullptr;
                                bool ax_ax = (a_parent != nullptr && b_parent != nullptr) && a_parent->equals(b_parent); // ax+ax -> ax
                                if (both$ || ax_ax) {
                                    mergedParents[k] = a_parent; // choose left
                                    mergedReturnStates[k] = payload;
                                }
                                else { // ax+ay -> a'[x,y]
                                    PredictionContext *mergedParent = merge(a_parent, b_parent, rootIsWildcard, mergeCache);
                                    mergedParents[k] = mergedParent;
                                    mergedReturnStates[k] = payload;
                                }
                                i++; // hop over left one as usual
                                j++; // but also skip one in right side since we merge
                            } else if (a->returnStates[i] < b->returnStates[j]) { // copy a[i] to M
                                mergedParents[k] = a_parent;
                                mergedReturnStates[k] = a->returnStates[i];
                                i++;
                            }
                            else { // b > a, copy b[j] to M
                                mergedParents[k] = b_parent;
                                mergedReturnStates[k] = b->returnStates[j];
                                j++;
                            }
                            k++;
                        }

                        // copy over any payloads remaining in either array
                        if (i < a->returnStates->length) {
                            for (int p = i; p < a->returnStates->length; p++) {
                                mergedParents[k] = a->parents[p];
                                mergedReturnStates[k] = a->returnStates[p];
                                k++;
                            }
                        } else {
                            for (int p = j; p < b->returnStates->length; p++) {
                                mergedParents[k] = b->parents[p];
                                mergedReturnStates[k] = b->returnStates[p];
                                k++;
                            }
                        }

                        // trim merged if we combined a few that had same stack tops
                        if (k < sizeof(mergedParents) / sizeof(mergedParents[0])) { // write index < last position; trim
                            if (k == 1) { // for just one merged element, return singleton top
                                PredictionContext *a_ = SingletonPredictionContext::create(mergedParents[0], mergedReturnStates[0]);
                                if (mergeCache != nullptr) {
                                    mergeCache->put(a,b,a_);
                                }
                                return a_;
                            }
                            mergedParents = Arrays::copyOf(mergedParents, k);
                            mergedReturnStates = Arrays::copyOf(mergedReturnStates, k);
                        }

                        PredictionContext *M = new ArrayPredictionContext(mergedParents, mergedReturnStates);

                        // if we created same array as a or b, return that instead
                        // TODO: track whether this is possible above during merge sort for speed
                        if (M->equals(a)) {
                            if (mergeCache != nullptr) {
                                mergeCache->put(a,b,a);
                            }
                            return a;
                        }
                        if (M->equals(b)) {
                            if (mergeCache != nullptr) {
                                mergeCache->put(a,b,b);
                            }
                            return b;
                        }

                        combineCommonParents(mergedParents);

                        if (mergeCache != nullptr) {
                            mergeCache->put(a,b,M);
                        }
                        return M;
                    }

                    void PredictionContext::combineCommonParents(PredictionContext parents[]) {
                        Map<PredictionContext*, PredictionContext*> *uniqueParents = std::unordered_map<PredictionContext*, PredictionContext*>();

                        for (int p = 0; p < sizeof(parents) / sizeof(parents[0]); p++) {
                            PredictionContext *parent = parents[p];
                            if (!uniqueParents->containsKey(parent)) { // don't replace
                                uniqueParents->put(parent, parent);
                            }
                        }

                        for (int p = 0; p < sizeof(parents) / sizeof(parents[0]); p++) {
                            parents[p] = uniqueParents->get(parents[p]);
                        }
                    }

                    std::wstring PredictionContext::toDOTString(PredictionContext *context) {
                        if (context == nullptr) {
                            return L"";
                        }
                        StringBuilder *buf = new StringBuilder();
                        buf->append(L"digraph G {\n");
                        buf->append(L"rankdir=LR;\n");

                        std::vector<PredictionContext*> nodes = getAllContextNodes(context);
                        Collections::sort(nodes, new ComparatorAnonymousInnerClassHelper());

                        for (auto current : nodes) {
                            if (dynamic_cast<SingletonPredictionContext*>(current) != nullptr) {
                                std::wstring s = static_cast<std::wstring>(current.id);
                                buf->append(L"  s")->append(s);
                                std::wstring returnState = static_cast<std::wstring>(current.getReturnState(0));
                                if (dynamic_cast<EmptyPredictionContext*>(current) != nullptr) {
                                    returnState = L"$";
                                }
                                buf->append(L" [label=\"")->append(returnState)->append(L"\"];\n");
                                continue;
                            }
                            ArrayPredictionContext *arr = static_cast<ArrayPredictionContext*>(current);
                            buf->append(L"  s")->append(arr->id);
                            buf->append(L" [shape=box, label=\"");
                            buf->append(L"[");
                            bool first = true;
                            for (auto inv : arr->returnStates) {
                                if (!first) {
                                    buf->append(L", ");
                                }
                                if (inv == EMPTY_RETURN_STATE) {
                                    buf->append(L"$");
                                } else {
                                    buf->append(inv);
                                }
                                first = false;
                            }
                            buf->append(L"]");
                            buf->append(L"\"];\n");
                        }

                        for (auto current : nodes) {
                            if (current == EMPTY) {
                                continue;
                            }
                            for (int i = 0; i < current.size(); i++) {
                                if (current.getParent(i) == nullptr) {
                                    continue;
                                }
                                std::wstring s = static_cast<std::wstring>(current.id);
                                buf->append(L"  s")->append(s);
                                buf->append(L"->");
                                buf->append(L"s");
                                buf->append(current.getParent(i)->id);
                                if (current.size() > 1) {
                                    buf->append(std::wstring(L" [label=\"parent[") + i + std::wstring(L"]\"];\n"));
                                } else {
                                    buf->append(L";\n");
                                }
                            }
                        }

                        buf->append(L"}\n");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    PredictionContext::ComparatorAnonymousInnerClassHelper::ComparatorAnonymousInnerClassHelper() {
                    }

                    int PredictionContext::ComparatorAnonymousInnerClassHelper::compare(PredictionContext *o1, PredictionContext *o2) {
                        return o1->id - o2->id;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContext::getCachedContext(PredictionContext *context, PredictionContextCache *contextCache, IdentityHashMap<PredictionContext*, PredictionContext*> *visited) {
                        if (context->isEmpty()) {
                            return context;
                        }

                        PredictionContext *existing = visited->get(context);
                        if (existing != nullptr) {
                            return existing;
                        }

                        existing = contextCache->get(context);
                        if (existing != nullptr) {
                            visited->put(context, existing);
                            return existing;
                        }

                        bool changed = false;
                        PredictionContext parents[context->size()];
                        for (int i = 0; i < sizeof(parents) / sizeof(parents[0]); i++) {
                            PredictionContext *parent = getCachedContext(context->getParent(i), contextCache, visited);
                            if (changed || parent != context->getParent(i)) {
                                if (!changed) {
                                    parents = new PredictionContext[context->size()];
                                    for (int j = 0; j < context->size(); j++) {
                                        parents[j] = context->getParent(j);
                                    }

                                    changed = true;
                                }

                                parents[i] = parent;
                            }
                        }

                        if (!changed) {
                            contextCache->add(context);
                            visited->put(context, context);
                            return context;
                        }

                        PredictionContext *updated;
                        if (sizeof(parents) / sizeof(parents[0]) == 0) {
                            updated = EMPTY;
                        } else if (sizeof(parents) / sizeof(parents[0]) == 1) {
                            updated = SingletonPredictionContext::create(parents[0], context->getReturnState(0));
                        } else {
                            ArrayPredictionContext *arrayPredictionContext = static_cast<ArrayPredictionContext*>(context);
                            updated = new ArrayPredictionContext(parents, arrayPredictionContext->returnStates);
                        }

                        contextCache->add(updated);
                        visited->put(updated, updated);
                        visited->put(context, updated);

                        return updated;
                    }

                    std::vector<PredictionContext*> PredictionContext::getAllContextNodes(PredictionContext *context) {
                        std::vector<PredictionContext*> nodes = std::vector<PredictionContext*>();
                        Map<PredictionContext*, PredictionContext*> *visited = new IdentityHashMap<PredictionContext*, PredictionContext*>();
                        getAllContextNodes_(context, nodes, visited);
                        return nodes;
                    }

                    void PredictionContext::getAllContextNodes_(PredictionContext *context, std::vector<PredictionContext*> &nodes, Map<PredictionContext*, PredictionContext*> *visited) {
                        if (context == nullptr || visited->containsKey(context)) {
                            return;
                        }
                        visited->put(context, context);
                        nodes.push_back(context);
                        for (int i = 0; i < context->size(); i++) {
                            getAllContextNodes_(context->getParent(i), nodes, visited);
                        }
                    }

template<typename T1, typename T1>
                    std::wstring PredictionContext::toString(Recognizer<T1> *recog) {
                        return toString();
                                        //		return toString(recog, ParserRuleContext.EMPTY);
                    }

template<typename T1, typename T1>
                    std::wstring *PredictionContext::toStrings(Recognizer<T1> *recognizer, int currentState) {
                        return toStrings(recognizer, EMPTY, currentState);
                    }

template<typename T1, typename T1>
                    std::wstring *PredictionContext::toStrings(Recognizer<T1> *recognizer, PredictionContext *stop, int currentState) {
                        std::vector<std::wstring> result = std::vector<std::wstring>();

                        for (int perm = 0; ; perm++) {
                            int offset = 0;
                            bool last = true;
                            PredictionContext *p = this;
                            int stateNumber = currentState;
                            StringBuilder *localBuffer = new StringBuilder();
                            localBuffer->append(L"[");
                            while (!p->isEmpty() && p != stop) {
                                int index = 0;
                                if (p->size() > 0) {
                                    int bits = 1;
                                    while ((1 << bits) < p->size()) {
                                        bits++;
                                    }

                                    int mask = (1 << bits) - 1;
                                    index = (perm >> offset) & mask;
                                    last &= index >= p->size() - 1;
                                    if (index >= p->size()) {
                                        goto outerContinue;
                                    }
                                    offset += bits;
                                }

                                if (recognizer != nullptr) {
                                    if (localBuffer->length() > 1) {
                                        // first char is '[', if more than that this isn't the first rule
                                        localBuffer->append(L' ');
                                    }

                                    ATN *atn = recognizer->getATN();
                                    ATNState *s = atn->states[stateNumber];
                                    std::wstring ruleName = recognizer->getRuleNames()[s->ruleIndex];
                                    localBuffer->append(ruleName);
                                } else if (p->getReturnState(index) != EMPTY_RETURN_STATE) {
                                    if (!p->isEmpty()) {
                                        if (localBuffer->length() > 1) {
                                            // first char is '[', if more than that this isn't the first rule
                                            localBuffer->append(L' ');
                                        }

                                        localBuffer->append(p->getReturnState(index));
                                    }
                                }
                                stateNumber = p->getReturnState(index);
                                p = p->getParent(index);
                            }
                            localBuffer->append(L"]");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            result.push_back(localBuffer->toString());

                            if (last) {
                                break;
                            }
                            outerContinue:
                        }
                        outerBreak:

                        return result.toArray(new std::wstring[result.size()]);
                    }
                }
            }
        }
    }
}

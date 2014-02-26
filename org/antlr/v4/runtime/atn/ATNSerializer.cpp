#include "ATNSerializer.h"
#include "ATNDeserializer.h"
#include "IntervalSet.h"
#include "ATNState.h"
#include "DecisionState.h"
#include "RuleStartState.h"
#include "LoopEndState.h"
#include "BlockStartState.h"
#include "Transition.h"
#include "SetTransition.h"
#include "ATNType.h"
#include "Token.h"
#include "Interval.h"
#include "RuleTransition.h"
#include "PrecedencePredicateTransition.h"
#include "PredicateTransition.h"
#include "RangeTransition.h"
#include "AtomTransition.h"
#include "ActionTransition.h"
#include "Utils.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::Token;
                    using org::antlr::v4::runtime::misc::IntegerList;
                    using org::antlr::v4::runtime::misc::Interval;
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::Utils;

                    ATNSerializer::ATNSerializer(ATN *atn) {
                        assert(atn->grammarType != nullptr);
                        this->atn = atn;
                    }

                    ATNSerializer::ATNSerializer(ATN *atn, std::vector<std::wstring> &tokenNames) {
                        assert(atn->grammarType != nullptr);
                        this->atn = atn;
                        this->tokenNames = tokenNames;
                    }

                    org::antlr::v4::runtime::misc::IntegerList *ATNSerializer::serialize() {
                        IntegerList *data = new IntegerList();
                        data->add(ATNDeserializer::SERIALIZED_VERSION);
                        serializeUUID(data, ATNDeserializer::SERIALIZED_UUID);

                        // convert grammar type to ATN const to avoid dependence on ANTLRParser
                        data->add(atn->grammarType->ordinal());
                        data->add(atn->maxTokenType);
                        int nedges = 0;

                        Map<IntervalSet*, int> *setIndices = std::unordered_map<IntervalSet*, int>();
                        std::vector<IntervalSet*> sets = std::vector<IntervalSet*>();

                        // dump states, count edges and collect sets while doing so
                        IntegerList *nonGreedyStates = new IntegerList();
                        IntegerList *precedenceStates = new IntegerList();
                        data->add(atn->states->size());
                        for (ATNState *s : atn->states) {
                            if (s == nullptr) { // might be optimized away
                                data->add(ATNState::INVALID_TYPE);
                                continue;
                            }

                            int stateType = s.getStateType();
                            if (dynamic_cast<DecisionState*>(s) != nullptr && (static_cast<DecisionState*>(s))->nonGreedy) {
                                nonGreedyStates->add(s.stateNumber);
                            }

                            if (dynamic_cast<RuleStartState*>(s) != nullptr && (static_cast<RuleStartState*>(s))->isPrecedenceRule) {
                                precedenceStates->add(s.stateNumber);
                            }

                            data->add(stateType);

                            if (s.ruleIndex == -1) {
                                data->add(wchar_t::MAX_VALUE);
                            } else {
                                data->add(s.ruleIndex);
                            }

                            if (s.getStateType() == ATNState::LOOP_END) {
                                data->add((static_cast<LoopEndState*>(s))->loopBackState->stateNumber);
                            } else if (dynamic_cast<BlockStartState*>(s) != nullptr) {
                                data->add((static_cast<BlockStartState*>(s))->endState->stateNumber);
                            }

                            if (s.getStateType() != ATNState::RULE_STOP) {
                                // the deserializer can trivially derive these edges, so there's no need to serialize them
                                nedges += s.getNumberOfTransitions();
                            }

                            for (int i = 0; i < s.getNumberOfTransitions(); i++) {
                                Transition *t = s.transition(i);
                                int edgeType = Transition::serializationTypes->get(t->getClass());
                                if (edgeType == Transition::SET || edgeType == Transition::NOT_SET) {
                                    SetTransition *st = static_cast<SetTransition*>(t);
                                    if (!setIndices->containsKey(st->set)) {
                                        sets.push_back(st->set);
                                        setIndices->put(st->set, sets.size() - 1);
                                    }
                                }
                            }
                        }

                        // non-greedy states
                        data->add(nonGreedyStates->size());
                        for (int i = 0; i < nonGreedyStates->size(); i++) {
                            data->add(nonGreedyStates->get(i));
                        }

                        // precedence states
                        data->add(precedenceStates->size());
                        for (int i = 0; i < precedenceStates->size(); i++) {
                            data->add(precedenceStates->get(i));
                        }

                        int nrules = atn->ruleToStartState->length;
                        data->add(nrules);
                        for (int r = 0; r < nrules; r++) {
                            ATNState *ruleStartState = atn->ruleToStartState[r];
                            data->add(ruleStartState->stateNumber);
                            if (atn->grammarType == ATNType::LEXER) {
                                if (atn->ruleToTokenType[r] == Token::EOF) {
                                    data->add(wchar_t::MAX_VALUE);
                                } else {
                                    data->add(atn->ruleToTokenType[r]);
                                }

                                if (atn->ruleToActionIndex[r] == -1) {
                                    data->add(wchar_t::MAX_VALUE);
                                } else {
                                    data->add(atn->ruleToActionIndex[r]);
                                }
                            }
                        }

                        int nmodes = atn->modeToStartState->size();
                        data->add(nmodes);
                        if (nmodes > 0) {
                            for (ATNState *modeStartState : atn->modeToStartState) {
                                data->add(modeStartState.stateNumber);
                            }
                        }

                        int nsets = sets.size();
                        data->add(nsets);
                        for (auto set : sets) {
                            bool containsEof = set.contains(Token::EOF);
                            if (containsEof && set.getIntervals()->get(0)->b == Token::EOF) {
                                data->add(set.getIntervals()->size() - 1);
                            } else {
                                data->add(set.getIntervals()->size());
                            }

                            data->add(containsEof ? 1 : 0);
                            for (Interval *I : set.getIntervals()) {
                                if (I.a == Token::EOF) {
                                    if (I.b == Token::EOF) {
                                        continue;
                                    } else {
                                        data->add(0);
                                    }
                                } else {
                                    data->add(I.a);
                                }

                                data->add(I.b);
                            }
                        }

                        data->add(nedges);
                        for (ATNState *s : atn->states) {
                            if (s == nullptr) {
                                // might be optimized away
                                continue;
                            }

                            if (s.getStateType() == ATNState::RULE_STOP) {
                                continue;
                            }

                            for (int i = 0; i < s.getNumberOfTransitions(); i++) {
                                Transition *t = s.transition(i);

                                if (atn->states[t->target->stateNumber] == nullptr) {
                                    throw IllegalStateException(L"Cannot serialize a transition to a removed state.");
                                }

                                int src = s.stateNumber;
                                int trg = t->target->stateNumber;
                                int edgeType = Transition::serializationTypes->get(t->getClass());
                                int arg1 = 0;
                                int arg2 = 0;
                                int arg3 = 0;
                                switch (edgeType) {
                                    case Transition::RULE :
                                        trg = (static_cast<RuleTransition*>(t))->followState->stateNumber;
                                        arg1 = (static_cast<RuleTransition*>(t))->target->stateNumber;
                                        arg2 = (static_cast<RuleTransition*>(t))->ruleIndex;
                                        arg3 = (static_cast<RuleTransition*>(t))->precedence;
                                        break;
                                    case Transition::PRECEDENCE:
                                        PrecedencePredicateTransition *ppt = static_cast<PrecedencePredicateTransition*>(t);
                                        arg1 = ppt->precedence;
                                        break;
                                    case Transition::PREDICATE :
                                        PredicateTransition *pt = static_cast<PredicateTransition*>(t);
                                        arg1 = pt->ruleIndex;
                                        arg2 = pt->predIndex;
                                        arg3 = pt->isCtxDependent ? 1 : 0;
                                        break;
                                    case Transition::RANGE :
                                        arg1 = (static_cast<RangeTransition*>(t))->from;
                                        arg2 = (static_cast<RangeTransition*>(t))->to;
                                        if (arg1 == Token::EOF) {
                                            arg1 = 0;
                                            arg3 = 1;
                                        }

                                        break;
                                    case Transition::ATOM :
                                        arg1 = (static_cast<AtomTransition*>(t))->label_Renamed;
                                        if (arg1 == Token::EOF) {
                                            arg1 = 0;
                                            arg3 = 1;
                                        }

                                        break;
                                    case Transition::ACTION :
                                        ActionTransition *at = static_cast<ActionTransition*>(t);
                                        arg1 = at->ruleIndex;
                                        arg2 = at->actionIndex;
                                        if (arg2 == -1) {
                                            arg2 = 0xFFFF;
                                        }

                                        arg3 = at->isCtxDependent ? 1 : 0;
                                        break;
                                    case Transition::SET :
                                        arg1 = setIndices->get((static_cast<SetTransition*>(t))->set);
                                        break;
                                    case Transition::NOT_SET :
                                        arg1 = setIndices->get((static_cast<SetTransition*>(t))->set);
                                        break;
                                    case Transition::WILDCARD :
                                        break;
                                }

                                data->add(src);
                                data->add(trg);
                                data->add(edgeType);
                                data->add(arg1);
                                data->add(arg2);
                                data->add(arg3);
                            }
                        }
                        int ndecisions = atn->decisionToState->size();
                        data->add(ndecisions);
                        for (DecisionState *decStartState : atn->decisionToState) {
                            data->add(decStartState.stateNumber);
                        }

                        // don't adjust the first value since that's the version number
                        for (int i = 1; i < data->size(); i++) {
                            if (data->get(i) < wchar_t::MIN_VALUE || data->get(i) > wchar_t::MAX_VALUE) {
                                throw UnsupportedOperationException(L"Serialized ATN data element out of range.");
                            }

                            int value = (data->get(i) + 2) & 0xFFFF;
                            data->set(i, value);
                        }

                        return data;
                    }

                    std::wstring ATNSerializer::decode(wchar_t data[]) {
                        data = data->clone();
                        // don't adjust the first value since that's the version number
                        for (int i = 1; i < sizeof(data) / sizeof(data[0]); i++) {
                            data[i] = static_cast<wchar_t>(data[i] - 2);
                        }

                        StringBuilder *buf = new StringBuilder();
                        int p = 0;
                        int version = ATNDeserializer::toInt(data[p++]);
                        if (version != ATNDeserializer::SERIALIZED_VERSION) {
                            std::wstring reason = std::wstring::format(L"Could not deserialize ATN with version %d (expected %d).", version, ATNDeserializer::SERIALIZED_VERSION);
                            throw UnsupportedOperationException(InvalidClassException(ATN::typeid::getName(), reason));
                        }

                        UUID *uuid = ATNDeserializer::toUUID(data, p);
                        p += 8;
                        if (!uuid->equals(ATNDeserializer::SERIALIZED_UUID)) {
                            std::wstring reason = std::wstring::format(Locale::getDefault(), L"Could not deserialize ATN with UUID %s (expected %s).", uuid, ATNDeserializer::SERIALIZED_UUID);
                            throw UnsupportedOperationException(InvalidClassException(ATN::typeid::getName(), reason));
                        }

                        p++; // skip grammarType
                        int maxType = ATNDeserializer::toInt(data[p++]);
                        buf->append(L"max type ")->append(maxType)->append(L"\n");
                        int nstates = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < nstates; i++) {
                            int stype = ATNDeserializer::toInt(data[p++]);
                            if (stype == ATNState::INVALID_TYPE) { // ignore bad type of states
                                continue;
                            }
                            int ruleIndex = ATNDeserializer::toInt(data[p++]);
                            if (ruleIndex == wchar_t::MAX_VALUE) {
                                ruleIndex = -1;
                            }

                            std::wstring arg = L"";
                            if (stype == ATNState::LOOP_END) {
                                int loopBackStateNumber = ATNDeserializer::toInt(data[p++]);
                                arg = std::wstring(L" ") + loopBackStateNumber;
                            } else if (stype == ATNState::PLUS_BLOCK_START || stype == ATNState::STAR_BLOCK_START || stype == ATNState::BLOCK_START) {
                                int endStateNumber = ATNDeserializer::toInt(data[p++]);
                                arg = std::wstring(L" ") + endStateNumber;
                            }
                            buf->append(i)->append(L":")->append(ATNState::serializationNames[stype])->append(L" ")->append(ruleIndex)->append(arg)->append(L"\n");
                        }
                        int numNonGreedyStates = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < numNonGreedyStates; i++) {
                            int stateNumber = ATNDeserializer::toInt(data[p++]);
                        }
                        int numPrecedenceStates = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < numPrecedenceStates; i++) {
                            int stateNumber = ATNDeserializer::toInt(data[p++]);
                        }
                        int nrules = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < nrules; i++) {
                            int s = ATNDeserializer::toInt(data[p++]);
                            if (atn->grammarType == ATNType::LEXER) {
                                int arg1 = ATNDeserializer::toInt(data[p++]);
                                int arg2 = ATNDeserializer::toInt(data[p++]);
                                if (arg2 == wchar_t::MAX_VALUE) {
                                    arg2 = -1;
                                }
                                buf->append(L"rule ")->append(i)->append(L":")->append(s)->append(L" ")->append(arg1)->append(L",")->append(arg2)->append(L'\n');
                            } else {
                                buf->append(L"rule ")->append(i)->append(L":")->append(s)->append(L'\n');
                            }
                        }
                        int nmodes = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < nmodes; i++) {
                            int s = ATNDeserializer::toInt(data[p++]);
                            buf->append(L"mode ")->append(i)->append(L":")->append(s)->append(L'\n');
                        }
                        int nsets = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < nsets; i++) {
                            int nintervals = ATNDeserializer::toInt(data[p++]);
                            buf->append(i)->append(L":");
                            bool containsEof = data[p++] != 0;
                            if (containsEof) {
                                buf->append(getTokenName(Token::EOF));
                            }

                            for (int j = 0; j < nintervals; j++) {
                                if (containsEof || j > 0) {
                                    buf->append(L", ");
                                }

                                buf->append(getTokenName(ATNDeserializer::toInt(data[p])))->append(L"..")->append(getTokenName(ATNDeserializer::toInt(data[p + 1])));
                                p += 2;
                            }
                            buf->append(L"\n");
                        }
                        int nedges = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < nedges; i++) {
                            int src = ATNDeserializer::toInt(data[p]);
                            int trg = ATNDeserializer::toInt(data[p + 1]);
                            int ttype = ATNDeserializer::toInt(data[p + 2]);
                            int arg1 = ATNDeserializer::toInt(data[p + 3]);
                            int arg2 = ATNDeserializer::toInt(data[p + 4]);
                            int arg3 = ATNDeserializer::toInt(data[p + 5]);
                            buf->append(src)->append(L"->")->append(trg)->append(L" ")->append(Transition::serializationNames[ttype])->append(L" ")->append(arg1)->append(L",")->append(arg2)->append(L",")->append(arg3)->append(L"\n");
                            p += 6;
                        }
                        int ndecisions = ATNDeserializer::toInt(data[p++]);
                        for (int i = 0; i < ndecisions; i++) {
                            int s = ATNDeserializer::toInt(data[p++]);
                            buf->append(i)->append(L":")->append(s)->append(L"\n");
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    std::wstring ATNSerializer::getTokenName(int t) {
                        if (t == -1) {
                            return L"EOF";
                        }

                        if (atn->grammarType == ATNType::LEXER && t >= wchar_t::MIN_VALUE && t <= wchar_t::MAX_VALUE) {
                            switch (t) {
                            case L'\n':
                                return L"'\\n'";
                            case L'\r':
                                return L"'\\r'";
                            case L'\t':
                                return L"'\\t'";
                            case L'\b':
                                return L"'\\b'";
                            case L'\f':
                                return L"'\\f'";
                            case L'\\':
                                return L"'\\\\'";
                            case L'\'':
                                return L"'\\''";
                            default:
                                if (wchar_t::UnicodeBlock::of(static_cast<wchar_t>(t)) == wchar_t::UnicodeBlock::BASIC_LATIN && !iscntrl(static_cast<wchar_t>(t))) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    return L'\'' + wchar_t::toString(static_cast<wchar_t>(t)) + L'\'';
                                }
                                // turn on the bit above max "\uFFFF" value so that we pad with zeros
                                // then only take last 4 digits
                                std::wstring hex = int::toHexString(t | 0x10000)->toUpperCase()->substr(1, 4);
                                std::wstring unicodeStr = std::wstring(L"'\\u") + hex + std::wstring(L"'");
                                return unicodeStr;
                            }
                        }

                        if (tokenNames.size() > 0 && t >= 0 && t < tokenNames.size()) {
                            return tokenNames[t];
                        }

                        return StringConverterHelper::toString(t);
                    }

                    std::wstring ATNSerializer::getSerializedAsString(ATN *atn) {
                        return std::wstring(getSerializedAsChars(atn));
                    }

                    org::antlr::v4::runtime::misc::IntegerList *ATNSerializer::getSerialized(ATN *atn) {
                        return (new ATNSerializer(atn))->serialize();
                    }

                    wchar_t *ATNSerializer::getSerializedAsChars(ATN *atn) {
                        return Utils::toCharArray(getSerialized(atn));
                    }

                    std::wstring ATNSerializer::getDecoded(ATN *atn, std::vector<std::wstring> &tokenNames) {
                        IntegerList *serialized = getSerialized(atn);
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: char[] data = org.antlr.v4.runtime.misc.Utils.toCharArray(serialized);
                        wchar_t *data = Utils::toCharArray(serialized);
                        return (new ATNSerializer(atn, tokenNames))->decode(data);
                    }

                    void ATNSerializer::serializeUUID(IntegerList *data, UUID *uuid) {
                        serializeLong(data, uuid->getLeastSignificantBits());
                        serializeLong(data, uuid->getMostSignificantBits());
                    }

                    void ATNSerializer::serializeLong(IntegerList *data, long long value) {
                        serializeInt(data, static_cast<int>(value));
                        serializeInt(data, static_cast<int>(value >> 32));
                    }

                    void ATNSerializer::serializeInt(IntegerList *data, int value) {
                        data->add(static_cast<wchar_t>(value));
                        data->add(static_cast<wchar_t>(value >> 16));
                    }
                }
            }
        }
    }
}

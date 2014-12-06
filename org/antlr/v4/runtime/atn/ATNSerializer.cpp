#include <unordered_map>
#include <wchar.h>

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
#include "Exceptions.h"
#include "TokensStartState.h"

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
namespace atn {

ATNSerializer::ATNSerializer(ATN *atn) { this->atn = atn; }

ATNSerializer::ATNSerializer(ATN *atn, std::vector<std::wstring> &tokenNames) {
  this->atn = atn;
  this->tokenNames = tokenNames;
}

std::vector<int>* ATNSerializer::serialize() {
  std::vector<int>* data = new std::vector<int>();
  data->push_back(ATNDeserializer::SERIALIZED_VERSION);
  serializeUUID(data, ATNDeserializer::SERIALIZED_UUID);

  // convert grammar type to ATN const to avoid dependence on ANTLRParser
  data->push_back(static_cast<int>(atn->grammarType));
  data->push_back(atn->maxTokenType);
  int nedges = 0;

  std::unordered_map<misc::IntervalSet *, int> *setIndices =
      new std::unordered_map<misc::IntervalSet *, int>();
  std::vector<misc::IntervalSet *> sets = std::vector<misc::IntervalSet *>();

  // dump states, count edges and collect sets while doing so
  std::vector<int> nonGreedyStates;
  std::vector<int> precedenceStates;
  data->push_back(atn->states.size());
  for (ATNState *s : atn->states) {
    if (s == nullptr) {  // might be optimized away
      data->push_back(ATNState::INVALID_TYPE);
      continue;
    }

    int stateType = s->getStateType();
    if (dynamic_cast<DecisionState *>(s) != nullptr &&
        (static_cast<DecisionState *>(s))->nonGreedy) {
      nonGreedyStates.push_back(s->stateNumber);
    }

    if (dynamic_cast<RuleStartState *>(s) != nullptr &&
        (static_cast<RuleStartState *>(s))->isPrecedenceRule) {
      precedenceStates.push_back(s->stateNumber);
    }

    data->push_back(stateType);

    if (s->ruleIndex == -1) {
      data->push_back(WCHAR_MAX);
    } else {
      data->push_back(s->ruleIndex);
    }

    if (s->getStateType() == ATNState::LOOP_END) {
      data->push_back((static_cast<LoopEndState *>(s))->loopBackState->stateNumber);
    } else if (dynamic_cast<BlockStartState *>(s) != nullptr) {
      data->push_back((static_cast<BlockStartState *>(s))->endState->stateNumber);
    }

    if (s->getStateType() != ATNState::RULE_STOP) {
      // the deserializer can trivially derive these edges, so there's no need
      // to serialize them
      nedges += s->getNumberOfTransitions();
    }

    for (int i = 0; i < s->getNumberOfTransitions(); i++) {
      Transition *t = s->transition(i);
      int edgeType = Transition::serializationTypes->get(t->getClass());
      if (edgeType == Transition::SET || edgeType == Transition::NOT_SET) {
        SetTransition *st = static_cast<SetTransition *>(t);
        if (!setIndices->containsKey(st->set)) {
          sets.push_back(st->set);
          setIndices->put(st->set, sets.size() - 1);
        }
      }
    }
  }

  // non-greedy states
  data->push_back(nonGreedyStates.size());
  for (int i = 0; i < nonGreedyStates.size(); i++) {
    data->push_back(nonGreedyStates.at(i));
  }

  // precedence states
  data->push_back(precedenceStates.size());
  for (int i = 0; i < precedenceStates.size(); i++) {
    data->push_back(precedenceStates.at(i));
  }

  int nrules = atn->ruleToStartState.size();
  data->push_back(nrules);
  for (int r = 0; r < nrules; r++) {
    ATNState *ruleStartState = atn->ruleToStartState[r];
    data->push_back(ruleStartState->stateNumber);
    if (atn->grammarType == ATNType::LEXER) {
      if (atn->ruleToTokenType[r] == Token::_EOF) {
        data->push_back(WCHAR_MAX);
      } else {
        data->push_back(atn->ruleToTokenType[r]);
      }

      if (atn->ruleToActionIndex[r] == -1) {
        data->push_back(WCHAR_MAX);
      } else {
        data->push_back(atn->ruleToActionIndex[r]);
      }
    }
  }

  int nmodes = atn->modeToStartState->size();
  data->push_back(nmodes);
  if (nmodes > 0) {
    for (const auto& modeStartState : *atn->modeToStartState) {
      data->push_back(modeStartState->stateNumber);
    }
  }

  int nsets = sets.size();
  data->push_back(nsets);
  for (auto set : sets) {
    bool containsEof = set->contains(Token::_EOF);
    if (containsEof && set->getIntervals().at(0)->b == Token::_EOF) {
      data->push_back(set->getIntervals().size() - 1);
    } else {
      data->push_back(set->getIntervals().size());
    }

    data->push_back(containsEof ? 1 : 0);
    for (misc::Interval *I : set->getIntervals()) {
      if (I->a == Token::_EOF) {
        if (I->b == Token::_EOF) {
          continue;
        } else {
          data->push_back(0);
        }
      } else {
        data->push_back(I->a);
      }

      data->push_back(I->b);
    }
  }

  data->push_back(nedges);
  for (ATNState *s : atn->states) {
    if (s == nullptr) {
      // might be optimized away
      continue;
    }

    if (s->getStateType() == ATNState::RULE_STOP) {
      continue;
    }

    for (int i = 0; i < s->getNumberOfTransitions(); i++) {
      Transition *t = s->transition(i);

      if (atn->states[t->target->stateNumber] == nullptr) {
        throw IllegalStateException(
            L"Cannot serialize a transition to a removed state.");
      }

      int src = s->stateNumber;
      int trg = t->target->stateNumber;
      int edgeType = Transition::serializationTypes->get(t->getClass());
      int arg1 = 0;
      int arg2 = 0;
      int arg3 = 0;
      switch (edgeType) {
        case Transition::RULE:
          trg = (static_cast<RuleTransition *>(t))->followState->stateNumber;
          arg1 = (static_cast<RuleTransition *>(t))->target->stateNumber;
          arg2 = (static_cast<RuleTransition *>(t))->ruleIndex;
          arg3 = (static_cast<RuleTransition *>(t))->precedence;
          break;
        case Transition::PRECEDENCE:
          PrecedencePredicateTransition *ppt =
              static_cast<PrecedencePredicateTransition *>(t);
          arg1 = ppt->precedence;
          break;
        case Transition::PREDICATE:
          PredicateTransition *pt = static_cast<PredicateTransition *>(t);
          arg1 = pt->ruleIndex;
          arg2 = pt->predIndex;
          arg3 = pt->isCtxDependent ? 1 : 0;
          break;
        case Transition::RANGE:
          arg1 = (static_cast<RangeTransition *>(t))->from;
          arg2 = (static_cast<RangeTransition *>(t))->to;
          if (arg1 == Token) {
            arg1 = 0;
            arg3 = 1;
          }

          break;
        case Transition::ATOM:
          arg1 = (static_cast<AtomTransition *>(t))->label_Renamed;
          if (arg1 == Token::_EOF) {
            arg1 = 0;
            arg3 = 1;
          }

          break;
        case Transition::ACTION:
          ActionTransition *at = static_cast<ActionTransition *>(t);
          arg1 = at->ruleIndex;
          arg2 = at->actionIndex;
          if (arg2 == -1) {
            arg2 = 0xFFFF;
          }

          arg3 = at->isCtxDependent ? 1 : 0;
          break;
        case Transition::SET:
          arg1 = setIndices->get((static_cast<SetTransition *>(t))->set);
          break;
        case Transition::NOT_SET:
          arg1 = setIndices->get((static_cast<SetTransition *>(t))->set);
          break;
        case Transition::WILDCARD:
          break;
      }

      data->push_back(src);
      data->push_back(trg);
      data->push_back(edgeType);
      data->push_back(arg1);
      data->push_back(arg2);
      data->push_back(arg3);
    }
  }
  int ndecisions = atn->decisionToState.size();
  data->add(ndecisions);
  for (DecisionState *decStartState : atn->decisionToState) {
    data->add(decStartState->stateNumber);
  }

  // don't adjust the first value since that's the version number
  for (int i = 1; i < data->size(); i++) {
    if (data->at(i) < WCHAR_MIN || data->at(i) > WCHAR_MAX) {
      throw UnsupportedOperationException(
          L"Serialized ATN data element out of range.");
    }

    int value = (data->at(i) + 2) & 0xFFFF;
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
    std::wstring reason = std::wstring::format(
        L"Could not deserialize ATN with version %d (expected %d).", version,
        ATNDeserializer::SERIALIZED_VERSION);
    throw UnsupportedOperationException(
        InvalidClassException(ATN::typeid::getName(), reason));
  }

  UUID *uuid = ATNDeserializer::toUUID(data, p);
  p += 8;
  if (!uuid->equals(ATNDeserializer::SERIALIZED_UUID)) {
    std::wstring reason = std::wstring::format(
        Locale::getDefault(),
        L"Could not deserialize ATN with UUID %s (expected %s).", uuid,
        ATNDeserializer::SERIALIZED_UUID);
    throw UnsupportedOperationException(
        InvalidClassException(ATN::typeid::getName(), reason));
  }

  p++;  // skip grammarType
  int maxType = ATNDeserializer::toInt(data[p++]);
  buf->append(L"max type ")->append(maxType)->append(L"\n");
  int nstates = ATNDeserializer::toInt(data[p++]);
  for (int i = 0; i < nstates; i++) {
    int stype = ATNDeserializer::toInt(data[p++]);
    if (stype == ATNState::INVALID_TYPE) {  // ignore bad type of states
      continue;
    }
    int ruleIndex = ATNDeserializer::toInt(data[p++]);
    if (ruleIndex == WCHAR_MAX) {
      ruleIndex = -1;
    }

    std::wstring arg = L"";
    if (stype == ATNState::LOOP_END) {
      int loopBackStateNumber = ATNDeserializer::toInt(data[p++]);
      arg = std::wstring(L" ") + loopBackStateNumber;
    } else if (stype == ATNState::PLUS_BLOCK_START ||
               stype == ATNState::STAR_BLOCK_START ||
               stype == ATNState::BLOCK_START) {
      int endStateNumber = ATNDeserializer::toInt(data[p++]);
      arg = std::wstring(L" ") + endStateNumber;
    }
    buf->append(i)
        ->append(L":")
        ->append(ATNState::serializationNames[stype])
        ->append(L" ")
        ->append(ruleIndex)
        ->append(arg)
        ->append(L"\n");
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
      if (arg2 == WCHAR_MAX) {
        arg2 = -1;
      }
      buf->append(L"rule ")
          ->append(i)
          ->append(L":")
          ->append(s)
          ->append(L" ")
          ->append(arg1)
          ->append(L",")
          ->append(arg2)
          ->append(L'\n');
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
      buf->append(getTokenName(Token::_EOF));
    }

    for (int j = 0; j < nintervals; j++) {
      if (containsEof || j > 0) {
        buf->append(L", ");
      }

      buf->append(getTokenName(ATNDeserializer::toInt(data[p])))
          ->append(L"..")
          ->append(getTokenName(ATNDeserializer::toInt(data[p + 1])));
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
    buf->append(src)
        ->append(L"->")
        ->append(trg)
        ->append(L" ")
        ->append(Transition::serializationNames[ttype])
        ->append(L" ")
        ->append(arg1)
        ->append(L",")
        ->append(arg2)
        ->append(L",")
        ->append(arg3)
        ->append(L"\n");
    p += 6;
  }
  int ndecisions = ATNDeserializer::toInt(data[p++]);
  for (int i = 0; i < ndecisions; i++) {
    int s = ATNDeserializer::toInt(data[p++]);
    buf->append(i)->append(L":")->append(s)->append(L"\n");
  }
  // JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to
  // 'toString':
  return buf->toString();
}

std::wstring ATNSerializer::getTokenName(int t) {
  if (t == -1) {
    return L"EOF";
  }

  if (atn->grammarType == ATNType::LEXER && t >= WCHAR_MIN &&
      t <= WCHAR_MAX) {
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
        if (UnicodeBlock::of(static_cast<wchar_t>(t)) ==
                UnicodeBlock::BASIC_LATIN &&
            !iscntrl(static_cast<wchar_t>(t))) {
          // JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent
          // to 'toString':
          return L'\'' + toString(static_cast<wchar_t>(t)) + L'\'';
        }
        // turn on the bit above max "\uFFFF" value so that we pad with zeros
        // then only take last 4 digits
        std::wstring hex =
            int ::toHexString(t | 0x10000)->toUpperCase()->substr(1, 4);
        std::wstring unicodeStr =
            std::wstring(L"'\\u") + hex + std::wstring(L"'");
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

std::vector<int> *ATNSerializer::getSerialized(ATN *atn) {
  return (new ATNSerializer(atn))->serialize();
}

wchar_t *ATNSerializer::getSerializedAsChars(ATN *atn) {
  return Utils::toCharArray(getSerialized(atn));
}

std::wstring ATNSerializer::getDecoded(ATN *atn,
                                       std::vector<std::wstring> &tokenNames) {
  std::vector<int> *serialized = getSerialized(atn);
  // JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this
  // declaration, Java to C++ Converter has converted this array to a pointer.
  // You will need to call 'delete[]' where appropriate:
  // ORIGINAL LINE: char[] data =
  // org.antlr.v4.runtime.misc.Utils.toCharArray(serialized);
  wchar_t *data = Utils::toCharArray(serialized);
  return (new ATNSerializer(atn, tokenNames))->decode(data);
}

void ATNSerializer::serializeUUID(std::vector<int> *data, UUID *uuid) {
  serializeLong(data, uuid->getLeastSignificantBits());
  serializeLong(data, uuid->getMostSignificantBits());
}

void ATNSerializer::serializeLong(std::vector<int> *data, long long value) {
  serializeInt(data, static_cast<int>(value));
  serializeInt(data, static_cast<int>(value >> 32));
}

void ATNSerializer::serializeInt(std::vector<int> *data, int value) {
  data->add(static_cast<wchar_t>(value));
  data->add(static_cast<wchar_t>(value >> 16));
}

}  // namespace atn
}  // namespace runtime
}  // namespace v4
}  // namespace antlr
}  // namespace org

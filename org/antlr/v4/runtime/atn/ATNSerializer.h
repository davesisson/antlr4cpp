﻿#pragma once

#include <vector>
#include <cctype>
#include <string>
#include <unordered_map>

#include "ATN.h"
#include "stringconverter.h"
#include "UUID.h"

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

					class ATNSerializer {
					public:
						ATN *atn;

					private:
						std::vector<std::wstring> tokenNames;

					public:
						ATNSerializer(ATN *atn);

						ATNSerializer(ATN *atn, const std::vector<std::wstring> &tokenNames);

						/// <summary>
						/// Serialize state descriptors, edge descriptors, and decision->state map
						///  into list of ints:
						///
						/// 		grammar-type, (ANTLRParser.LEXER, ...)
						///  	max token type,
						///  	num states,
						///  	state-0-type ruleIndex, state-1-type ruleIndex, ... state-i-type
						///  ruleIndex optional-arg ...
						///  	num rules,
						///  	rule-1-start-state rule-1-args, rule-2-start-state  rule-2-args, ...
						///  	(args are token type,actionIndex in lexer else 0,0)
						///      num modes,
						///      mode-0-start-state, mode-1-start-state, ... (parser has 0 modes)
						///      num sets
						///      set-0-interval-count intervals, set-1-interval-count intervals, ...
						///  	num total edges,
						///      src, trg, edge-type, edge arg1, optional edge arg2 (present always),
						///      ...
						///      num decisions,
						///      decision-0-start-state, decision-1-start-state, ...
						///
						///  Convenient to pack into unsigned shorts to make as Java string.
						/// </summary>
						virtual std::vector<size_t> *serialize();

						virtual std::wstring decode(const std::wstring& data);

						virtual std::wstring getTokenName(int t);

						/// <summary>
						/// Used by Java target to encode short/int array as chars in string.
						/// </summary>
						static std::wstring getSerializedAsString(ATN *atn);

						static std::vector<size_t> *getSerialized(ATN *atn);

						static wchar_t *getSerializedAsChars(ATN *atn);

						static std::wstring getDecoded(ATN *atn,
							std::vector<std::wstring> &tokenNames);

					private:
						void serializeUUID(std::vector<size_t> *data, antlrcpp::UUID *uuid);

						void serializeLong(std::vector<size_t> *data, long long value);

						void serializeInt(std::vector<size_t> *data, int value);
					};

				}  // namespace org
			}  // namespace antlr
		}  // namespace v4
	}  // namespace runtime
}  // namespace atn

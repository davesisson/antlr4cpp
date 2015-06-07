#include "Utils.h"
#include <stdarg.h>  // for va_start, etc
#include <memory>    // for std::unique_ptr
#include <stdlib.h>
#include <vector>

/*
 * [The "BSD license"]
 *  Copyright (c) 2014 Dan McLaughlin
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


namespace Utils {
#ifdef TODO
    Add the body
#endif
    std::wstring join(std::vector<std::wstring> strings, const std::wstring &separator) {
        std::wstring str;
        bool firstItem = true;
        for (std::wstring s : strings) {
            if (!firstItem) {
                str.append(separator);
            } else {
                firstItem = false;
            }
            str.append(s);
        }
        return str;
    }
    
    std::map<std::wstring, int>* toMap(std::wstring *) {
#ifdef TODO
        Add the body
#endif
        return nullptr;
    }
    
    std::map<std::wstring, int>* toMap(const std::vector<std::wstring> &keys) {
        std::map<std::wstring, int>* m = new std::map<std::wstring, int>();
        for (int i = 0; i < keys.size(); ++i) {
            m->insert(std::pair<std::wstring, int>(keys[i], i));
        }
        return m;
    }
    
    std::wstring escapeWhitespace() {
#ifdef TODO
        
#endif
        return nullptr;
    }
    
    std::wstring stringFormat(const std::wstring fmt_str, ...)
    {
        // Not sure this is needed, just use swprintf.
        // TODO(dsisson): Remove this function in a future change.
        std::wstring blank;
        return blank;
    }

	wchar_t* toCharArray(const std::vector<int> *data){
		if (data == nullptr) return nullprt;
		wchar_t* cdata = new wchar_t(*data.size());

		for (int i = 0; i < *data.size(); i++){
			cdata[i] = (char)*data[i];
		}

		return cdata;
	}
}
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
    
    std::map<std::wstring, int>* toMap(std::wstring *) {
#ifdef TODO
        Add the body
#endif
        return nullptr;
    }
    
    std::wstring escapeWhitespace() {
#ifdef TODO
        
#endif
        return nullptr;
    }
    
    std::wstring stringFormat(const wchar_t* fmt, ...)
    {
        if (!fmt) {
            return L"";
        }
        
        std::vector<wchar_t> buff;
        size_t size = wcslen(fmt) * 2;
        buff.resize(size);
        va_list ap;
        va_start(ap, fmt);
        while (true) {
            int ret = _vsnwprintf_s(buff.data(), size, _TRUNCATE, fmt, ap);
            if (ret != -1)
                break;
            else {
                size *= 2;
                buff.resize(size);
            }
        }
        va_end(ap);
        return std::wstring(buff.data());
    }
        

}
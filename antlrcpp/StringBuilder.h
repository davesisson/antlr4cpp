#pragma once

#include "Declarations.h"

/*
 * [The "BSD license"]
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

namespace antlrcpp {
    
    class StringBuilder {
    private:
        std::wstring main;
        std::wstring scratch;
        
        const std::wstring::size_type ScratchSize = 1024;  // or some other arbitrary number
        
    public:
        StringBuilder & append(const std::wstring & str);
        
        StringBuilder & append(const org::antlr::v4::runtime::atn::ATNState* const state);
        
        StringBuilder & append(const org::antlr::v4::runtime::atn::SemanticContext* const state);
        
        StringBuilder & append(const wchar_t & str);
        
        const std::wstring & str();
        
        const std::wstring & toString();
        
        std::wstring::size_type length();
        
        // Utility function to provide "format" capability
        /* Problems with this
         static std::wstring format(const std::string fmt_str, ...) {
         int final_n, n = (int)fmt_str.size() * 2; // reserve 2 times as much as the length of the fmt_str
         std::string str;
         std::unique_ptr<char[]> formatted;
         va_list ap;
         while(1) {
         formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
         strcpy(&formatted[0], fmt_str.c_str());
         va_start(ap, fmt_str);
         final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
         va_end(ap);
         if (final_n < 0 || final_n >= n)
         n += abs(final_n - n + 1);
         else
         break;
         }
         return std::wstring((char*)formatted.get());
         }
         
         static std::wstring format(const std::wstring fmt_str, ...) {
         }
         */
        static std::wstring stringToWstring( std::string str );
        
        static std::string WstringTostring( std::wstring wStr );
        
        static std::string string_replace(std::string src,
                                          std::string const& target,
                                          std::string const& repl);
    };
    
}


//
//  StringBuilder.h
//  antlrcpp
//
//  Created by Dan McLaughlin on 12/29/13.
//  Copyright (c) 2013 Dan McLaughlin. All rights reserved.
//

#ifndef __antlrcpp__StringBuilder__
#define __antlrcpp__StringBuilder__

#include <iostream>

class StringBuilder {
private:
    std::wstring main;
    std::wstring scratch;
    
    const std::wstring::size_type ScratchSize = 1024;  // or some other arbitrary number
    
public:
    StringBuilder & append(const std::wstring & str) {
        scratch.append(str);
        if (scratch.size() > ScratchSize) {
            main.append(scratch);
            scratch.resize(0);
        }
        return *this;
    }
    
    const std::wstring & str() {
        if (scratch.size() > 0) {
            main.append(scratch);
            scratch.resize(0);
        }
        return main;
    }
    const std::wstring & toString() {
        return str();
    }
};

#endif /* defined(__antlrcpp__StringBuilder__) */

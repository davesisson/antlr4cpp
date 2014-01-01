//
//  Exceptions.h
//  antlrcpp
//
//  Created by Dan McLaughlin on 12/31/13.
//  Copyright (c) 2013 Dan McLaughlin. All rights reserved.
//


#include <iostream>
#include <exception>
#include <string>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                
                class IllegalStateException : public std::exception {
                public:
                    IllegalStateException(const std::wstring msg);
                    IllegalStateException();
                };
                
                
                class IllegalArgumentException : public std::exception {
                public:
                    IllegalArgumentException(const std::wstring msg);
                    IllegalArgumentException();
                };
                
                class NoSuchElementException : public std::exception {
                public:
                    NoSuchElementException(const std::wstring msg);
                    NoSuchElementException();
                };
                
                
            }
        }
    }
}
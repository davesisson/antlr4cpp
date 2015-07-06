//
//  Exceptions.h
//  antlrcpp
//
//  Created by Dan McLaughlin on 12/31/13.
//  Copyright (c) 2013 Dan McLaughlin. All rights reserved.
//

#pragma once

#include "Declarations.h"

#include <iostream>
#include <exception>
#include <string>


namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                class ANTLRException : public std::exception {
                private:
                    std::wstring errormsg;
                    
                    public:
                    ANTLRException() {}
                    ANTLRException(const std::wstring msg) {
                        this->errormsg = msg;
                    }
                    std::wstring getMessage() {
                        return errormsg;
                    }

                };
                
                class IllegalClassException : public ANTLRException {
                public:
                    IllegalClassException(const std::wstring msg) : ANTLRException(msg) {};
                    IllegalClassException() {};
                };

                class IllegalStateException : public ANTLRException {
                public:
                    IllegalStateException(const std::wstring msg) : ANTLRException(msg) {};
                    IllegalStateException(){};
                };
                
                class IllegalArgumentException : public ANTLRException {
                public:
                    IllegalArgumentException(const std::wstring msg)  : ANTLRException(msg) {};
                    IllegalArgumentException(const std::wstring msg, std::exception e);
                    IllegalArgumentException(){};
                };
                
                class NoSuchElementException : public ANTLRException {
                public:
                    NoSuchElementException(const std::wstring msg)  : ANTLRException(msg) {};
                    NoSuchElementException(){};
                };
                
                class NullPointerException : public ANTLRException {
                public:
                    NullPointerException(const std::wstring msg) : ANTLRException(msg) {};
                    NullPointerException(){};
                };
                class IndexOutOfBoundsException : public ANTLRException {
                public:
                    IndexOutOfBoundsException(const std::wstring msg) : ANTLRException(msg) {};
                    IndexOutOfBoundsException(){};
                };
                class UnsupportedOperationException : public ANTLRException {
                public:
                    UnsupportedOperationException(const std::wstring msg) : ANTLRException(msg) {};
                    UnsupportedOperationException(){};
                };
                class IOException : public ANTLRException {
                public:
                    IOException(const std::wstring msg)  : ANTLRException(msg) {};
                    IOException(){};
                };
                class TODOException : public ANTLRException {
                public:
                    TODOException(const std::wstring msg)  : ANTLRException(msg) {};
                    TODOException();
                };
                class ASSERTException : public ANTLRException {
                public:
                    ASSERTException(const std::wstring location,
                                    const std::wstring condition)
                                    : ANTLRException(location + L" condition= " + condition) {};
                    ASSERTException();
                };

                
            }
        }
    }
}
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
                    public:
                    std::wstring getMessage();

                };
                
                class IllegalClassException : public ANTLRException {
                public:
                    IllegalClassException(const std::wstring msg);
                    IllegalClassException();
                };

                class IllegalStateException : public ANTLRException {
                public:
                    IllegalStateException(const std::wstring msg);
                    IllegalStateException();
                };
                
                class IllegalArgumentException : public ANTLRException {
                public:
                    IllegalArgumentException(const std::wstring msg);
                    IllegalArgumentException(const std::wstring msg, std::exception e);
                    IllegalArgumentException();
                };
                
                class NoSuchElementException : public ANTLRException {
                public:
                    NoSuchElementException(const std::wstring msg);
                    NoSuchElementException();
                };
                
                class NullPointerException : public std::exception {
                public:
                    NullPointerException(const std::wstring msg);
                    NullPointerException();
                };
                class IndexOutOfBoundsException : public ANTLRException {
                public:
                    IndexOutOfBoundsException(const std::wstring msg);
                    IndexOutOfBoundsException();
                };
                class UnsupportedOperationException : public ANTLRException {
                public:
                    UnsupportedOperationException(const std::wstring msg);
                    UnsupportedOperationException();
                };
                class IOException : public ANTLRException {
                public:
                    IOException(const std::wstring msg);
                    IOException();
                };
                
            }
        }
    }
}
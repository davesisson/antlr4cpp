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
#include "Declarations.h"
#include "RecognitionException.h"

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
                    IllegalArgumentException(const std::wstring msg, std::exception e);
                    IllegalArgumentException();
                };
                
                class NoSuchElementException : public std::exception {
                public:
                    NoSuchElementException(const std::wstring msg);
                    NoSuchElementException();
                };
                
                class NullPointerException : public std::exception {
                public:
                    NullPointerException(const std::wstring msg);
                    NullPointerException();
                };
                class IndexOutOfBoundsException : public std::exception {
                public:
                    IndexOutOfBoundsException(const std::wstring msg);
                    IndexOutOfBoundsException();
                };
                class UnsupportedOperationException : public std::exception {
                public:
                    UnsupportedOperationException(const std::wstring msg);
                    UnsupportedOperationException();
                };
                class IOException : public std::exception {
                public:
                    IOException(const std::wstring msg);
                    IOException();
                };

                class FailedPredicateException :public std::exception {
                public:
                    FailedPredicateException(const std::wstring msg);
                    FailedPredicateException(Parser*);
                    FailedPredicateException(Parser*, const std::wstring msg);
                    FailedPredicateException();
                };
                

                // Recognition exceptions
                
                class ParseCancellationException : public RecognitionException {
                public:
                    ParseCancellationException(const std::wstring msg);
                    ParseCancellationException(RecognitionException*);
                    ParseCancellationException();
                };
    
                class InputMismatchException : public RecognitionException {
                public:
                    InputMismatchException(const std::wstring msg);
                    InputMismatchException(const Parser * parser);
                    InputMismatchException(const RecognitionException*);
                    InputMismatchException();
                };

                class LexerNoViableAltException : public RecognitionException {
                public:
                    LexerNoViableAltException(const std::wstring msg);
                    LexerNoViableAltException();
                };
                class EmptyStackException : public RecognitionException {
                public:
                    EmptyStackException(const std::wstring msg);
                    EmptyStackException();
                };
                
                
            }
        }
    }
}
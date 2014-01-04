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
                
                class NullPointerException : public std::exception {
                public:
                    NullPointerException(const std::wstring msg);
                    NullPointerException();
                };
                
                class RecognitionException :  public std::exception {
                public:
                    RecognitionException(const std::wstring msg);
                    RecognitionException();
                };

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
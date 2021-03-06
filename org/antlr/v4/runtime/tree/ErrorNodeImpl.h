﻿#pragma once

#include "ErrorNode.h"
#include "TerminalNodeImpl.h"
#include "Declarations.h"

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
                namespace tree {


                    /// <summary>
                    /// Represents a token that was consumed during resynchronization
                    ///  rather than during a valid match operation. For example,
                    ///  we will create this kind of a node during single token insertion
                    ///  and deletion as well as during "consume until error recovery set"
                    ///  upon no viable alternative exceptions.
                    /// </summary>
                    class ErrorNodeImpl : public virtual TerminalNodeImpl, public virtual ErrorNode {
                    public:
                        ErrorNodeImpl(Token *token);

                        template<typename T, typename T1>
                        T accept(ParseTreeVisitor<T1> *visitor);
                        
                        // From ErrorNode
                        
                        // TerminalNodeImpl->TerminalNode->ParseTree->SyntaxTree->Tree
                        //        ErrorNode->TerminalNode ...

                        
                        // From TerminalnodeImpl
                        
                        // From Terminal Node
                    public:
                        Token *getSymbol();
                        
                        // From Parse Tree
                    public:
                        ParseTree *getParent() override;
                        ParseTree *getChild(int i) override;
                        std::wstring getText();
                        std::wstring toStringTree(Parser *parser);
                        
                        // From SyntaxTree
                    public:
                        misc::Interval *getSourceInterval() override;
                        
                        // From Tree
                    public:
//                        Tree *getParent() override;
                        void *getPayload() override;
//                        Tree *getChild(int i) override;
                        
                        int getChildCount() override;
                        std::wstring toStringTree() override;
                    };

                }
            }
        }
    }
}

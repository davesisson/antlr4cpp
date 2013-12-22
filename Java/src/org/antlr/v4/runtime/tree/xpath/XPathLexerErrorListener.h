#pragma once

#include "Java/src/org/antlr/v4/runtime/BaseErrorListener.h"
#include "Java/src/org/antlr/v4/runtime/RecognitionException.h"
#include "Java/src/org/antlr/v4/runtime/Recognizer.h"
#include <string>

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace tree {
                    namespace xpath {

                        using org::antlr::v4::runtime::BaseErrorListener;
                        using org::antlr::v4::runtime::RecognitionException;
                        using org::antlr::v4::runtime::Recognizer;

                        class XPathLexerErrorListener : public BaseErrorListener {
                        public:
                            template<typename T1, typename T1>
                            virtual void syntaxError(Recognizer<T1> *recognizer, void *offendingSymbol, int line, int charPositionInLine, const std::wstring &msg, RecognitionException *e) override;
                        };

                    }
                }
            }
        }
    }
}

#include "CommonTokenFactory.h"
#include "Java/src/org/antlr/v4/runtime/misc/Interval.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                using org::antlr::v4::runtime::misc::Interval;
                using org::antlr::v4::runtime::misc::Pair;
TokenFactory<CommonToken*> *const CommonTokenFactory::DEFAULT = new CommonTokenFactory();

                CommonTokenFactory::CommonTokenFactory(bool copyText) : copyText(copyText) {
                }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                CommonTokenFactory::CommonTokenFactory() {
                }

                org::antlr::v4::runtime::CommonToken *CommonTokenFactory::create(Pair<TokenSource*, CharStream*> *source, int type, const std::wstring &text, int channel, int start, int stop, int line, int charPositionInLine) {
                    CommonToken *t = new CommonToken(source, type, channel, start, stop);
                    t->setLine(line);
                    t->setCharPositionInLine(charPositionInLine);
                    if (text != L"") {
                        t->setText(text);
                    } else if (copyText && source->b != nullptr) {
                        t->setText(source->b->getText(Interval::of(start,stop)));
                    }

                    return t;
                }

                org::antlr::v4::runtime::CommonToken *CommonTokenFactory::create(int type, const std::wstring &text) {
                    return new CommonToken(type, text);
                }
            }
        }
    }
}

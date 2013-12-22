#include "ParseCancellationException.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {
                    using org::antlr::v4::runtime::BailErrorStrategy;
                    using org::antlr::v4::runtime::RecognitionException;

                    ParseCancellationException::ParseCancellationException() {
                    }

                    ParseCancellationException::ParseCancellationException(const std::wstring &message) : java::util::concurrent::CancellationException(message) {
                    }

                    ParseCancellationException::ParseCancellationException(std::exception cause) {
                        initCause(cause);
                    }

                    ParseCancellationException::ParseCancellationException(const std::wstring &message, std::exception cause) : java::util::concurrent::CancellationException(message) {
                        initCause(cause);
                    }
                }
            }
        }
    }
}

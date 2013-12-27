#include "ATNDeserializationOptions.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
//                    using org::antlr::v4::runtime::misc::NotNull;

//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not allowed in native C++:
                    ATNDeserializationOptions::ATNDeserializationOptions() {
//                        defaultOptions = new ATNDeserializationOptions();
                        defaultOptions->makeReadOnly();
                    }

//                    ATNDeserializationOptions::ATNDeserializationOptions() {
//                        InitializeInstanceFields();
//                        this->verifyATN = true;
//                        this->generateRuleBypassTransitions = false;
//                    }

                    ATNDeserializationOptions::ATNDeserializationOptions(ATNDeserializationOptions *options) {
                        InitializeInstanceFields();
                        this->verifyATN = options->verifyATN;
                        this->generateRuleBypassTransitions = options->generateRuleBypassTransitions;
                    }

                    org::antlr::v4::runtime::atn::ATNDeserializationOptions *ATNDeserializationOptions::getDefaultOptions() {
                        return defaultOptions;
                    }

                    bool ATNDeserializationOptions::isReadOnly() {
                        return readOnly;
                    }

                    void ATNDeserializationOptions::makeReadOnly() {
                        readOnly = true;
                    }

                    bool ATNDeserializationOptions::isVerifyATN() {
                        return verifyATN;
                    }

                    void ATNDeserializationOptions::setVerifyATN(bool verifyATN) {
                        throwIfReadOnly();
                        this->verifyATN = verifyATN;
                    }

                    bool ATNDeserializationOptions::isGenerateRuleBypassTransitions() {
                        return generateRuleBypassTransitions;
                    }

                    void ATNDeserializationOptions::setGenerateRuleBypassTransitions(bool generateRuleBypassTransitions) {
                        throwIfReadOnly();
                        this->generateRuleBypassTransitions = generateRuleBypassTransitions;
                    }

                    void ATNDeserializationOptions::throwIfReadOnly() {
                        if (isReadOnly()) {
                            throw L"The object is read only.";
                        }
                    }

                    void ATNDeserializationOptions::InitializeInstanceFields() {
                        readOnly = false;
                        verifyATN = false;
                        generateRuleBypassTransitions = false;
                    }
                }
            }
        }
    }
}

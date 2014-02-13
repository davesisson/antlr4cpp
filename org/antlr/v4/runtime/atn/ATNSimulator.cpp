#include "ATNSimulator.h"
#include "ATNDeserializer.h"
#include "ATNConfigSet.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    const int ATNSimulator::SERIALIZED_VERSION = 0;

//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not allowed in native C++:
                    ATNSimulator::ATNSimulator() {
                        SERIALIZED_VERSION = ATNDeserializer::SERIALIZED_VERSION;
                        SERIALIZED_UUID = ATNDeserializer::SERIALIZED_UUID;
                        ERROR = new DFAState(new ATNConfigSet());
                        ERROR->stateNumber = int::MAX_VALUE;
                    }

                    ATNSimulator::ATNSimulator(ATN *atn, PredictionContextCache *sharedContextCache) : atn(atn), sharedContextCache(sharedContextCache) {
                    }

                    org::antlr::v4::runtime::atn::PredictionContextCache *ATNSimulator::getSharedContextCache() {
                        return sharedContextCache;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *ATNSimulator::getCachedContext(PredictionContext *context) {
                        if (sharedContextCache == nullptr) {
                            return context;
                        }

//JAVA TO C++ CONVERTER TODO TASK: There is no built-in support for multithreading in native C++:
                        synchronized(sharedContextCache) {
                            IdentityHashMap<PredictionContext*, PredictionContext*> *visited = new IdentityHashMap<PredictionContext*, PredictionContext*>();
                            return PredictionContext::getCachedContext(context, sharedContextCache, visited);
                        }
                    }

                    org::antlr::v4::runtime::atn::ATN *ATNSimulator::deserialize(wchar_t data[]) {
                        return (new ATNDeserializer())->deserialize(data);
                    }

                    void ATNSimulator::checkCondition(bool condition) {
                        (new ATNDeserializer())->checkCondition(condition);
                    }

                    void ATNSimulator::checkCondition(bool condition, const std::wstring &message) {
                        (new ATNDeserializer())->checkCondition(condition, message);
                    }

                    int ATNSimulator::toInt(wchar_t c) {
                        return ATNDeserializer::toInt(c);
                    }

                    int ATNSimulator::toInt32(wchar_t data[], int offset) {
                        return ATNDeserializer::toInt32(data, offset);
                    }

                    long long ATNSimulator::toLong(wchar_t data[], int offset) {
                        return ATNDeserializer::toLong(data, offset);
                    }

                    UUID *ATNSimulator::toUUID(wchar_t data[], int offset) {
                        return ATNDeserializer::toUUID(data, offset);
                    }

                    org::antlr::v4::runtime::atn::Transition *ATNSimulator::edgeFactory(ATN *atn, int type, int src, int trg, int arg1, int arg2, int arg3, std::vector<IntervalSet*> &sets) {
                        return (new ATNDeserializer())->edgeFactory(atn, type, src, trg, arg1, arg2, arg3, sets);
                    }

                    org::antlr::v4::runtime::atn::ATNState *ATNSimulator::stateFactory(int type, int ruleIndex) {
                        return (new ATNDeserializer())->stateFactory(type, ruleIndex);
                    }
                }
            }
        }
    }
}

#include "PredictionContextCache.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContextCache::add(PredictionContext *ctx) {
                        if (ctx == PredictionContext::EMPTY) {
                            return PredictionContext::EMPTY;
                        }
                        PredictionContext *existing = cache->get(ctx);
                        if (existing != nullptr) {
                                        //			System.out.println(name+" reuses "+existing);
                            return existing;
                        }
                        cache->put(ctx, ctx);
                        return ctx;
                    }

                    org::antlr::v4::runtime::atn::PredictionContext *PredictionContextCache::get(PredictionContext *ctx) {
                        return cache->get(ctx);
                    }

                    int PredictionContextCache::size() {
                        return cache->size();
                    }

                    void PredictionContextCache::InitializeInstanceFields() {
                        cache = std::unordered_map<PredictionContext*, PredictionContext*>();
                    }
                }
            }
        }
    }
}

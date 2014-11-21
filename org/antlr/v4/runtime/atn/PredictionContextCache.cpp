#include "PredictionContextCache.h"

#include <map>

#include "PredictionContext.h"

namespace org {
namespace antlr {
namespace v4 {
namespace runtime {
namespace atn {

org::antlr::v4::runtime::atn::PredictionContext *PredictionContextCache::add(
    PredictionContext *ctx) {
  if (ctx == PredictionContext::EMPTY) {
    return PredictionContext::EMPTY;
  }
  PredictionContext *existing = cache->at(ctx);
  if (existing != nullptr) {
    //			System.out.println(name+" reuses "+existing);
    return existing;
  }
  cache->insert(std::pair<PredictionContext *, PredictionContext *>(ctx, ctx));
  return ctx;
}

org::antlr::v4::runtime::atn::PredictionContext *PredictionContextCache::get(
    PredictionContext *ctx) {
  return cache->at(ctx);
}

size_t PredictionContextCache::size() { return cache->size(); }

void PredictionContextCache::InitializeInstanceFields() {
  cache = new std::map<PredictionContext *, PredictionContext *>();
}

}  // namespace atn
}  // namespace runtime
}  // namespace v4
}  // namespace antlr
}  // namespace org

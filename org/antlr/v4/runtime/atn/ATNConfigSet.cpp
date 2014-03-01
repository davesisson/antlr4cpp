#include "ATNConfigSet.h"
#include "ATN.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {

template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public AbstractConfigHashSet(org.antlr.v4.runtime.misc.AbstractEqualityComparator<? super ATNConfig> comparator)
//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfigSet::AbstractConfigHashSet::AbstractConfigHashSet(AbstractEqualityComparator<T1> *comparator) {
                    }

template<typename T1>
//JAVA TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the Java 'super' constraint:
//ORIGINAL LINE: public AbstractConfigHashSet(org.antlr.v4.runtime.misc.AbstractEqualityComparator<? super ATNConfig> comparator, int initialCapacity, int initialBucketCapacity)
                    ATNConfigSet::AbstractConfigHashSet::AbstractConfigHashSet(AbstractEqualityComparator<T1> *comparator, int initialCapacity, int initialBucketCapacity) : org::antlr::v4::runtime::misc::Array2DHashSet<ATNConfig>(comparator, initialCapacity, initialBucketCapacity) {
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ATNConfigSet::AbstractConfigHashSet::asElementType(void *o) {
                        if (!(dynamic_cast<ATNConfig*>(o) != nullptr)) {
                            return nullptr;
                        }

                        return static_cast<ATNConfig*>(o);
                    }

                    ATNConfig **ATNConfigSet::AbstractConfigHashSet::createBuckets(int capacity) {
                        return new ATNConfig[capacity][];
                    }

                    ATNConfig *ATNConfigSet::AbstractConfigHashSet::createBucket(int capacity) {
                        return new ATNConfig[capacity];
                    }

                    ATNConfigSet::ConfigHashSet::ConfigHashSet() : AbstractConfigHashSet(ConfigEqualityComparator::INSTANCE) {
                    }

ConfigEqualityComparator *const ATNConfigSet::ConfigEqualityComparator::INSTANCE = new ConfigEqualityComparator();

                    ATNConfigSet::ConfigEqualityComparator::ConfigEqualityComparator() {
                    }

                    int ATNConfigSet::ConfigEqualityComparator::hashCode(ATNConfig *o) {
                        int hashCode = 7;
                        hashCode = 31 * hashCode + o->state->stateNumber;
                        hashCode = 31 * hashCode + o->alt;
                        hashCode = 31 * hashCode + o->semanticContext->hashCode();
                        return hashCode;
                    }

                    bool ATNConfigSet::ConfigEqualityComparator::equals(ATNConfig *a, ATNConfig *b) {
                        if (a == b) {
                            return true;
                        }
                        if (a == nullptr || b == nullptr) {
                            return false;
                        }
                        return a->state->stateNumber == b->state->stateNumber && a->alt == b->alt && a->semanticContext->equals(b->semanticContext);
                    }

                    ATNConfigSet::ATNConfigSet(bool fullCtx) : fullCtx(fullCtx), configs(new java::util::ArrayList<ATNConfig>(7)) {
                        InitializeInstanceFields();
                        configLookup = new ConfigHashSet();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfigSet::ATNConfigSet() : configs(new java::util::ArrayList<ATNConfig>(7)) {
                    }

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
                    ATNConfigSet::ATNConfigSet(ATNConfigSet *old) : configs(new java::util::ArrayList<ATNConfig>(7)) {
                        addAll(old);
                        this->uniqueAlt = old->uniqueAlt;
                        this->conflictingAlts = old->conflictingAlts;
                        this->hasSemanticContext = old->hasSemanticContext;
                        this->dipsIntoOuterContext = old->dipsIntoOuterContext;
                    }

                    bool ATNConfigSet::add(ATNConfig *config) {
                        return add(config, nullptr);
                    }

                    bool ATNConfigSet::add(ATNConfig *config, DoubleKeyMap<PredictionContext*, PredictionContext*, PredictionContext*> *mergeCache) {
                        if (readonly) {
                            throw IllegalStateException(L"This set is readonly");
                        }
                        if (config->semanticContext != SemanticContext::NONE) {
                            hasSemanticContext = true;
                        }
                        if (config->reachesIntoOuterContext > 0) {
                            dipsIntoOuterContext = true;
                        }
                        ATNConfig *existing = configLookup->getOrAdd(config);
                        if (existing == config) { // we added this new one
                            cachedHashCode = -1;
                            configs.push_back(config); // track order here
                            return true;
                        }
                        // a previous (s,i,pi,_), merge with it and save result
                        bool rootIsWildcard = !fullCtx;
                        PredictionContext *merged = PredictionContext::merge(existing->context, config->context, rootIsWildcard, mergeCache);
                        // no need to check for existing.context, config.context in cache
                        // since only way to create new graphs is "call rule" and here. We
                        // cache at both places.
                        existing->reachesIntoOuterContext = std::max(existing->reachesIntoOuterContext, config->reachesIntoOuterContext);
                        existing->context = merged; // replace context; no need to alt mapping
                        return true;
                    }

                    std::vector<ATNConfig*> ATNConfigSet::elements() {
                        return configs;
                    }

                    Set<ATNState*> *ATNConfigSet::getStates() {
                        Set<ATNState*> *states = std::set<ATNState*>();
                        for (auto c : configs) {
                            states->add(c.state);
                        }
                        return states;
                    }

                    std::vector<SemanticContext*> ATNConfigSet::getPredicates() {
                        std::vector<SemanticContext*> preds = std::vector<SemanticContext*>();
                        for (auto c : configs) {
                            if (c.semanticContext != SemanticContext::NONE) {
                                preds.push_back(c.semanticContext);
                            }
                        }
                        return preds;
                    }

                    org::antlr::v4::runtime::atn::ATNConfig *ATNConfigSet::get(int i) {
                        return configs[i];
                    }

                    void ATNConfigSet::optimizeConfigs(ATNSimulator *interpreter) {
                        if (readonly) {
                            throw IllegalStateException(L"This set is readonly");
                        }
                        if (configLookup->isEmpty()) {
                            return;
                        }

                        for (auto config : configs) {
                                        //			int before = PredictionContext.getAllContextNodes(config.context).size();
                            config.context = interpreter->getCachedContext(config.context);
                                        //			int after = PredictionContext.getAllContextNodes(config.context).size();
                                        //			System.out.println("configs "+before+"->"+after);
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T1> where T1 : ATNConfig
                    bool ATNConfigSet::addAll(Collection<T1> *coll) {
                        for (auto c : coll) {
                            add(c);
                        }
                        return false;
                    }

                    bool ATNConfigSet::equals(void *o) {
                        if (o == this) {
                            return true;
                        } else if (!(dynamic_cast<ATNConfigSet*>(o) != nullptr)) {
                            return false;
                        }

                                        //		System.out.print("equals " + this + ", " + o+" = ");
                        ATNConfigSet *other = static_cast<ATNConfigSet*>(o);
                        bool same = configs.size() > 0 && configs.equals(other->configs) && this->fullCtx == other->fullCtx && this->uniqueAlt == other->uniqueAlt && this->conflictingAlts == other->conflictingAlts && this->hasSemanticContext == other->hasSemanticContext && this->dipsIntoOuterContext == other->dipsIntoOuterContext; // includes stack context

                                        //		System.out.println(same);
                        return same;
                    }

                    int ATNConfigSet::hashCode() {
                        if (isReadonly()) {
                            if (cachedHashCode == -1) {
                                cachedHashCode = configs.hashCode();
                            }

                            return cachedHashCode;
                        }

                        return configs.hashCode();
                    }

                    int ATNConfigSet::size() {
                        return configs.size();
                    }

                    bool ATNConfigSet::isEmpty() {
                        return configs.empty();
                    }

                    bool ATNConfigSet::contains(void *o) {
                        if (configLookup == nullptr) {
                            throw UnsupportedOperationException(L"This method is not implemented for readonly sets.");
                        }

                        return configLookup->contains(o);
                    }

                    bool ATNConfigSet::containsFast(ATNConfig *obj) {
                        if (configLookup == nullptr) {
                            throw UnsupportedOperationException(L"This method is not implemented for readonly sets.");
                        }

                        return configLookup->containsFast(obj);
                    }

                    Iterator<ATNConfig*> *ATNConfigSet::iterator() {
                        return configs.begin();
                    }

                    void ATNConfigSet::clear() {
                        if (readonly) {
                            throw IllegalStateException(L"This set is readonly");
                        }
                        configs.clear();
                        cachedHashCode = -1;
                        configLookup->clear();
                    }

                    bool ATNConfigSet::isReadonly() {
                        return readonly;
                    }

                    void ATNConfigSet::setReadonly(bool readonly) {
                        this->readonly = readonly;
//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
                        delete configLookup; // can't mod, no need for lookup cache
                    }

                    std::wstring ATNConfigSet::toString() {
                        StringBuilder *buf = new StringBuilder();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        buf->append(elements().toString());
                        if (hasSemanticContext) {
                            buf->append(L",hasSemanticContext=")->append(hasSemanticContext);
                        }
                        if (uniqueAlt != ATN::INVALID_ALT_NUMBER) {
                            buf->append(L",uniqueAlt=")->append(uniqueAlt);
                        }
                        if (conflictingAlts != nullptr) {
                            buf->append(L",conflictingAlts=")->append(conflictingAlts);
                        }
                        if (dipsIntoOuterContext) {
                            buf->append(L",dipsIntoOuterContext");
                        }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return buf->toString();
                    }

                    ATNConfig *ATNConfigSet::toArray() {
                        return configLookup->toArray();
                    }

template<typename T>
                    T *ATNConfigSet::toArray(T a[]) {
                        return configLookup->toArray(a);
                    }

                    bool ATNConfigSet::remove(void *o) {
                        throw UnsupportedOperationException();
                    }

template<typename T1>
                    bool ATNConfigSet::containsAll(Collection<T1> *c) {
                        throw UnsupportedOperationException();
                    }

template<typename T1>
                    bool ATNConfigSet::retainAll(Collection<T1> *c) {
                        throw UnsupportedOperationException();
                    }

template<typename T1>
                    bool ATNConfigSet::removeAll(Collection<T1> *c) {
                        throw UnsupportedOperationException();
                    }

                    void ATNConfigSet::InitializeInstanceFields() {
                        readonly = false;
                        uniqueAlt = 0;
                        hasSemanticContext = false;
                        dipsIntoOuterContext = false;
                        cachedHashCode = -1;
                    }
                }
            }
        }
    }
}

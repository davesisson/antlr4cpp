#include "Transition.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::misc::IntervalSet;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Nullable;
const std::vector<std::wstring> Transition::serializationNames = java::util::Collections::unmodifiableList(java::util::Arrays::asList(L"INVALID", L"EPSILON", L"RANGE", L"RULE", L"PREDICATE", L"ATOM", L"ACTION", L"SET", L"NOT_SET", L"WILDCARD", L"PRECEDENCE"));
java::util::Map<Class*, int> *const Transition::serializationTypes = java::util::Collections::unmodifiableMap(std::unordered_map<Class*, int>() {{put(EpsilonTransition::typeid, EPSILON); put(RangeTransition::typeid, RANGE); put(RuleTransition::typeid, RULE); put(PredicateTransition::typeid, PREDICATE); put(AtomTransition::typeid, ATOM); put(ActionTransition::typeid, ACTION); put(SetTransition::typeid, SET); put(NotSetTransition::typeid, NOT_SET); put(WildcardTransition::typeid, WILDCARD); put(PrecedencePredicateTransition::typeid, PRECEDENCE);}});

                    Transition::Transition(ATNState *target) {
                        if (target == nullptr) {
                            throw NullPointerException(L"target cannot be null.");
                        }

                        this->target = target;
                    }

                    bool Transition::isEpsilon() {
                        return false;
                    }

                    org::antlr::v4::runtime::misc::IntervalSet *Transition::label() {
                        return nullptr;
                    }
                }
            }
        }
    }
}

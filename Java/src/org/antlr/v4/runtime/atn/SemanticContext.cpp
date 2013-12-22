#include "SemanticContext.h"
#include "Java/src/org/antlr/v4/runtime/misc/MurmurHash.h"
#include "Java/src/org/antlr/v4/runtime/misc/Utils.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace atn {
                    using org::antlr::v4::runtime::Recognizer;
                    using org::antlr::v4::runtime::RuleContext;
                    using org::antlr::v4::runtime::misc::MurmurHash;
                    using org::antlr::v4::runtime::misc::NotNull;
                    using org::antlr::v4::runtime::misc::Utils;

                    SemanticContext::Predicate::Predicate() : ruleIndex(-1), predIndex(-1), isCtxDependent(false) {
                    }

                    SemanticContext::Predicate::Predicate(int ruleIndex, int predIndex, bool isCtxDependent) : ruleIndex(ruleIndex), predIndex(predIndex), isCtxDependent(isCtxDependent) {
                    }

template<typename T1, typename T1>
                    bool SemanticContext::Predicate::eval(Recognizer<T1> *parser, RuleContext *outerContext) {
                        RuleContext *localctx = isCtxDependent ? outerContext : nullptr;
                        return parser->sempred(localctx, ruleIndex, predIndex);
                    }

                    int SemanticContext::Predicate::hashCode() {
                        int hashCode = MurmurHash::initialize();
                        hashCode = MurmurHash::update(hashCode, ruleIndex);
                        hashCode = MurmurHash::update(hashCode, predIndex);
                        hashCode = MurmurHash::update(hashCode, isCtxDependent ? 1 : 0);
                        hashCode = MurmurHash::finish(hashCode, 3);
                        return hashCode;
                    }

                    bool SemanticContext::Predicate::equals(void *obj) {
                        if (!(dynamic_cast<Predicate*>(obj) != nullptr)) {
                            return false;
                        }
                        if (this == obj) {
                            return true;
                        }
                        Predicate *p = static_cast<Predicate*>(obj);
                        return this->ruleIndex == p->ruleIndex && this->predIndex == p->predIndex && this->isCtxDependent == p->isCtxDependent;
                    }

                    std::wstring SemanticContext::Predicate::toString() {
                        return std::wstring(L"{") + ruleIndex + std::wstring(L":") + predIndex + std::wstring(L"}?");
                    }

                    SemanticContext::PrecedencePredicate::PrecedencePredicate() : precedence(0) {
                    }

                    SemanticContext::PrecedencePredicate::PrecedencePredicate(int precedence) : precedence(precedence) {
                    }

template<typename T1, typename T1>
                    bool SemanticContext::PrecedencePredicate::eval(Recognizer<T1> *parser, RuleContext *outerContext) {
                        return parser->precpred(outerContext, precedence);
                    }

                    int SemanticContext::PrecedencePredicate::compareTo(PrecedencePredicate *o) {
                        return precedence - o->precedence;
                    }

                    int SemanticContext::PrecedencePredicate::hashCode() {
                        int hashCode = 1;
                        hashCode = 31 * hashCode + precedence;
                        return hashCode;
                    }

                    bool SemanticContext::PrecedencePredicate::equals(void *obj) {
                        if (!(dynamic_cast<PrecedencePredicate*>(obj) != nullptr)) {
                            return false;
                        }

                        if (this == obj) {
                            return true;
                        }

                        PrecedencePredicate *other = static_cast<PrecedencePredicate*>(obj);
                        return this->precedence == other->precedence;
                    }

                    std::wstring SemanticContext::PrecedencePredicate::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return SemanticContext::toString();
                    }

                    SemanticContext::AND::AND(SemanticContext *a, SemanticContext *b) : opnds(operands::toArray(new SemanticContext[operands->size()])) {
                        Set<SemanticContext*> *operands = std::set<SemanticContext*>();
                        if (dynamic_cast<AND*>(a) != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<AND*>(a))->opnds));
                        } else {
                            operands->add(a);
                        }
                        if (dynamic_cast<AND*>(b) != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<AND*>(b))->opnds));
                        } else {
                            operands->add(b);
                        }

                        std::vector<PrecedencePredicate*> precedencePredicates = filterPrecedencePredicates(operands);
                        if (!precedencePredicates.empty()) {
                            // interested in the transition with the lowest precedence
                            PrecedencePredicate *reduced = Collections::min(precedencePredicates);
                            operands->add(reduced);
                        }

                    }

                    bool SemanticContext::AND::equals(void *obj) {
                        if (this == obj) {
                            return true;
                        }
                        if (!(dynamic_cast<AND*>(obj) != nullptr)) {
                            return false;
                        }
                        AND *other = static_cast<AND*>(obj);
                        return Arrays::equals(this->opnds, other->opnds);
                    }

                    int SemanticContext::AND::hashCode() {
                        return MurmurHash::hashCode(opnds, AND::typeid::hashCode());
                    }

template<typename T1, typename T1>
                    bool SemanticContext::AND::eval(Recognizer<T1> *parser, RuleContext *outerContext) {
                        for (auto opnd : opnds) {
                            if (!opnd->eval(parser, outerContext)) {
                                return false;
                            }
                        }
                        return true;
                    }

                    std::wstring SemanticContext::AND::toString() {
                        return Utils::join(Arrays::asList(opnds)->begin(), L"&&");
                    }

                    SemanticContext::OR::OR(SemanticContext *a, SemanticContext *b) : opnds(operands::toArray(new SemanticContext[operands->size()])) {
                        Set<SemanticContext*> *operands = std::set<SemanticContext*>();
                        if (dynamic_cast<OR*>(a) != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<OR*>(a))->opnds));
                        } else {
                            operands->add(a);
                        }
                        if (dynamic_cast<OR*>(b) != nullptr) {
                            operands->addAll(Arrays::asList((static_cast<OR*>(b))->opnds));
                        } else {
                            operands->add(b);
                        }

                        std::vector<PrecedencePredicate*> precedencePredicates = filterPrecedencePredicates(operands);
                        if (!precedencePredicates.empty()) {
                            // interested in the transition with the highest precedence
                            PrecedencePredicate *reduced = Collections::max(precedencePredicates);
                            operands->add(reduced);
                        }

                    }

                    bool SemanticContext::OR::equals(void *obj) {
                        if (this == obj) {
                            return true;
                        }
                        if (!(dynamic_cast<OR*>(obj) != nullptr)) {
                            return false;
                        }
                        OR *other = static_cast<OR*>(obj);
                        return Arrays::equals(this->opnds, other->opnds);
                    }

                    int SemanticContext::OR::hashCode() {
                        return MurmurHash::hashCode(opnds, OR::typeid::hashCode());
                    }

template<typename T1, typename T1>
                    bool SemanticContext::OR::eval(Recognizer<T1> *parser, RuleContext *outerContext) {
                        for (auto opnd : opnds) {
                            if (opnd->eval(parser, outerContext)) {
                                return true;
                            }
                        }
                        return false;
                    }

                    std::wstring SemanticContext::OR::toString() {
                        return Utils::join(Arrays::asList(opnds)->begin(), L"||");
                    }

SemanticContext *const SemanticContext::NONE = new Predicate();

                    org::antlr::v4::runtime::atn::SemanticContext *SemanticContext::and(SemanticContext *a, SemanticContext *b) {
                        if (a == nullptr || a == NONE) {
                            return b;
                        }
                        if (b == nullptr || b == NONE) {
                            return a;
                        }
                        AND *result = new AND(a, b);
                        if (result->opnds->length == 1) {
                            return result->opnds[0];
                        }

                        return result;
                    }

                    org::antlr::v4::runtime::atn::SemanticContext *SemanticContext::or(SemanticContext *a, SemanticContext *b) {
                        if (a == nullptr) {
                            return b;
                        }
                        if (b == nullptr) {
                            return a;
                        }
                        if (a == NONE || b == NONE) {
                            return NONE;
                        }
                        OR *result = new OR(a, b);
                        if (result->opnds->length == 1) {
                            return result->opnds[0];
                        }

                        return result;
                    }

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to generic constraints:
template<typename T1> where T1 : SemanticContext
                    std::vector<PrecedencePredicate*> SemanticContext::filterPrecedencePredicates(Collection<T1> *collection) {
                        std::vector<PrecedencePredicate*> result;
                        for (Collection<? extends SemanticContext>::const_iterator iterator = collection->begin(); iterator != collection->end(); ++iterator) {
                            SemanticContext *context = *iterator;
                            if (dynamic_cast<PrecedencePredicate*>(context) != nullptr) {
                                if (result.empty()) {
                                    result = std::vector<PrecedencePredicate*>();
                                }

                                result.push_back(static_cast<PrecedencePredicate*>(context));
                                (*iterator)->remove();
                            }
                        }

                        if (result.empty()) {
                            return Collections::emptyList();
                        }

                        return result;
                    }
                }
            }
        }
    }
}

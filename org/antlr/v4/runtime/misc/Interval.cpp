#include "Interval.h"

#ifdef WINDOWS
#include <algorithm>
#endif 

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

                    Interval *const Interval::INVALID = new Interval(-1,-2);
                    Interval *      Interval::cache[Interval::INTERVAL_POOL_MAX_VALUE+1];
                    
                    int Interval::creates = 0;
                    int Interval::misses = 0;
                    int Interval::hits = 0;
                    int Interval::outOfRange = 0;

                    Interval::Interval(int a, int b) {
                        InitializeInstanceFields();
                        this->a = a;
                        this->b = b;
                    }

                    org::antlr::v4::runtime::misc::Interval *Interval::of(int a, int b) {
                        // cache just a..a
                        if (a != b || a < 0 || a>INTERVAL_POOL_MAX_VALUE) {
                            return new Interval(a,b);
                        }
                        if (cache[a] == nullptr) {
                            cache[a] = new Interval(a,a);
                        }
                        return cache[a];
                    }

                    int Interval::length() {
                        if (b < a) {
                            return 0;
                        }
                        return b - a + 1;
                    }

                    bool Interval::equals(void *o) {
                        if (o == nullptr || !( ((Interval*)o) != nullptr)) {
                            return false;
                        }
                        Interval *other = static_cast<Interval*>(o);
                        return this->a == other->a && this->b == other->b;
                    }

                    int Interval::hashCode() {
                        int hash = 23;
                        hash = hash * 31 + a;
                        hash = hash * 31 + b;
                        return hash;
                    }

                    bool Interval::startsBeforeDisjoint(Interval *other) {
                        return this->a < other->a && this->b < other->a;
                    }

                    bool Interval::startsBeforeNonDisjoint(Interval *other) {
                        return this->a <= other->a && this->b >= other->a;
                    }

                    bool Interval::startsAfter(Interval *other) {
                        return this->a > other->a;
                    }

                    bool Interval::startsAfterDisjoint(Interval *other) {
                        return this->a > other->b;
                    }

                    bool Interval::startsAfterNonDisjoint(Interval *other) {
                        return this->a > other->a && this->a <= other->b; // this.b>=other.b implied
                    }

                    bool Interval::disjoint(Interval *other) {
                        return startsBeforeDisjoint(other) || startsAfterDisjoint(other);
                    }

                    bool Interval::adjacent(Interval *other) {
                        return this->a == other->b + 1 || this->b == other->a - 1;
                    }

                    bool Interval::properlyContains(Interval *other) {
                        return other->a >= this->a && other->b <= this->b;
                    }

                    org::antlr::v4::runtime::misc::Interval *Interval::union_Renamed(Interval *other) {
                        return Interval::of(std::min(a, other->a), std::max(b, other->b));
                    }

                    org::antlr::v4::runtime::misc::Interval *Interval::intersection(Interval *other) {
                        return Interval::of(std::max(a, other->a), std::min(b, other->b));
                    }

                    org::antlr::v4::runtime::misc::Interval *Interval::differenceNotProperlyContained(Interval *other) {
                        Interval *diff = nullptr;
                        // other.a to left of this.a (or same)
                        if (other->startsBeforeNonDisjoint(this)) {
                            diff = Interval::of(std::max(this->a, other->b + 1), this->b);
                        }

                        // other.a to right of this.a
                        else if (other->startsAfterNonDisjoint(this)) {
                            diff = Interval::of(this->a, other->a - 1);
                        }
                        return diff;
                    }

                    std::wstring Interval::toString() {
                        return std::to_wstring(a) + std::wstring(L"..") + std::to_wstring(b);
                    }

                    void Interval::InitializeInstanceFields() {
                        a = 0;
                        b = 0;
                    }
                }
            }
        }
    }
}

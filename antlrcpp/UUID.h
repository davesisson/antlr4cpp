
#pragma once

#include <string>

class UUID {
public:
    UUID(long mostSigBits, long leastSigBits);
    UUID(const UUID &other);
    ~UUID();
    
    const UUID &operator=(const UUID &rhs);
    bool operator==(const UUID &rhs) const;
    bool equals(const UUID &rhs) const;
    bool equals(const UUID *rhs) const;
    
    static UUID *fromString(const std::wstring &name);
};


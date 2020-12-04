#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <list>
#include <limits>
#include <string>
#include <iostream>

#include "compare.h"

enum class Sign : int8_t
{
    zero = 0,
    positive = 1,
    negative = -1
};

inline Sign operator-(Sign sign)
{
    return static_cast<Sign>(
        -static_cast<std::underlying_type_t<Sign>>(sign));
}

inline Sign operator*(Sign lhs, Sign rhs)
{
    return static_cast<Sign>(
        static_cast<std::underlying_type_t<Sign>>(lhs) *
        static_cast<std::underlying_type_t<Sign>>(rhs));
}

inline Sign operator+(Sign lhs, Sign rhs)
{
    return static_cast<Sign>(
        (static_cast<std::underlying_type_t<Sign>>(lhs) +
         static_cast<std::underlying_type_t<Sign>>(rhs)) /
        2);
}

inline std::strong_ordering operator<=>(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) <=>
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

inline bool operator==(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) ==
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

inline bool operator!=(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) !=
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

inline bool operator<(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) <
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

inline bool operator>(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) >
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

inline bool operator<=(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) <=
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

inline bool operator>=(Sign lhs, Sign rhs)
{
    return static_cast<std::underlying_type_t<Sign>>(lhs) >=
           static_cast<std::underlying_type_t<Sign>>(rhs);
}

class Bignum
{
public:
    using sint = std::int64_t;
    using uint = std::uint64_t;
    static constexpr size_t default_base = 2;

private:
    Sign sign;
    sint exponents;
    std::list<uint> fraction;

public:
    static Bignum random(size_t minSize, size_t maxSize);

    Bignum(Bignum &&) = default;
    Bignum(const Bignum &) = default;
    Bignum &operator=(Bignum &&) = default;
    Bignum &operator=(const Bignum &) = default;

    Bignum() : sign(Sign::zero), exponents(0){};
    Bignum(uint64_t n);
    Bignum(int64_t n);

    Bignum(std::string::const_iterator first,
           std::string::const_iterator last,
           size_t base = default_base,
           bool isSigned = true);

    Bignum(std::string s, size_t base = default_base, bool isSigned = true)
        : Bignum(s.cbegin(),
                 s.cend(),
                 base,
                 isSigned){};

private:
    void opposite() { sign = -sign; }

public:
    Bignum operator-() const
    {
        auto result = *this;
        result.opposite();
        return result;
    }

    Bignum abs() const
    {
        auto result = *this;
        result.sign = Sign::positive;
        return result;
    }

    void operator<<=(size_t n);
    void operator>>=(size_t n);
    Bignum operator<<(size_t n) const;
    Bignum operator>>(size_t n) const;

private:
    std::strong_ordering uCompare(const Bignum other) const;

public:
    std::strong_ordering operator<=>(const Bignum other) const
    {
        switch (sign + other.sign)
        {
        case Sign::positive:
            return uCompare(other);
        case Sign::negative:
            return 0 <=> uCompare(other);
        default:
            return 0 <=> 0;
        }
    }

    bool operator==(const Bignum other) const { return operator<=>(other) == 0; }
    bool operator!=(const Bignum other) const { return operator<=>(other) != 0; }
    bool operator<(const Bignum other) const { return operator<=>(other) < 0; }
    bool operator>(const Bignum other) const { return operator<=>(other) > 0; }
    bool operator<=(const Bignum other) const { return operator<=>(other) <= 0; }
    bool operator>=(const Bignum other) const { return operator<=>(other) >= 0; }

    // private:
    //     void uAddAssign(const Bignum other);

private:
    std::string uTo_string(size_t base = default_base) const;

public:
    std::string to_string(size_t base = default_base) const;
    operator std::string() const { return to_string(default_base); };
};

inline std::istream &operator>>(std::istream &is, Bignum &bignum)
{
    std::string s;
    is >> s;
    bignum = Bignum(s);
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Bignum &bignum)
{
    os << bignum.to_string();
    return os;
}

#endif

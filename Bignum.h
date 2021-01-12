#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <list>
#include <variant>
#include <algorithm>

#include "compare.h"

namespace BignumNS
{
    enum class Sign : int8_t
    {
        zero     = 0,
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
        switch (static_cast<std::underlying_type_t<Sign>>(lhs) +
                static_cast<std::underlying_type_t<Sign>>(rhs))
        {
            default:
                return Sign::zero;
            case 1:
            case 2:
                return Sign::positive;
            case -1:
            case -2:
                return Sign::negative;
        }
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

} // namespace BignumNS

class Bignum
{
public:
//    using sint = std::int64_t;
    using uint = std::uint64_t;
//    static constexpr size_t size = 64;
    static constexpr size_t default_base = 2;

private:
    // Bignum = sign * 2 ^ exp * (2 - 2^-f[0] + 2^-f[1] - 2^-f[2] + 2^-f[3] ... - 2^-f.back())
    BignumNS::Sign  sign;
    uint            exponents;
    std::list<uint> fraction;

public:
    Bignum(Bignum &&) = default;
    Bignum(const Bignum &) = default;
    Bignum &operator=(Bignum &&) = default;
    Bignum &operator=(const Bignum &) = default;
    Bignum() : sign(BignumNS::Sign::zero), exponents(0) {};

    explicit Bignum(int64_t n);
    explicit Bignum(uint64_t n);
    explicit Bignum(char n) : Bignum(static_cast<int64_t>(n)) {};
    explicit Bignum(short n) : Bignum(static_cast<int64_t>(n)) {};
    explicit Bignum(int n) : Bignum(static_cast<int64_t>(n)) {};
    explicit Bignum(long n) : Bignum(static_cast<int64_t>(n)) {};
    explicit Bignum(unsigned char n) : Bignum(static_cast<uint64_t>(n)) {};
    explicit Bignum(unsigned short n) : Bignum(static_cast<uint64_t>(n)) {};
    explicit Bignum(unsigned int n) : Bignum(static_cast<uint64_t>(n)) {};
    explicit Bignum(unsigned long n) : Bignum(static_cast<uint64_t>(n)) {};

    Bignum(std::string::const_iterator first,
           std::string::const_iterator last,
           size_t base = default_base,
           bool isSigned = true);

    explicit Bignum(const std::string &s,
                    size_t base = default_base,
                    bool isSigned = true)
        : Bignum(s.cbegin(),
                 s.cend(),
                 base,
                 isSigned) {};

private:
    // Inefficient, only for debugging
    bool isLegalIgnoreSign() const
    {
        bool isZero   = fraction.empty() && exponents == 0;
        bool isNoZero = !fraction.empty()
                        &&
                        fraction.back() <= exponents
                        &&
                        std::distance(fraction.cbegin(), fraction.cend()) % 2 == 1
                        &&
                        std::is_sorted(fraction.cbegin(), fraction.cend());
        return isZero || isNoZero;
    }

    // Inefficient, only for debugging
    bool isLegal() const
    {
        bool isZero   = sign == BignumNS::Sign::zero
                        &&
                        fraction.empty()
                        &&
                        exponents == 0;
        bool isNoZero = sign != BignumNS::Sign::zero
                        &&
                        !fraction.empty()
                        &&
                        fraction.back() <= exponents
                        &&
                        std::distance(fraction.cbegin(), fraction.cend()) % 2 == 1
                        &&
                        std::is_sorted(fraction.cbegin(), fraction.cend());
        return isZero || isNoZero;
    }

    void opposite() { sign = -sign; }

    std::list<uint>::iterator
    termOpposite(std::list<uint>::iterator it)
    {
        assert(it != fraction.end() && *it > 0);
        if (it != fraction.begin() &&
            *std::prev(it) == *it - 1)
        {
            return fraction.erase(std::prev(it));
        }
        else
        {
            return fraction.insert(it, *it - 1);
        }
    }

public:
    Bignum operator-() const
    {
        auto result = *this;
        result.opposite();
        return result;
    }

    Bignum abs() const
    {
        switch (sign)
        {
            case BignumNS::Sign::zero:
                return Bignum();
            case BignumNS::Sign::positive:
                return *this;
            case BignumNS::Sign::negative:
                auto result = *this;
                result.sign = BignumNS::Sign::positive;
                return result;
        }
    }

    void uLeftMoveAssign(size_t n);
    void uRightMoveAssign(size_t n);
    void operator<<=(size_t n)
    {
        assert(isLegal());
        uLeftMoveAssign(n);
        assert(isLegal());
    };
    void operator>>=(size_t n)
    {
        assert(isLegal());
        uRightMoveAssign(n);
        assert(isLegal());
    };
    Bignum operator<<(size_t n) const
    {
        Bignum result = *this;
        result <<= n;
        return result;
    }
    Bignum operator>>(size_t n) const
    {
        Bignum result = *this;
        result >>= n;
        return result;
    }

private:
public: // for test
    std::strong_ordering uCompare(const Bignum &other) const;

public:
    std::strong_ordering operator<=>(const Bignum &other) const
    {
        switch (sign + other.sign)
        {
            case BignumNS::Sign::positive:
                return uCompare(other);
            case BignumNS::Sign::negative:
                return 0 <=> uCompare(other);
            case BignumNS::Sign::zero:
                return sign <=> other.sign;
        }
    }

    bool operator==(const Bignum &other) const { return operator<=>(other) == 0; }
    bool operator!=(const Bignum &other) const { return operator<=>(other) != 0; }
    bool operator<(const Bignum &other) const { return operator<=>(other) < 0; }
    bool operator>(const Bignum &other) const { return operator<=>(other) > 0; }
    bool operator<=(const Bignum &other) const { return operator<=>(other) <= 0; }
    bool operator>=(const Bignum &other) const { return operator<=>(other) >= 0; }

private:
    void uAddTerm(std::list<uint>::iterator &it,
                  bool &isLPositive,
                  uint value,
                  bool isRPositive);

public: // for test

    void uAddAssign(const Bignum &other);
    void uSubAssign(const Bignum &other);

private:
public: // for test
    std::string uTo_string(size_t base = default_base) const;

public:
    std::string to_string(size_t base = default_base) const;
    std::string to_string(bool showSign, size_t base = default_base) const;
    explicit operator std::string() const { return to_string(default_base); };
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

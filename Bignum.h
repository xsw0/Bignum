#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <list>
#include <variant>

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

class Bignum
{
public:
    using sint = std::int64_t;
    // using uint = std::uint64_t;
    // static constexpr size_t size = 64;
    static constexpr size_t default_base = 2;
    // static constexpr bool bit(sint bits, size_t index)
    // {
    //     assert(index < size);
    //     return !!(bits & (1 << index));
    // }
    // static constexpr bool bit(sint bits, size_t index)
    // {
    //     return bit(static_cast<sint>(bits), index);
    // }

    class Term
    {
    public:
        bool isPositive;
        sint exp;
        Term(sint exp, bool isPositive = false)
            : isPositive(isPositive), exp(exp) {}
        auto uCompare(const Term &other) const
        {
            return exp <=> other.exp;
        }

        Term &toDouble()
        {
            ++exp;
            return *this;
        }
    };

    class const_reverse_iterator
    {
    public:
        bool isPositive;

    private:
        using iterator = std::list<sint>::const_reverse_iterator;
        using InternalType =
            std::variant<std::nullptr_t,
                         iterator>;
        const Bignum &bignum;
        InternalType it;

    public:
        const_reverse_iterator(const Bignum &bignum,
                               InternalType it,
                               bool isPositive = false)
            : bignum(bignum), it(it), isPositive(isPositive) {}

        const_reverse_iterator &operator++()
        {
            assert(std::holds_alternative<iterator>(it));
            if (std::get<iterator>(it) != bignum.fraction.crend())
            {
                ++std::get<iterator>(it);
            }
            else
            {
                it = nullptr;
            }
            isPositive = !isPositive;
            return *this;
        }

        Term operator*() const
        {
            assert(std::holds_alternative<iterator>(it));
            if (std::get<iterator>(it) != bignum.fraction.crend())
            {
                return Term(bignum.exponents - *std::get<iterator>(it), isPositive);
            }
            else
            {
                assert(isPositive);
                return Term(bignum.exponents + 1, true);
            }
        }
        bool operator==(const const_reverse_iterator other) const
        {
            return isPositive == other.isPositive &&
                   it == other.it;
        }
        bool operator!=(const const_reverse_iterator other) const
        {
            return !operator==(other);
        }

        static const_reverse_iterator begin(const Bignum &bignum)
        {
            return const_reverse_iterator(bignum, bignum.fraction.crbegin(), false);
        }
        static const_reverse_iterator last(const Bignum &bignum)
        {
            return const_reverse_iterator(bignum, bignum.fraction.crend(), true);
        }
        static const_reverse_iterator end(const Bignum &bignum)
        {
            return const_reverse_iterator(bignum, nullptr, false);
        }
    };

    const_reverse_iterator cbegin() const
    {
        return const_reverse_iterator::last(*this);
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator::begin(*this);
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator::end(*this);
    }

    Term second() const
    {
        assert(!fraction.empty());
        return Term(exponents - fraction.front(), false);
    }

private:
    // Bignum = sign * 2 ^ exp * (2 - 2^-f[0] + 2^-f[1] - 2^-f[2] + 2^-f[3] ... - 2^-f.back())
    Sign sign;
    sint exponents;
    std::list<sint> fraction;

public:
    Bignum(Bignum &&) = default;
    Bignum(const Bignum &) = default;
    Bignum &operator=(Bignum &&) = default;
    Bignum &operator=(const Bignum &) = default;

    Bignum() : sign(Sign::zero), exponents(0){};
    Bignum(int64_t n);
    Bignum(uint64_t n);
    Bignum(int32_t n) : Bignum(static_cast<int64_t>(n)) {}
    Bignum(uint32_t n) : Bignum(static_cast<uint64_t>(n)) {}
    Bignum(size_t n) : Bignum(static_cast<uint64_t>(n)) {}

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
            return sign <=> other.sign;
        }
    }

    bool operator==(const Bignum other) const { return operator<=>(other) == 0; }
    bool operator!=(const Bignum other) const { return operator<=>(other) != 0; }
    bool operator<(const Bignum other) const { return operator<=>(other) < 0; }
    bool operator>(const Bignum other) const { return operator<=>(other) > 0; }
    bool operator<=(const Bignum other) const { return operator<=>(other) <= 0; }
    bool operator>=(const Bignum other) const { return operator<=>(other) >= 0; }

private:
    bool uAddTerm(const Term &term, bool isPositive)
    {
        assert(fraction.empty() || term.exp >= second().exp);
        if (!fraction.empty() &&
            second().exp == term.exp)
        {
            if (isPositive == term.isPositive)
            {
                fraction.pop_front();
                return !isPositive;
            }
            else
            {
                assert(fraction.front() > 0);
                --fraction.front();
                return isPositive;
            }
        }
        else
        {
            if (isPositive == term.isPositive)
            {
                fraction.push_front(exponents - term.exp);
                return !isPositive;
            }
            else
            {
                assert(exponents > term.exp);
                fraction.push_front(exponents - term.exp);
                fraction.push_front(exponents - term.exp - 1);
                return isPositive;
            }
        }
    }

public: // for test
    Bignum uAdd(const Bignum other) const;

private:
public: // for test
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

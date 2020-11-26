#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <list>
#include <limits>
#include <string>
#include <random>

class Bignum
{
public:
    enum class Sign : int8_t
    {
        zero = 0,
        positive = 1,
        negative = -1
    };

    using value_type = std::uint32_t;
    static constexpr size_t default_base = 2;

private:
    Sign sign = Sign::zero;
    size_t precision = 0;
    std::list<unsigned> _v;
    int64_t floating_point = 0;

    std::string uTo_string(size_t base = default_base) const;

public:
    static Bignum random(size_t minSize, size_t maxSize);

    Bignum(){};
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

    std::string to_string(size_t base = default_base) const;

    operator std::string() const { return to_string(default_base); };

    void operator<<=(size_t n);
    void operator>>=(size_t n);
    Bignum operator<<(size_t n) const;
    Bignum operator>>(size_t n) const;

private:
    void opposite();

public:
    Bignum operator+() const;
    Bignum operator-() const;
    Bignum abs() const;

    size_t size() const
    {
        assert(floating_point >= 0);
        return floating_point + precision;
    };

private:
    // void uAddAssign(const Bignum other);
};

#endif

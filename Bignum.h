#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <list>
#include <limits>
#include <string>

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

private:
    Sign sign = Sign::zero;
    size_t size = 0;
    std::list<unsigned> _v;

    std::string uTo_string(size_t base = 2) const;

public:
    Bignum(int64_t n = 0);

    Bignum(std::string::const_iterator first,
           std::string::const_iterator last,
           size_t base = 2,
           bool isSigned = true);

    Bignum(std::string s, size_t base = 2, bool isSigned = true)
        : Bignum(s.cbegin(),
                 s.cend(),
                 base,
                 isSigned){};

    std::string to_string(size_t base = 2) const;
};

#endif

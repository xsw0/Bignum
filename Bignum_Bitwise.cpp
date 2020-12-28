#include "Bignum.h"

void Bignum::operator<<=(size_t n)
{
    assert(!fraction.empty() && fraction.back() <= exponents ||
           fraction.empty() && exponents == 0);
    assert(exponents >= 0);
    if (!fraction.empty())
    {
        assert(std::numeric_limits<decltype(exponents)>::max() - exponents >= n);
        exponents += n;
    }
}

void Bignum::operator>>=(size_t n)
{
    assert(!fraction.empty() && fraction.back() <= exponents ||
           fraction.empty() && exponents == 0);
    assert(exponents >= 0);
    if (exponents < n)
    {
        *this = Bignum();
    }
    else
    {
        exponents -= n;
        while (fraction.back() > exponents)
        {
            if (std::next(fraction.crbegin()) == fraction.crend() ||
                *std::next(fraction.crbegin()) < exponents)
            {
                fraction.back() = exponents;
                break;
            }
            else
            {
                fraction.pop_back();
                fraction.pop_back();
            }
        }
    }
    assert(!fraction.empty() && fraction.back() <= exponents ||
           fraction.empty() && exponents == 0);
    assert(exponents >= 0);
}

Bignum Bignum::operator<<(size_t n) const
{
    Bignum result = *this;
    result <<= n;
    return result;
}

Bignum Bignum::operator>>(size_t n) const
{
    Bignum result = *this;
    result >>= n;
    return result;
}

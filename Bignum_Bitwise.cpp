#include "Bignum.h"

void Bignum::operator<<=(size_t n)
{
    if (sign != Sign::zero)
    {
        exponents += n;
    }
}

void Bignum::operator>>=(size_t n)
{
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

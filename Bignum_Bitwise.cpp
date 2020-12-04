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
    if (exponents < n)
    {
        *this = Bignum();
    }
    else
    {
        exponents -= n;
        while (fraction.back() > exponents)
        {
            fraction.pop_back();
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

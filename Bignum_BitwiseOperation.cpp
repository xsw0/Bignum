#include "Bignum.h"

void Bignum::operator<<=(size_t n)
{
    if (sign != Sign::zero)
    {
        ++size;
        if (isLastBitOne)
        {
            _v.push_back(n);
            isLastBitOne = false;
        }
        else
        {
            _v.back() += n;
        }
    }
}

void Bignum::operator>>=(size_t n)
{
    if (size > n)
    {
        size -= n;
        while (n >= _v.back())
        {
            n -= _v.back();
            _v.pop_back();
            isLastBitOne = !isLastBitOne;
        }
        _v.back() -= n;
    }
    else
    {
        size = 0;
        sign = Sign::zero;
        isLastBitOne = false;
        _v.clear();
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

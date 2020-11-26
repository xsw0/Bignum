#include "Bignum.h"

void Bignum::operator<<=(size_t n)
{
    if (sign != Sign::zero)
    {
        floating_point += n;
    }
}

void Bignum::operator>>=(size_t n)
{
    if (floating_point < n)
    {
        n -= floating_point;
        if (precision > n)
        {
            precision -= n;
            while (true)
            {
                if (n < _v.back())
                {
                    _v.back() -= n;
                    floating_point = 0;
                    break;
                }
                else
                {
                    n -= _v.back();
                    _v.pop_back();
                }

                if (n > _v.back())
                {
                    n -= _v.back();
                    _v.pop_back();
                }
                else
                {
                    floating_point = _v.back() - n;
                    _v.pop_back();
                    break;
                }
            }
            precision -= floating_point;
        }
        else
        {
            floating_point = 0;
            precision = 0;
            sign = Sign::zero;
            _v.clear();
        }
    }
    else
    {
        floating_point -= n;
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

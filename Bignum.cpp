#include "Bignum.h"

void Bignum::opposite()
{
    switch (sign)
    {
    case Sign::negative:
        sign = Sign::positive;
        return;
    case Sign::positive:
        sign = Sign::negative;
        return;
    default:
        return;
    }
}

Bignum Bignum::operator+() const
{
    return *this;
}

Bignum Bignum::operator-() const
{
    auto result = *this;
    result.opposite();
    return result;
}

Bignum Bignum::abs() const
{
    auto result = *this;
    result.sign = Sign::positive;
    return result;
}

#include "Bignum.h"

std::strong_ordering Bignum::uCompare(const Bignum other) const
{
    if (exponents != other.exponents)
    {
        return exponents <=> other.exponents;
    }
    auto itL = fraction.cbegin();
    auto itR = other.fraction.cbegin();
    while (itL != fraction.cend() &&
           itR != other.fraction.cend())
    {
        if (*itL != *itR)
        {
            return *itL <=> *itR;
        }
        ++itL, ++itR;
    }
    return (itL != fraction.cend()) <=> (itR != other.fraction.cend());
}

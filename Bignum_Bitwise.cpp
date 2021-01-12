#include "Bignum.h"

void Bignum::uLeftMoveAssign(size_t n)
{
    assert(isLegalIgnoreSign());
    if (!fraction.empty())
    {
        assert(std::numeric_limits<decltype(exponents)>::max() - exponents >= n);
        exponents += n;
    }
    assert(isLegalIgnoreSign());
}

void Bignum::uRightMoveAssign(size_t n)
{
    assert(isLegalIgnoreSign());
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
    assert(isLegalIgnoreSign());
}

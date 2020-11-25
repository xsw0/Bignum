#include "Bignum.h"

Bignum Bignum::random(size_t minSize, size_t maxSize)
{
    static std::default_random_engine e;
    auto size = std::uniform_int_distribution<size_t>(
        minSize,
        maxSize)(e);
    static std::uniform_int_distribution<int> ranBit(0, 1);
    if (size == 0)
    {
        return Bignum();
    }
    else
    {
        Bignum bignum;
        bignum.sign = ranBit(e) == 0 ? Sign::positive : Sign::negative;
        bignum._v = {1};
        bool b = true;
        bignum.size = size;
        for (size_t i = 1; i < size; ++i)
        {
            if (ranBit(e) == b)
            {
                ++bignum._v.back();
            }
            else
            {
                bignum._v.push_back(1);
                b = !b;
            }
        }
        bignum.isLastBitOne = b;
        return bignum;
    }
}

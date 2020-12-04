#include "Bignum.h"

#include <random>
Bignum Bignum::random(size_t minSize, size_t maxSize)
{
    static std::default_random_engine e;
    auto size = std::uniform_int_distribution<size_t>(
        minSize,
        maxSize)(e);
    if (size == 0)
    {
        return Bignum();
    }
    else
    {
        static std::uniform_int_distribution<int> ranBit(0, 1);

        static Bignum bignum;

        bignum.sign = ranBit(e) == 0 ? Sign::positive : Sign::negative;
        bignum.exponents = size - 1;

        bignum.fraction = {0};

        for (size_t i = 1; i < size; ++i)
        {
            if (ranBit(e) != 0)
            {
                bignum.fraction.push_back(i);
            }
        }
        return bignum;
    }
}

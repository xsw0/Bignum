#include "Bignum.h"

#include <bitset>

Bignum::Bignum(uint64_t n)
{
    switch (n)
    {
    case 0:
    {
        *this = Bignum();
        return;
    }
    default:
    {
        sign = Sign::positive;

        std::bitset<64> bs = n;

        exponents = 63;
        while (!bs[exponents])
        {
            --exponents;
        }

        for (size_t i = 0; i <= exponents; ++i)
        {
            if (bs[i])
            {
                fraction.push_front(exponents - i);
            }
        }
        return;
    }
    }
}

Bignum::Bignum(int64_t n)
{
    switch (n)
    {
    case 0:
        *this = Bignum();
        return;
    case std::numeric_limits<int64_t>::min():
        sign = Sign::negative;
        exponents = 62;
        for (uint i = 0; i < 63; ++i)
        {
            fraction.push_back(i);
        }
        return;
    default:
    {
        *this = Bignum(static_cast<uint64_t>(n < 0 ? -n : n));
        sign = n < 0 ? Sign::negative : Sign::positive;
        return;
    }
    }
}

Bignum::Bignum(std::string::const_iterator first,
               std::string::const_iterator last,
               size_t base,
               bool isSigned)
{
    assert(first != last);
    if (isSigned)
    {
        switch (*first)
        {
        case '+':
            *this = Bignum(next(first), last, base, false);
            return;
        case '-':
            *this = Bignum(next(first), last, base, false);
            sign = -sign;
            return;
        default:
            assert(std::isdigit(*first));
            *this = Bignum(first, last, base, false);
            return;
        }
    }
    else
    {
        switch (base)
        {
        case 2:
        {
            if (*first == '0')
            {
                assert(next(first) == last);
                *this = Bignum();
                return;
            }
            assert(*first == '1');
            exponents = 0;
            sign = Sign::positive;
            fraction.push_back(0);
            ++first;
            while (first != last)
            {
                assert(*first == '0' || *first == '1');
                ++exponents;
                if (*first == '1')
                {
                    fraction.push_back(exponents);
                }
                ++first;
            }
            return;
        }
        default:
            assert(0);
            return;
        }
    }
}

std::string Bignum::uTo_string(size_t base) const
{
    switch (base)
    {
    case 2:
    {
        if (sign == Sign::zero)
        {
            return "0";
        }

        std::string result;
        result.resize(exponents + 1, '0');
        for (auto index : fraction)
        {
            result[index] = '1';
        }
        return result;
    }
    default:
        assert(0);
        return "";
    }
}

std::string Bignum::to_string(size_t base) const
{
    switch (base)
    {
    case 2:
    {
        switch (sign)
        {
        case Sign::zero:
            return "0";
        case Sign::positive:
            return uTo_string(base);
        case Sign::negative:
            return std::string("-" + uTo_string(base));
        }
    }
    default:
        assert(0);
        return "";
    }
}

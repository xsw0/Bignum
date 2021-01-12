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
            sign = BignumNS::Sign::positive;

            std::bitset<64> bs = n;

            exponents = 63;

            while (!bs[exponents])
            {
                --exponents;
            }

            bool      preBit = true;
            for (uint i      = 1; i <= exponents; ++i)
            {
                if (bs[exponents - i] != preBit)
                {
                    fraction.push_back(i - 1);
                    preBit = bs[exponents - i];
                }
            }
            if (preBit)
            {
                fraction.push_back(exponents);
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
            sign      = BignumNS::Sign::negative;
            exponents = 63;
            fraction  = {0};
            return;
        default:
        {
            *this = Bignum(static_cast<uint64_t>(n < 0 ? -n : n));
            sign = n < 0 ? BignumNS::Sign::negative : BignumNS::Sign::positive;
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
                sign      = BignumNS::Sign::positive;
                ++first;
                bool preBit = true;
                while (first != last)
                {
                    assert(*first == '0' || *first == '1');
                    if (*first - '0' != preBit)
                    {
                        fraction.push_back(exponents);
                        preBit = !preBit;
                    }
                    ++exponents;
                    ++first;
                }
                if (preBit)
                {
                    fraction.push_back(exponents);
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
    assert(!fraction.empty() && fraction.back() <= exponents ||
           fraction.empty() && exponents == 0);
    switch (base)
    {
        case 2:
        {
            if (fraction.empty())
            {
                return "0";
            }
            std::string result;
            assert(exponents >= fraction.back());
            bool      b = true;
            for (auto index : fraction)
            {
                result.resize(index + 1, '0' + b);
                b = !b;
            }
            assert(!b);
            result.resize(exponents + 1, '0');
            return result;
        }
        default:
            assert(0);
            return "";
    }
}

std::string Bignum::to_string(size_t base) const
{
    assert(!fraction.empty() && fraction.back() <= exponents ||
           fraction.empty() && exponents == 0);
    switch (base)
    {
        case 2:
        {
            switch (sign)
            {
                case BignumNS::Sign::zero:
                    return "0";
                case BignumNS::Sign::positive:
                    return uTo_string(base);
                case BignumNS::Sign::negative:
                    return std::string("-" + uTo_string(base));
            }
        }
        default:
            assert(0);
            return {};
    }
}

std::string Bignum::to_string(bool showSign, size_t base) const
{
    assert(isLegal());
    switch (base)
    {
        case 2:
        {
            switch (sign)
            {
                case BignumNS::Sign::zero:
                    return "0";
                case BignumNS::Sign::positive:
                    return showSign ? std::string("+" + uTo_string(base)) : uTo_string(base);
                case BignumNS::Sign::negative:
                    return showSign ? std::string("-" + uTo_string(base)) : uTo_string(base);
            }
        }
        default:
            assert(0);
            return {};
    }
}

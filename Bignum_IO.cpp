#include "Bignum.h"

#include <limits>

Bignum::Bignum(int64_t n)
{
    if (n == 0)
    {
        sign = Sign::zero;
        precision = 0;
        floating_point = 0;
        return;
    }

    if (n < 0)
    {
        sign = Sign::negative;
        if (n == std::numeric_limits<int64_t>::min())
        {
            precision = 63;
            floating_point = 0;
            _v = {63};
            return;
        }
        n = -n;
    }
    else
    {
        sign = Sign::positive;
    }

    floating_point = 0;
    while (n % 2 == 0)
    {
        ++floating_point;
        n >>= 1;
    }

    bool b = true;
    n >>= 1;
    precision = 1;
    _v = {1};
    while (n > 0)
    {
        ++precision;
        if ((n % 2 != 0) == b)
        {
            ++_v.front();
        }
        else
        {
            b = !b;
            _v.push_front(1);
        }
        n >>= 1;
    };
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
            *this = std::move(Bignum(next(first), last, base, false));
            return;
        case '-':
            *this = std::move(Bignum(next(first), last, base, false));
            if (sign == Sign::positive)
            {
                sign = Sign::negative;
            }
            return;
        default:
            assert(std::isdigit(*first));
            *this = std::move(Bignum(first, last, base, false));
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
                sign = Sign::zero;
                precision = 0;
                floating_point = 0;
                return;
            }
            assert(*first == '1');
            floating_point = 0;

            --last;
            while (*last == '0')
            {
                --last;
                ++floating_point;
            }
            assert(*last == '1');
            ++last;

            sign = Sign::positive;
            precision = 1;
            _v = {1};
            ++first;
            bool b = true;
            while (first != last)
            {
                switch (*first)
                {
                case '0':
                    if (b)
                    {
                        _v.push_back(1);
                        b = false;
                    }
                    else
                    {
                        ++_v.back();
                    }
                    break;
                case '1':
                    if (b)
                    {
                        ++_v.back();
                    }
                    else
                    {
                        _v.push_back(1);
                        b = true;
                    }
                    break;
                default:
                    assert(0);
                    break;
                }
                ++precision;
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
        result.reserve(precision);
        value_type count = 0;
        bool b = true;
        for (auto i : _v)
        {
            count += i;
            result.resize(count, b ? '1' : '0');
            b = !b;
        }
        assert(floating_point >= 0);
        result.resize(count + floating_point, '0');
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

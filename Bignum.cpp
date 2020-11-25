#include "Bignum.h"

#include <limits>

Bignum::Bignum(int64_t n)
{
    if (n == 0)
    {
        sign = Sign::zero;
        size = 0;
        return;
    }

    if (n < 0)
    {
        sign = Sign::negative;
        if (n == std::numeric_limits<int64_t>::min())
        {
            size = 63;
            _v = {63};
            return;
        }
        n = -n;
    }
    else
    {
        sign = Sign::positive;
    }

    bool b = n % 2 != 0;
    n /= 2;
    size = 1;
    _v.push_back(1);
    while (n > 0)
    {
        ++size;
        if ((n % 2 != 0) == b)
        {
            ++_v.front();
        }
        else
        {
            b = !b;
            _v.push_front(1);
        }
        n /= 2;
    };
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
        result.reserve(size);
        value_type count = 0;
        bool b = true;
        for (auto i : _v)
        {
            count += i;
            result.resize(count, b ? '1' : '0');
            b = !b;
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

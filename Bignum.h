#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <list>
#include <limits>
#include <string>
#include <random>

class Bignum
{
public:
    enum class Sign : int8_t
    {
        zero = 0,
        positive = 1,
        negative = -1
    };

    using value_type = std::uint32_t;

private:
    Sign sign = Sign::zero;
    size_t size = 0;
    std::list<unsigned> _v;
    bool isLastBitOne = false;

    std::string uTo_string(size_t base = 2) const;

public:
    static Bignum random(size_t minSize, size_t maxSize)
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

    Bignum(int64_t n = 0);

    Bignum(std::string::const_iterator first,
           std::string::const_iterator last,
           size_t base = 2,
           bool isSigned = true);

    Bignum(std::string s, size_t base = 2, bool isSigned = true)
        : Bignum(s.cbegin(),
                 s.cend(),
                 base,
                 isSigned){};

    std::string to_string(size_t base = 2) const;

    void operator<<=(size_t n)
    {
        if (sign != Sign::zero)
        {
            ++size;
            if (isLastBitOne)
            {
                _v.push_back(n);
                isLastBitOne = false;
            }
            else
            {
                _v.back() += n;
            }
        }
    }

    void operator>>=(size_t n)
    {
        if (size > n)
        {
            size -= n;
            while (n >= _v.back())
            {
                n -= _v.back();
                _v.pop_back();
                isLastBitOne = !isLastBitOne;
            }
            _v.back() -= n;
        }
        else
        {
            size = 0;
            sign = Sign::zero;
            isLastBitOne = false;
            _v.clear();
        }
    }

    Bignum operator<<(size_t n) const
    {
        Bignum result = *this;
        result <<= n;
        return result;
    }

    Bignum operator>>(size_t n) const
    {
        Bignum result = *this;
        result >>= n;
        return result;
    }
};

#endif

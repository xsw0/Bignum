#include "Bignum.h"

void Bignum::uAddAssign(const Bignum other)
{
    assert(!fraction.empty() ||
           fraction.empty() &&
               exponents == 0);
    assert(!other.fraction.empty() ||
           other.fraction.empty() &&
               other.exponents == 0);

    if (other.fraction.empty())
    {
        return;
    }

    if (fraction.empty())
    {
        exponents = other.exponents;
        fraction = other.fraction;
        return;
    }

    auto reverse = [&](std::list<sint>::iterator it) {
        assert(it != fraction.end() && *it > 0);
        if (it != fraction.begin() &&
            *std::prev(it) == *it - 1)
        {
            return fraction.erase(std::prev(it));
        }
        else
        {
            return fraction.insert(it, *it - 1);
        }
    };

    auto uAddTerm = [&](std::list<sint>::iterator &it,
                        bool &isLPositive,
                        const sint value,
                        const bool isRPositive) {
        assert(it == fraction.end() || *it > value);
        if (it == fraction.begin())
        {
            assert(isLPositive);
            fraction.push_front(value);
            it = fraction.begin();
            isLPositive = false;
            if (!isRPositive)
            {
                assert(value > 0);
                fraction.push_front(value - 1);
            }
        }
        else
        {
            assert(*std::prev(it) <= value);
            if (*std::prev(it) == value)
            {
                if (isLPositive == isRPositive)
                {
                    assert(*std::prev(it) != 0);
                    if (std::prev(it) != fraction.begin() &&
                        *std::prev(std::prev(it)) == *it - 1)
                    {
                        fraction.erase(std::prev(it));
                        fraction.erase(std::prev(it));
                    }
                    else
                    {
                        --*std::prev(it);
                    }
                }
                else
                {
                    fraction.erase(std::prev(it));
                    assert(it != fraction.begin());
                    reverse(std::prev(it));
                }
            }
            else
            {
                assert(*std::prev(it) != 0);
                it = fraction.insert(it, value);
                if (isLPositive == isRPositive)
                {
                    it = reverse(it);
                }
                if (it != fraction.begin() &&
                    *std::prev(it) == *it - 1)
                {
                    fraction.erase(std::prev(it));
                    it = fraction.erase(it);
                    reverse(std::prev(it));
                }
            }
        }
    };

    bool isLPositive = false;
    bool isRPositive = false;
    auto itL = fraction.end();
    auto itR = other.fraction.cend();

    auto align = [&](sint offset = 0) {
        assert(itR != other.fraction.cbegin());
        while (itL != fraction.begin() && *std::prev(itL) > offset + *std::prev(itR))
        {
            --itL;
            isLPositive = !isLPositive;
        }
    };

    if (exponents > other.exponents)
    {
        auto order = exponents - fraction.front() <=> other.exponents + 1;
        auto newBeginR = other.fraction.cbegin();
        if (order == 0)
        {
            isLPositive = true;
            fraction.pop_front();
            while ((order = !fraction.empty() <=>
                            (newBeginR != other.fraction.cend())) == 0 &&
                   !fraction.empty() && newBeginR != other.fraction.cend() &&
                   (order = exponents - fraction.front() <=>
                            other.exponents - *newBeginR) == 0)
            {
                ++newBeginR;
                fraction.pop_front();
                isLPositive = !isLPositive;
            }
            if (isLPositive)
            {
                reverse(fraction.begin());
                isLPositive = false;
                order = 0 <=> order;
            }
        }
        if (order != 0)
        {
            if (order < 0)
            {
                ++exponents;
                for (auto &i : fraction)
                {
                    ++i;
                }
            }
            while (itR != newBeginR)
            {
                align(exponents - other.exponents);
                uAddTerm(itL,
                         isLPositive,
                         *std::prev(itR) + (exponents - other.exponents),
                         isRPositive);
                isRPositive = !isRPositive;
                ++itR;
            }
        }
        else
        {
            assert(fraction.empty() && newBeginR == other.fraction.end());
            fraction = {0};
        }
    }
    else if (exponents < other.exponents)
    {
        auto order = exponents + 1 <=> other.exponents - other.fraction.front();
        auto newBeginR = other.fraction.cbegin();
        if (order == 0)
        {
            isLPositive = true;
            ++newBeginR;
            while ((order = !fraction.empty() <=>
                            (newBeginR != other.fraction.cend())) == 0 &&
                   !fraction.empty() && newBeginR != other.fraction.cend() &&
                   (order = exponents - fraction.front() <=>
                            other.exponents - *newBeginR) == 0)
            {
                ++newBeginR;
                fraction.pop_front();
                isLPositive = !isLPositive;
            }
            if (isLPositive)
            {
                reverse(fraction.begin());
                isLPositive = false;
                order = 0 <=> order;
            }
        }
        if (order != 0)
        {
            if (order > 0)
            {
                for (auto &i : fraction)
                {
                    i += (other.exponents - exponents) + 1;
                }
                exponents = other.exponents + 1;

                while (itR != newBeginR)
                {
                    align(1);
                    uAddTerm(itL, isLPositive, 1 + *std::prev(itR), isRPositive);
                    isRPositive = !isRPositive;
                    ++itR;
                }
            }
            else
            {
                for (auto &i : fraction)
                {
                    i += (other.exponents - exponents);
                }
                exponents = other.exponents;

                while (itR != newBeginR)
                {
                    align();
                    uAddTerm(itL, isLPositive, *std::prev(itR), isRPositive);
                    isRPositive = !isRPositive;
                    ++itR;
                }
            }
        }
        else
        {
            assert(fraction.empty() && newBeginR == other.fraction.end());
            exponents = other.exponents;
            fraction = {0};
        }
    }
    else
    {
        ++exponents;
        for (auto &i : fraction)
        {
            ++i;
        }

        while (itR != other.fraction.cbegin())
        {
            align(1);
            uAddTerm(itL, isLPositive, 1 + *std::prev(itR), isRPositive);
            isRPositive = !isRPositive;
            ++itR;
        }
    }
    assert(!fraction.empty() && fraction.back() <= exponents ||
           fraction.empty() && exponents == 0);
    assert(std::is_sorted(fraction.cbegin(), fraction.cend()));
}

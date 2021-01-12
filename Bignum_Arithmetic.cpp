#include "Bignum.h"

void Bignum::uAddTerm(std::list<uint>::iterator &it,
                      bool &isLPositive,
                      uint value,
                      bool isRPositive)
{
    assert(it == fraction.end() || *it > value);
    if (it == fraction.begin())
    {
        assert(isLPositive);
        fraction.push_front(value);
        it          = fraction.begin();
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
                termOpposite(std::prev(it));
            }
        }
        else
        {
            assert(*std::prev(it) != 0);
            it = fraction.insert(it, value);
            if (isLPositive == isRPositive)
            {
                it = termOpposite(it);
            }
            if (it != fraction.begin() &&
                *std::prev(it) == *it - 1)
            {
                fraction.erase(std::prev(it));
                it = fraction.erase(it);
                termOpposite(std::prev(it));
            }
        }
    }
}

void Bignum::uAddAssign(const Bignum &other)
{
    assert(isLegalIgnoreSign());
    assert(other.isLegalIgnoreSign());

    if (!other.fraction.empty())
    {
        if (fraction.empty())
        {
            exponents = other.exponents;
            fraction  = other.fraction;
        }
        else
        {
            bool isLPositive = false;
            bool isRPositive = false;
            auto itL         = fraction.end();
            auto itR         = other.fraction.cend();

            auto align = [&](uint offset) {
                assert(itR != other.fraction.cbegin());
                while (itL != fraction.begin() && *std::prev(itL) > offset + *std::prev(itR))
                {
                    --itL;
                    isLPositive = !isLPositive;
                }
            };

            if (exponents > other.exponents)
            {
                auto order     = exponents - fraction.front() <=> other.exponents + 1;
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
                        termOpposite(fraction.begin());
                        isLPositive = false;
                        order       = 0 <=> order;
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
                auto order     = exponents + 1 <=> other.exponents - other.fraction.front();
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
                        termOpposite(fraction.begin());
                        isLPositive = false;
                        order       = 0 <=> order;
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
                            align(0);
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
                    fraction  = {0};
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
        }

        assert(isLegalIgnoreSign());
        assert(other.isLegalIgnoreSign());
    }
}

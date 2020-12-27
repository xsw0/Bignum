#include "Bignum.h"

Bignum Bignum::uAdd(const Bignum other) const
{
    assert(!fraction.empty() ||
           fraction.empty() &&
               exponents == 0);
    assert(!other.fraction.empty() ||
           other.fraction.empty() &&
               other.exponents == 0);
    if (fraction.empty())
    {
        return other;
    }
    else if (other.fraction.empty())
    {
        return *this;
    }

    Bignum result;
    auto itL = crbegin();
    auto itR = other.crbegin();
    bool isPositive = false;
    if (exponents == other.exponents)
    {
        result.exponents = exponents + 1;
        while (itL != cbegin() || itR != other.cbegin())
        {
            if (auto cmp = (*itL).uCompare(*itR); cmp < 0)
            {
                isPositive = result.uAddTerm(*itL, isPositive);
                ++itL;
            }
            else if (cmp > 0)
            {
                isPositive = result.uAddTerm(*itR, isPositive);
                ++itR;
            }
            else
            {
                if (itL.isPositive == itR.isPositive)
                {
                    isPositive = result.uAddTerm((*itL).toDouble(), isPositive);
                }
                ++itL, ++itR;
            }
        }
        assert(isPositive);
    }
    else
    {
        result.exponents = 1 + std::max(exponents, other.exponents);
        while (itL != crend() && itR != other.crend())
        {
            if (auto cmp = (*itL).uCompare(*itR); cmp < 0)
            {
                isPositive = result.uAddTerm(*itL, isPositive);
                ++itL;
            }
            else if (cmp > 0)
            {
                isPositive = result.uAddTerm(*itR, isPositive);
                ++itR;
            }
            else
            {
                if (itL.isPositive == itR.isPositive)
                {
                    isPositive = result.uAddTerm((*itL).toDouble(), isPositive);
                }
                ++itL, ++itR;
            }
        }
        if ((itL != crend() &&
             itL != cbegin()) ||
            (itR != other.crend() &&
             itR != other.cbegin()))
        {
            bool b = itL != crend() &&
                     itL != cbegin();
            auto &it = b ? itL : itR;
            auto end = b ? cbegin() : other.cbegin();
            while (it != end)
            {
                isPositive = result.uAddTerm(*it, isPositive);
                ++it;
            }
        }
        if (isPositive)
        {
            --result.exponents;
            assert(result.fraction.front() > 0);
            for (auto &i : result.fraction)
            {
                --i;
            }
        }
        else
        {
            result.uAddTerm(Term(result.exponents, false), false);
        }
    }
    return result;
}

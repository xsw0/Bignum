#ifndef __COMPARE_H__
#define __COMPARE_H__

#include <compare>
#include <ostream>

constexpr std::strong_ordering operator<=>(
    decltype(nullptr),
    std::strong_ordering order)
{
    if (order > 0)
    {
        return std::strong_ordering::less;
    }
    else if (order < 0)
    {
        return std::strong_ordering::greater;
    }
    else
    {
        return std::strong_ordering::equal;
    }
}

inline std::ostream &operator<<(std::ostream &os, std::strong_ordering order)
{
    if (order > 0)
    {
        os << '>';
    }
    else if (order < 0)
    {
        os << '<';
    }
    else
    {
        os << '=';
    }
    return os;
}

#endif

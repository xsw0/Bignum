#ifndef __TEST_H__
#define __TEST_H__

#include <random>
#include <array>

namespace TEST
{
    constexpr size_t testCount = 0x20;

    std::default_random_engine &random_engine();

    template <typename T>
    T randomInteger(T lower_bound = std::numeric_limits<T>::min(),
                    T upper_bound = std::numeric_limits<T>::max())
    {
        return std::uniform_int_distribution<T>(
            lower_bound,
            upper_bound)(random_engine());
    }

    template <typename T, size_t SIZE = testCount>
    const std::array<T, SIZE> integerCase = [] {
        std::vector<T> v{
            0, -1, 1,
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max()};
        while (v.size() < SIZE)
        {
            auto r = randomInteger<T>(0, std::numeric_limits<T>::max());
            v.push_back(-r);
            v.push_back(r);
        }
        std::array<T, SIZE> result;
        std::move(v.begin(), v.begin() + SIZE, result.begin());
        return result;
    }();

    template <typename Function, typename Cases>
    void test(Function f, const Cases &cases)
    {
        for (const auto &c : cases)
        {
            f(c);
        }
    }
} // namespace TEST

#endif

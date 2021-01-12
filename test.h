#ifndef __TEST_H__
#define __TEST_H__

#include <random>
#include <iostream>

namespace TEST
{
    constexpr size_t defaultTestCount = 0x20;

    template<size_t Size = defaultTestCount, typename T>
    std::array<T, Size> constant(T value)
    {
        std::array<T, Size> result;
        result.fill(value);
        return result;
    }

    template<size_t Count, size_t Size, typename T>
    std::array<T, Size> repeat(const std::array<T, Size> &arr)
    {
        std::array<T, Size> result;
        for (size_t i = 0; i < Size; ++i)
        {
            result[i] = arr[i % Count];
        }
        return result;
    }

    template<typename T>
    void print(T v)
    {
        std::cout << v << '\n';
    }

    std::default_random_engine &random_engine();

    template<typename Integer>
    Integer randomInteger(Integer lower_bound, Integer upper_bound)
    {
        return std::uniform_int_distribution<Integer>(
            lower_bound,
            upper_bound)(random_engine());
    }

    template<typename Integer>
    Integer randomInteger(Integer upper_bound)
    {
        return randomInteger(0, upper_bound);
    }

    template<typename Integer>
    Integer randomInteger()
    {
        return randomInteger(std::numeric_limits<Integer>::min(), std::numeric_limits<Integer>::max());
    }

    template<typename Integer, size_t Size = defaultTestCount>
    const std::array<Integer, Size> integerCase = []
    {
        std::vector<Integer> v{
            0, -1, 1,
            std::numeric_limits<Integer>::min(),
            std::numeric_limits<Integer>::max()};
        while (v.size() < Size)
        {
            auto r = randomInteger<Integer>(0, std::numeric_limits<Integer>::max());
            v.push_back(-r);
            v.push_back(r);
        }
        std::array<Integer, Size> result;
        std::move(v.begin(), v.begin() + Size, result.begin());
        return result;
    }();

    template<typename T, T Start = 0, size_t Size = defaultTestCount>
    const std::array<T, Size> integerStart = []
    {
        std::array<T, Size> result;
        T start = Start;
        for (auto &n : result)
        {
            n = start;
            ++start;
        }
        return result;
    }();

    template<typename Function, size_t Size, typename... Types>
    void test(Function f, std::array<Types, Size>... args)
    {
        for (size_t i = 0; i < Size; ++i)
        {
            f(args[i]...);
        }
    }

    template<typename Types, size_t Size>
    std::tuple<
        std::array<Types, Size * Size>,
        std::array<Types, Size * Size>>
    x(const std::array<Types, Size> &arg)
    {
        std::tuple<
            std::array<Types, Size * Size>,
            std::array<Types, Size * Size>>
            result;
        for (size_t i = 0; i < Size; ++i)
        {
            for (size_t j = 0; j < Size; ++j)
            {
                std::get<0>(result)[i * Size + j] = arg[i];
                std::get<1>(result)[i * Size + j] = arg[j];
            }
        }
        return result;
    }
} // namespace TEST

#endif

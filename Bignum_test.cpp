#include "Bignum_test.h"

#include <iostream>
#include <iomanip>

namespace BIGNUM_TEST
{
    const std::string UnsignedBinaryString(size_t size)
    {
        if (size == 0)
        {
            return "0";
        }
        std::string result;
        result.reserve(size);
        result.push_back('1');
        while (result.size() < size)
        {
            result.push_back(TEST::randomInteger<char>('0', '1'));
        }
        return result;
    }

    void IOByInt64(int64_t n)
    {
        if (n == std::numeric_limits<int64_t>::min())
        {
            std::cout << std::string(64, '1') << '\n';
        }
        else
        {
            std::cout << (n < 0 ? 1 : 0) << std::bitset<63>(std::abs(n)) << '\n';
        }
        std::cout << std::setw(64) << Bignum(n).to_string() << "\n\n";
    }

    void IOByStr(const std::string &s)
    {
        std::cout << '"' << s << '"' << '\n';
        std::cout << std::setw(s.size() + 1) << Bignum(s).to_string() << "\n\n";
    }

    void testLeftMove(const Bignum &bignum, size_t size)
    {
        std::cout << bignum << ' ' << size << '\n';
        std::cout << (bignum << size) << "\n\n";
    }

    void testRightMove(const Bignum &bignum, size_t size)
    {
        std::cout << bignum << '\n';
        std::cout << (bignum >> size) << ' ' << size << "\n\n";
    }

    void testThree_wayComparison(const Bignum &lhs, const Bignum &rhs)
    {
        auto order = lhs <=> rhs;
        std::cout << lhs << '\n'
                  << order << '\n'
                  << rhs << "\n\n";
    }

    void testuAdd(const Bignum &lhs, const Bignum &rhs)
    {
        std::cout << std::setw(80) << lhs.uTo_string() << '\n'
                  << std::setw(80) << rhs.uTo_string() << '\n'
                  << std::setw(80) << lhs.uAdd(rhs).uTo_string() << "\n\n";
    }
} // namespace BIGNUM_TEST

#ifndef __BIGNUM_TEST_H__
#define __BIGNUM_TEST_H__

#include "Bignum.h"
#include "test.h"
#include <utility>

namespace BIGNUM_TEST
{
    const std::string UnsignedBinaryString(size_t Size = 0);

    template <size_t Size = TEST::defaultTestCount>
    std::array<std::string, Size> binaryStringCase = [] {
        std::vector<std::string> v{"0", "-0", "+0"};
        while (v.size() < Size)
        {
            std::string s = UnsignedBinaryString(v.size() / 3);
            v.push_back(s);
            v.push_back("-" + s);
            v.push_back("+" + s);
        }
        std::array<std::string, Size> result;
        std::move(v.begin(), v.begin() + Size, result.begin());
        return result;
    }();

    template <size_t Size = TEST::defaultTestCount>
    std::array<Bignum, Size> BignumCase = [] {
        std::array<Bignum, Size> result;
        for (size_t i = 0; i < Size; ++i)
        {
            result[i] = binaryStringCase<Size>[i];
        }
        return result;
    }();

    void IOByInt64(std::int64_t n);
    void IOByStr(const std::string &s);

    void testLeftMove(const Bignum &bignum, size_t size);
    void testRightMove(const Bignum &bignum, size_t size);

    void testThree_wayComparison(const Bignum &lhs, const Bignum &rhs);
} // namespace BIGNUM_TEST

#endif

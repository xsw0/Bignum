#ifndef __BIGNUM_TEST_H__
#define __BIGNUM_TEST_H__

#include "Bignum.h"
#include "test.h"
#include <utility>

namespace BIGNUM_TEST
{
    const std::string UnsignedBinaryString(size_t SIZE = 0);

    template <size_t SIZE = TEST::testCount>
    std::array<std::string, SIZE> binaryStringCase = [] {
        std::vector<std::string> v{"0", "-0", "+0"};
        while (v.size() < SIZE)
        {
            std::string s = UnsignedBinaryString(v.size() / 3);
            v.push_back(s);
            v.push_back("-" + s);
            v.push_back("+" + s);
        }
        std::array<std::string, SIZE> result;
        std::move(v.begin(), v.begin() + SIZE, result.begin());
        return result;
    }();

    template <size_t SIZE = TEST::testCount>
    std::array<Bignum, SIZE> BignumCase = [] {
        std::array<Bignum, SIZE> result;
        for (size_t i = 0; i < SIZE; ++i)
        {
            result[i] = Bignum::random(i, i);
        }
        return result;
    }();

    template <size_t MAX = TEST::testCount,
              size_t SIZE = TEST::testCount>
    std::array<std::pair<Bignum, size_t>, SIZE> moveCase = [] {
        std::vector<std::pair<Bignum, size_t>> v;
        auto bignum = BignumCase<MAX>;

        size_t i = 0;
        while (v.size() < SIZE)
        {
            for (size_t j = 0; j < bignum.size(); j++)
            {
                v.push_back(std::pair<Bignum, size_t>{bignum[j], i});
            }
            ++i;
        }
        std::array<std::pair<Bignum, size_t>, SIZE> result;
        std::move(v.begin(), v.begin() + SIZE, result.begin());
        return result;
    }();

    void IOByInt64(std::int64_t n);
    void IOByStr(const std::string &s);

    void testLeftMove(const std::pair<Bignum, size_t> &p);
    void testRightMove(const std::pair<Bignum, size_t> &p);
} // namespace BIGNUM_TEST

#endif

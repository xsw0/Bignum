#include <iostream>
#include <random>
#include <limits>
#include <bitset>
#include <vector>
#include <iomanip>
#include <numeric>
#include <limits>

#include "test.h"
#include "Bignum.h"
#include "Bignum_test.h"

using namespace std;

float f(int a, double b)
{
    return a - b;
}

int main(int argc, char const *argv[])
{
    // TEST::test(
    //     TEST::print<Bignum>,
    //     BIGNUM_TEST::BignumCase<>);
    // TEST::test(
    //     BIGNUM_TEST::IOByInt64,
    //     TEST::integerCase<int64_t>);
    // TEST::test(
    //     BIGNUM_TEST::IOByStr,
    //     BIGNUM_TEST::binaryStringCase<>);
    // TEST::test(
    //     BIGNUM_TEST::testLeftMove,
    //     BIGNUM_TEST::BignumCase<>,
    //     TEST::repeat<8>(TEST::integerStart<size_t>));
    // TEST::test(
    //     BIGNUM_TEST::testRightMove,
    //     BIGNUM_TEST::BignumCase<>,
    //     TEST::repeat<8>(TEST::integerStart<size_t>));
    // auto t = TEST::x(BIGNUM_TEST::BignumCase<8>);
    // TEST::test(
    //     BIGNUM_TEST::testThree_wayComparison,
    //     std::get<0>(t),
    //     std::get<1>(t));
    for (size_t i = 0; i < 100; i++)
    {
        for (size_t j = i; j < 100; j++)
        {
            BIGNUM_TEST::testuAdd(Bignum(i), Bignum(j));
        }
    }

    return 0;
}

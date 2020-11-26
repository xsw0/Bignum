#include <iostream>
#include <random>
#include <limits>
#include <bitset>
#include <vector>
#include <iomanip>

#include "test.h"
#include "Bignum_test.h"

using namespace std;

int main(int argc, char const *argv[])
{
    // TEST::test(
    //     BIGNUM_TEST::IOByStr,
    //     BIGNUM_TEST::binaryStringCase<>);
    // TEST::test(
    //     BIGNUM_TEST::testLeftMove,
    //     BIGNUM_TEST::moveCase<10, 100>);
    TEST::test(
        BIGNUM_TEST::testRightMove,
        BIGNUM_TEST::moveCase<10, 100>);
    // string s;
    // size_t n;
    // while (cin >> s >> n)
    // {
    //     BIGNUM_TEST::testRightMove({s, n});
    // }

    // for (size_t i = 0; i < 100; i++)
    // {
    //     cout << i << '\n';
    //     cout << Bignum::random(i, i).to_string() << '\n';
    // }

    return 0;
}

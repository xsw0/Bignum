#include <iostream>
#include <random>
#include <limits>
#include <bitset>
#include <vector>

#include "Bignum.h"

using namespace std;

static std::default_random_engine random_engine;
template <typename T = int8_t>
T randomInteger()
{
    static std::uniform_int_distribution<T>
        uniform_dist(numeric_limits<T>::min(),
                     numeric_limits<T>::max());
    return uniform_dist(random_engine);
}

const vector<int8_t> testcaseInteger = [] {
    vector<int8_t> testcase =
        {0, 1, -1, 127, -128,
         0b10,
         0b11111,
         0b111000,
         0b101010};
    while (testcase.size() < 100)
    {
        testcase.push_back(randomInteger());
    }
    return testcase;
}();

// const vector<Bignum> testcaseBignum = [] {
//     vector<Bignum> testcase;
//     testcase.reserve(
//         testcase.size() + testcaseInteger.size());
//     for (auto i : testcaseInteger)
//     {
//         testcase.emplace_back(i);
//     }
//     return testcase;
// }();

void testBignumIO(int8_t n)
{
    cout << bitset<8>(n) << '\n'
         << (n < 0 ? 1 : 0) << bitset<7>(abs(n)) << '\n'
         << Bignum(n).to_string() << "\n\n";
}

int main(int argc, char const *argv[])
{
    // for (auto n : testcaseInteger)
    // {
    //     testBignumIO(n);
    // }
    int n;
    while (cin >> n)
    {
        testBignumIO(n);
    }

    return 0;
}

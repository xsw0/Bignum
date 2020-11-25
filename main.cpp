#include <iostream>
#include <random>
#include <limits>
#include <bitset>
#include <vector>
#include <iomanip>

#include "Bignum.h"

using namespace std;

static std::default_random_engine random_engine;
template <typename T = int64_t>
T randomInteger()
{
    static std::uniform_int_distribution<T>
        uniform_dist(numeric_limits<T>::min(),
                     numeric_limits<T>::max());
    return uniform_dist(random_engine);
}

const vector<int64_t> testcaseInteger = [] {
    vector<int64_t> testcase =
        {0, 1, -1};
    testcase.push_back(numeric_limits<int64_t>::min());
    testcase.push_back(numeric_limits<int64_t>::max());
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

template <typename T = int64_t>
void testBignumIO(T n)
{
    // cout << ' ' << bitset<64>(n) << '\n';
    if (n == numeric_limits<int64_t>::min())
    {
        cout << string(64, '1') << '\n';
    }
    else
    {
        cout << (n < 0 ? 1 : 0) << bitset<63>(abs(n)) << '\n';
    }
    cout << setw(64) << Bignum(n).to_string() << "\n\n";
}

int main(int argc, char const *argv[])
{
    for (auto n : testcaseInteger)
    {
        testBignumIO(n);
    }
    // int n;
    // while (cin >> n)
    // {
    //     testBignumIO(n);
    // }

    return 0;
}

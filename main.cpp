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
T randomInteger(T lower_bound = numeric_limits<T>::min(), T upper_bound = numeric_limits<T>::max())
{
    return std::uniform_int_distribution<T>(
        lower_bound,
        upper_bound)(random_engine);
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

std::string random_bin_str()
{
    string s;
    switch (randomInteger(0, 2))
    {
    case 1:
        s = "-1";
        break;
    case 2:
        s = "+1";
        break;
    default:
        s = "1";
        break;
    }

    for (size_t i = 0, size = randomInteger<size_t>(0, 80); i < size; i++)
    {
        s.push_back('0' + randomInteger(0, 1));
    }
    return s;
}

vector<std::string> testcaseBinStr()
{
    vector<std::string> result{
        "0", "+0", "-0", "1", "+1", "-1"};
    while (result.size() < 100)
    {
        result.push_back(random_bin_str());
    }
    return result;
}

void testStringIO(const string &s)
{
    cout << '"' << s << '"' << '\n';
    cout << setw(s.size() + 1) << Bignum(s).to_string() << "\n\n";
}

void testLeftMove(const Bignum &bignum, size_t n)
{
    cout << bignum.to_string() << ' ' << n << '\n';
    cout << (bignum << n).to_string() << "\n\n";
}

void testRightMove(const Bignum &bignum, size_t n)
{
    string s = bignum.to_string();
    cout << s << '\n';
    cout << (bignum >> n).to_string() << ' ' << n << "\n\n";
}

int main(int argc, char const *argv[])
{
    for (size_t i = 0; i < 100; i++)
    {
        testRightMove(Bignum::random(0, 20), randomInteger(0, 10));
    }

    // string s;
    // while (cin >> s)
    // {
    //     testStringIO(s);
    // }

    return 0;
}

#include "Bignum.h"

// void Bignum::uAddAssign(const Bignum other)
// {
//     auto itR = other._v.crbegin();
//     bool bitR = false;
//     value_type countR = other.exp;

//     auto itL = _v.end();
//     bool bitL = false;
//     while (true)
//     {
//         value_type countL = 0;
//         while (countL > 0)
//         {
//             if (countL == countR)
//             {
//                 if (bitR)
//                 {
//                     if (bitL)
//                     {
//                         *prev(itL) += countL;
//                         *itL -= countL;
//                     }
//                     else
//                     {
//                         /* code */
//                     }
//                 }
//             }
//         }
//     }
// }

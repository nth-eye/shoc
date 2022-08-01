#ifndef SHOC_ELLIPTIC_H
#define SHOC_ELLIPTIC_H

#include "shoc/util.h"

namespace shoc {

constexpr int ff_mod(int x, int m)
{
    return (x % m + m) % m;
}

constexpr int ff_add(int x, int y, int m)
{
    return ff_mod(x + y, m);
}

constexpr int ff_sub(int x, int y, int m)
{
    return ff_mod(x - y, m);
}

constexpr int ff_mul(int x, int y, int m)
{
    return ff_mod(x * y, m);
}

constexpr int ff_pow(int b, int e, int m)
{
    int r = 1;
    e = ff_mod(e, m - 1);
    while (e) {
        if (e & 1)
            r = ff_mul(r, b, m);
        b = ff_mul(b, b, m);
        e >>= 1;
    }
    return r;
}

constexpr int ff_inv(int x, int m)
{
    return ff_pow(x, m - 2, m);
}

constexpr int ff_div(int x, int y, int m)
{
    return ff_mul(x, ff_inv(y, m), m);
}

constexpr bool elliptic_curve(int x, int y, int a, int b)
{
    return y * y == x * x * x + a * x + b;
}

constexpr bool secp256k1(int x, int y)
{
    return elliptic_curve(x, y, 0, 7);
}

// template<unsigned P>
// struct fint {

//     using type = fint<P>;

//     fint() = default;
//     fint(unsigned val) : val{val} {}

//     operator unsigned() const { return val; }
//     operator int() const { return val; }

//     bool operator<=>(const type &other) const = default;

//     type& operator+=(const type &rhs)  
//     {
//         val = (val + rhs.val) % P;
//         return *this; 
//     }
//     friend type operator+(type lhs, const type &rhs)
//     {
//         lhs += rhs;
//         return lhs;
//     }
//     type& operator-=(const type &rhs)  
//     {
//         val = (val - rhs.val) % P;
//         return *this; 
//     }
//     friend type operator-(type lhs, const type &rhs)
//     {
//         lhs -= rhs;
//         return lhs;
//     }
//     type& operator*=(const type &rhs)  
//     {
//         val = (val * rhs.val) % P;
//         return *this; 
//     }
//     friend type operator*(type lhs, const type &rhs)
//     {
//         lhs *= rhs;
//         return lhs;
//     }
//     type& operator/=(const type &rhs)  
//     {
//         val = (val * mpow(rhs.val, P - 2, P)) % P;
//         return *this; 
//     }
//     friend type operator/(type lhs, const type &rhs)
//     {
//         lhs /= rhs;
//         return lhs;
//     }

//     // friend type pow(type lhs, int exp)
//     // {
//     //     return mpow(lhs.val, exp % (P - 1) , P);
//     // }
// private:
//     unsigned val;
// };

}

#endif
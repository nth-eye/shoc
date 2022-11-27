#ifndef SHOC_FINITE_FIELD_H
#define SHOC_FINITE_FIELD_H

#include "shoc/util.h"
#include <type_traits>

namespace shoc {

template<std::integral T>
constexpr T ff_mod(T x, T m)
{
    return (x % m + m) % m;
}

template<std::integral T>
constexpr T ff_add(T x, T y, T m)
{
    return ff_mod(x + y, m);
}

template<std::integral T>
constexpr T ff_sub(T x, T y, T m)
{
    return ff_mod(x - y, m);
}

template<std::integral T>
constexpr T ff_mul(T x, T y, T m)
{
    return ff_mod(x * y, m);
}

template<std::integral T>
constexpr T ff_pow(T b, T e, T m)
{
    T r = 1;
    e = ff_mod(e, m - 1);
    while (e) {
        if (e & 1)
            r = ff_mul(r, b, m);
        b = ff_mul(b, b, m);
        e >>= 1;
    }
    return r;
}

template<std::integral T>
constexpr T ff_inv(T x, T m)
{
    return ff_pow(x, m - 2, m);
}

template<std::integral T>
constexpr T ff_div(T x, T y, T m)
{
    return ff_mul(x, ff_inv(y, m), m);
}

template<std::integral T, T P>
struct ff_int {

    using value_type = T;

    constexpr ff_int() = default;
    constexpr ff_int(value_type val) : val{val}             {}
    constexpr operator value_type() const                   { return val; }
    friend constexpr bool operator==(ff_int x, ff_int y)    { return x.val == y.val; }
    friend constexpr bool operator!=(ff_int x, ff_int y)    { return x.val != y.val; }
    friend constexpr ff_int operator+(ff_int x, ff_int y)   { return ff_mod(x.val + y.val, P); }
    friend constexpr ff_int operator-(ff_int x, ff_int y)   { return ff_mod(x.val - y.val, P); }
    friend constexpr ff_int operator*(ff_int x, ff_int y)   { return ff_mod(x.val * y.val, P); }
    friend constexpr ff_int operator/(ff_int x, ff_int y)   { return x * inv(y); }
    friend constexpr ff_int inv(ff_int x)                   { return pow(x, P - 2); }
    friend constexpr ff_int pow(ff_int b, value_type e)
    {
        ff_int r = 1;
        e = ff_mod(e, P - 1);
        while (e) {
            if (e & 1)
                r = r * b;
            b = b * b;
            e >>= 1;
        }
        return r;
    }
// private: NOTE: private not possible because of literal class limitations in templates 
    value_type val = 0;
};

/**
 * @brief Concept for finite field integers.
 * 
 * @tparam T Class to test
 */
template<class T>
concept finite_field = requires(T x) { 
    { ff_int{x} } -> std::same_as<T>;
};

}

#endif
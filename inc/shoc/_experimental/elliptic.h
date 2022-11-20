#ifndef SHOC_ELLIPTIC_H
#define SHOC_ELLIPTIC_H

#include "shoc/util.h"
#include <type_traits>

namespace shoc {

template<std::integral T>
constexpr T ff_mod(T x, T m)
{
    return (x % m + m) % m;
}

// template<std::integral T>
// constexpr T ff_add(T x, T y, T m)
// {
//     return ff_mod(x + y, m);
// }

// template<std::integral T>
// constexpr T ff_sub(T x, T y, T m)
// {
//     return ff_mod(x - y, m);
// }

// template<std::integral T>
// constexpr T ff_mul(T x, T y, T m)
// {
//     return ff_mod(x * y, m);
// }

// template<std::integral T>
// constexpr T ff_pow(T b, T e, T m)
// {
//     T r = 1;
//     e = ff_mod(e, m - 1);
//     while (e) {
//         if (e & 1)
//             r = ff_mul(r, b, m);
//         b = ff_mul(b, b, m);
//         e >>= 1;
//     }
//     return r;
// }

// template<std::integral T>
// constexpr T ff_inv(T x, T m)
// {
//     return ff_pow(x, m - 2, m);
// }

// template<std::integral T>
// constexpr T ff_div(T x, T y, T m)
// {
//     return ff_mul(x, ff_inv(y, m), m);
// }



// template<std::integral T>
// constexpr bool elliptic_curve(T x, T y, T a, T b)
// {
//     return y * y == x * x * x + a * x + b;
// }

// template<std::integral T>
// constexpr bool secp256k1(T x, T y)
// {
//     return elliptic_curve(x, y, T(0), T(7));
// }

// template<std::integral T, T P>
// constexpr bool ff_elliptic_curve(T x, T y, T a, T b)
// {
//     return ff_mul(y, y, P) == ff_add(ff_add(ff_pow(x, 3, P), ff_mul(a, x, P), P), b, P);
// }



template<std::integral T, T P>
struct ff_int {
    constexpr ff_int() = default;
    constexpr ff_int(T val) : val{val} {}
public:
    friend constexpr bool operator==(ff_int x, ff_int y)
    {
        return x.val == y.val;
    }
    friend constexpr bool operator!=(ff_int x, ff_int y)
    {
        return x.val != y.val;
    }
    friend constexpr ff_int operator+(ff_int x, ff_int y)
    {
        return ff_mod(x.val + y.val, P);
    }
    friend constexpr ff_int operator-(ff_int x, ff_int y)
    {
        return ff_mod(x.val - y.val, P);
    }
    friend constexpr ff_int operator*(ff_int x, ff_int y)
    {
        return ff_mod(x.val * y.val, P);
    }
    friend constexpr ff_int pow(ff_int b, T e)
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
    friend constexpr ff_int inv(ff_int x)
    {
        return pow(x, P - 2);
    }
    friend constexpr ff_int operator/(ff_int x, ff_int y)
    {
        return x * inv(y);
    }
// private:
    T val = 0;
};

template<class T>
concept finite = requires(T x) { 
    { ff_int{x} } -> std::same_as<T>;
};

template<finite T>
constexpr bool ff_elliptic_curve(T x, T y, T a, T b)
{
    return y * y == x * x * x + a * x + b;
}

template<finite T, T A, T B>
struct ff_point {
    constexpr ff_point() = default;
    constexpr ff_point(T x, T y) : x{x}, y{y}, inf{false}
    {
        assert(ff_elliptic_curve(x, y, A, B));
    }
public:
    friend constexpr ff_point operator+(const ff_point& lhs, const ff_point& rhs)
    {
        if (lhs.is_infinity())
            return rhs;
        if (rhs.is_infinity())
            return lhs;
        if (lhs.x == rhs.x) {
            if (lhs.y != rhs.y || lhs.y == 0)
                return {};
            auto s = (3 * lhs.x * lhs.x + A) / (2 * lhs.y);
            auto x = s * s - 2 * lhs.x;
            auto y = s * (lhs.x - x) - lhs.y;
            return {x, y};
        }
        auto s = (rhs.y - lhs.y) / (rhs.x - lhs.x);
        auto x = s * s - lhs.x - rhs.x;
        auto y = s * (lhs.x - x) - lhs.y;
        return {x, y};
    }
    constexpr bool is_infinity() const
    {
        return inf;
    }
private:
    T x = 0;
    T y = 0;
    bool inf = true;
};

template<finite T, T A, T B>
inline constexpr auto ff_infinity = ff_point<T, A, B>{};


// template<std::integral T, T P>
// constexpr bool ff_elliptic_curve(ff_int<T, P> x, ff_int<T, P> y, ff_int<T, P> a, ff_int<T, P> b)
// {
//     return y * y == x * x * x + a * x + b;
// }


// template<std::integral T, T A, T B, T P>
// struct ff_point {

//     ff_point() = default;
//     ff_point(T x, T y) : x{x}, y{y} 
//     {
//         assert(ff_elliptic_curve<P>(x, y, A, B));
//     }
//     friend constexpr ff_point operator+(const ff_point& lhs, const ff_point& rhs)
//     {
//         if (lhs.is_infinity())
//             return rhs;
//         if (rhs.is_infinity())
//             return lhs;
//         if (lhs.x == rhs.x) {
//             if (lhs.y != rhs.y || lhs.y == 0)
//                 return {};
//             auto s = (3 * lhs.x * lhs.x + A) / (2 * lhs.y);
//             auto x = s * s - 2 * lhs.x;
//             auto y = s * (lhs.x - x) - lhs.y;
//             return {x, y};
//         }
//         auto s = (rhs.y - lhs.y) / (rhs.x - lhs.x);
//         auto x = s * s - lhs.x - rhs.x;
//         auto y = s * (lhs.x - x) - lhs.y;
//         return {x, y};
//     }
//     constexpr bool is_infinity() const
//     {
//         return inf;
//     }
// private:
//     T x = 0;
//     T y = 0;
//     bool inf = true;
// };

// template<std::integral T, T A, T B, T P>
// inline constexpr auto ff_infinity = ff_point<T, A, B, P>{};




// template<std::integral T, T a, T b>
// struct ff_point_inf {};

// template<std::integral T, T a, T b>
// inline constexpr auto ff_infinity = ff_point_inf<T, a, b>;

// template<std::integral T, T a, T b>
// struct ff_point {

//     friend constexpr auto operator+(ff_point_inf<T, a, b>, ff_point rhs)
//     {
//         return rhs;
//     }
//     friend constexpr auto operator+(ff_point lhs, ff_point_inf<T, a, b>)
//     {
//         return lhs;
//     }
//     friend constexpr auto operator+(ff_point lhs, const ff_point& rhs)
//     {

//         lhs += 
//         return lhs;
//     }
//     T x;
//     T y;
// };

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
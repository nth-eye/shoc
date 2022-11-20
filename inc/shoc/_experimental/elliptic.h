#ifndef SHOC_ELLIPTIC_H
#define SHOC_ELLIPTIC_H

#include "shoc/_experimental/finite_field.h"

namespace shoc {

template<finite_field T>
constexpr bool ff_elliptic_curve(T x, T y, T a, T b)
{
    return y * y == x * x * x + a * x + b;
}

template<finite_field T, T A, T B>
struct ff_point {
    constexpr ff_point() = default;
    constexpr ff_point(T x, T y) : x{x}, y{y}, inf{false}
    {
        assert(ff_elliptic_curve(x, y, A, B));
    }
    constexpr bool is_infinity() const
    {
        return inf;
    }
    constexpr auto get_x() const
    {
        return x.val;
    }
    constexpr auto get_y() const
    {
        return y.val;
    }
    friend constexpr ff_point operator+(const ff_point& lhs, const ff_point& rhs)
    {
        if (lhs.is_infinity())
            return rhs;
        if (rhs.is_infinity())
            return lhs;
        if (lhs.x == rhs.x) {
            if (lhs.y != rhs.y || lhs.y == T{0})
                return {};
            auto s = (T{3} * lhs.x * lhs.x + A) / (T{2} * lhs.y);
            auto x = s * s - T{2} * lhs.x;
            auto y = s * (lhs.x - x) - lhs.y;
            return {x, y};
        }
        auto s = (rhs.y - lhs.y) / (rhs.x - lhs.x);
        auto x = s * s - lhs.x - rhs.x;
        auto y = s * (lhs.x - x) - lhs.y;
        return ff_point{x, y};
    }
    friend constexpr ff_point operator*(const ff_point& lhs, T::value_type rhs)
    {
        return mul(lhs, rhs);
    }
    friend constexpr ff_point operator*(T::value_type lhs, const ff_point& rhs)
    {
        return mul(rhs, lhs);
    }
private:
    static constexpr ff_point mul(ff_point point, T::value_type coeff)
    {
        ff_point result = {};
        while (coeff) {
            if (coeff & 1)
                result = result + point;
            point = point + point;
            coeff >>= 1;
        }
        return result;
    }
private:
    T x = 0;
    T y = 0;
    bool inf = true;
};

template<finite_field T, T A, T B>
inline constexpr auto ff_infinity = ff_point<T, A, B>{};

}

#endif

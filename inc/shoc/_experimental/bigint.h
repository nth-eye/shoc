#ifndef SHOC_BIGINT_H
#define SHOC_BIGINT_H

#include "shoc/util.h"
#include <limits>

namespace shoc {

template<std::unsigned_integral T>
constexpr bool overflow_sum(T a, T b, bool carry)
{
    return b + carry && a > std::numeric_limits<T>::max() - b - carry;
}

template<std::unsigned_integral T>
constexpr bool overflow_sub(T a, T b, bool borrow)
{
    return b + borrow && a < std::numeric_limits<T>::min() + b + borrow;
}

template<std::unsigned_integral T>
constexpr bool overflow_mul(T a, T b)
{
    return b && a > std::numeric_limits<T>::max() / b;
}

template<std::unsigned_integral T, size_t N> 
struct bigint {

    // ANCHOR Member types

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using universal_reference = value_type&&;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ANCHOR Constructors

    constexpr bigint() noexcept = default;
    constexpr bigint(value_type x) noexcept : buf{x} 
    {
        
    }
    constexpr explicit bigint(value_type x, size_type n) noexcept
    {
        std::fill_n(buf, std::min(words, n), x);
    }
    template<size_type Size>
    constexpr bigint(value_type(&&array)[Size]) noexcept
    {
        static_assert(capacity() >= Size);
        std::copy_n(array, Size, buf);
    }
    // template<size_type Size>
    // constexpr bigint(const char(&str)[Size]) noexcept
    // {
        // size_type dst = 0;
        // size_type src = Size - 1;

        // bool carry = false;

        // while (dst < capacity()) {
        //     int num = str[--src] - '0';
        //     if (num < 0 || num > 10) {
        //         std::fill_n(buf, capacity(), value_type{0});
        //         break;
        //     }
        //     buf[dst] += num + carry;
        //     carry = overflow_sum(buf[dst], value_type(num), carry);
        //     if (!src)
        //         break;
        //     if (carry)
        //         ++dst;
        // }
    // }

    // ANCHOR Capacity

    constexpr static size_type capacity()       { return words; }
    constexpr static size_type max_size()       { return words; }

    // ANCHOR Iterators

    constexpr iterator begin() noexcept                 { return buf; }
    constexpr const_iterator begin() const noexcept     { return buf; }
    constexpr const_iterator cbegin() const noexcept    { return begin(); }
    constexpr iterator end() noexcept                   { return buf + words; }
    constexpr const_iterator end() const noexcept       { return buf + words; }
    constexpr const_iterator cend() const noexcept      { return end(); }

    constexpr reverse_iterator rbegin() noexcept                { return std::reverse_iterator(buf + words - 1); }
    constexpr const_reverse_iterator rbegin() const noexcept    { return std::reverse_iterator(buf + words - 1); }
    constexpr const_reverse_iterator crbegin() const noexcept   { return rbegin(); }
    constexpr reverse_iterator rend() noexcept                  { return std::reverse_iterator(buf - 1); }
    constexpr const_reverse_iterator rend() const noexcept      { return std::reverse_iterator(buf - 1); }
    constexpr const_reverse_iterator crend() const noexcept     { return rend(); }

    // ANCHOR Access

    constexpr reference operator[](size_type i)             { assert(capacity() > i); return buf[i]; }
    constexpr const_reference operator[](size_type i) const { assert(capacity() > i); return buf[i]; }
    constexpr reference front()                             { return buf[0]; }
    constexpr const_reference front() const                 { return buf[0]; }
    constexpr reference back()                              { return buf[words - 1]; }
    constexpr const_reference back() const                  { return buf[words - 1]; }
    constexpr pointer data() noexcept                       { return buf; }
    constexpr const_pointer data() const noexcept           { return buf; }

    // ANCHOR Modifiers

    constexpr bool operator==(const bigint& rhs) const
    {
        for (size_type i = 0; i < capacity(); ++i) {
            if (buf[i] != rhs[i])
                return false;
        }
        return true; 
    }

    constexpr bool operator!=(const bigint& rhs) const
    {
        return !operator==(rhs);
    }

    constexpr bigint operator+(const bigint& rhs) const
    {
        bigint res;
        bool carry = false;
        for (size_type i = 0; i < capacity() - 1; ++i) {
            res[i] = buf[i] + rhs[i] + carry;
            carry = overflow_sum(buf[i], rhs[i], carry);
        }
        res.back() = back() + rhs.back() + carry;
        return res;
    }

    constexpr bigint operator-(const bigint& rhs) const
    {
        bigint res;
        bool borrow = false;
        for (size_type i = 0; i < capacity() - 1; ++i) {
            res[i] = buf[i] - rhs[i] - borrow;
            borrow = overflow_sub(buf[i], rhs[i], borrow);
        }
        res.back() = back() - rhs.back() - borrow;
        return res;
    }

    constexpr bigint operator*(const bigint& rhs) const
    {
        bigint res;


    }

    // ANCHOR Print

    void print() const
    {
        printf("0x");
        for (size_type i = capacity(); i;)
            printf("%0*lx'", padding, buf[--i]);
        printf("\b \n");
    }
private:
    static constexpr auto words = utl::words_in_bits<T>(N);
    static constexpr auto padding = int(sizeof(value_type) * 2);
private:
    T buf[words]{};
};

// friend std::ostream& operator<<(std::ostream& os, const bigint& value)
// {
//     os << 

//     for (size_t i = words; i; --i) {

//     }
// }

}

#endif
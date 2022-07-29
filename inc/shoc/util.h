#ifndef SHOC_UTIL_H
#define SHOC_UTIL_H

#include <cassert>
#include <bit>
#include <algorithm>
#include "utl/bit.h"
#include "utl/str.h"

namespace shoc {

using byte = uint8_t;

/**
 * @brief Wrapper for swap function.
 * 
 * @param x
 * @param y
 */
constexpr void swap(auto &x, auto &y)
{ 
    std::swap(x, y); 
}

/**
 * @brief Rotate bits of an integer to left.
 * 
 * @param x Integer to rotate
 * @param s Shift
 * @return Result
 */
constexpr auto rol(auto x, int s)
{ 
    return std::rotl(x, s); 
}

/**
 * @brief Rotate bits of an integer to right.
 * 
 * @param x Integer to rotate
 * @param s Shift
 * @return Result 
 */
constexpr auto ror(auto x, int s)
{ 
    return std::rotr(x, s); 
}

/**
 * @brief Check if platform little endian.
 * 
 * @return true if little endian
 */
constexpr bool little_endian()
{ 
    return std::endian::native == std::endian::little; 
}

/**
 * @brief Copy from one memory region to another.
 * 
 * @param dst Destination
 * @param src Source
 * @param count Number of bytes
 */
constexpr void copy(void *dst, const void *src, size_t count)
{
    std::copy(static_cast<const byte*>(src), static_cast<const byte*>(src) + count, static_cast<byte*>(dst));
}

/**
 * @brief Fill memory with given byte value.
 * 
 * @param dst Memory to fill
 * @param val Byte value
 * @param count Number of bytes
 */
constexpr void fill(void *dst, byte val, size_t count)
{
    std::fill_n(static_cast<byte*>(dst), count, val);
}

/**
 * @brief Reliably zero out memory region.
 * 
 * @param dst Memory to zero out
 * @param count Number of bytes
 */
constexpr void zero(void *dst, size_t count)
{
    std::fill_n(static_cast<volatile byte*>(dst), count, 0);
}

/**
 * @brief XOR block of bytes with another. Arrays must be of equal 
 * length and valid pointers!
 * 
 * @param x Destination array
 * @param y Another array
 * @param len Length of a block in bytes, default if 16
 */
constexpr void xorb(byte *x, const byte *y, size_t len = 16)
{
    for (size_t i = 0; i < len; ++i)
        x[i] ^= y[i];
}

/**
 * @brief Reverse bits in the given integer.
 * 
 * @tparam T Integer type
 * @param x Integer to reverse
 * @return Result
 */
template<class T>
constexpr T bitswap(T x)
{
    auto bits = sizeof(T) * 8; 
    auto mask = ~T(0);
    while (bits >>= 1) {
        mask ^= mask << bits;
        x = (x & ~mask) >> bits | (x & mask) << bits;
    }
    return x;
}

/**
 * @brief Reverse bytes in the given integer.
 * 
 * @tparam T Integer type
 * @param x Integer to reverse
 * @return Result
 */
template<class T>
constexpr T byteswap(T x)
{
    // TODO
}

/**
 * @brief Put integer into array in little endian order.
 * 
 * @tparam T Integer type
 * @param val Input integer
 * @param out Output array
 */
template<class T>
constexpr void putle(T val, byte *out)
{
    for (int i = 0; i < sizeof(T) * 8; i += 8)
        *out++ = val >> i;
}

/**
 * @brief Put integer into array in big endian order.
 * 
 * @tparam T Integer type
 * @param val Input integer
 * @param out Output array
 */
template<class T>
constexpr void putbe(T val, byte *out)
{
    for (int i = sizeof(T) * 8 - 8; i >= 0; i -= 8)
        *out++ = val >> i;
}

/**
 * @brief Choose function, used in SHA and MD.
 */
template <class T>
constexpr T ch(T x, T y, T z)
{ 
    return (x & y) ^ (~x & z); 
}

/**
 * @brief Major function, used in SHA and MD.
 */
template <class T>
constexpr T maj(T x, T y, T z)
{ 
    return (x & y) ^ (x & z) ^ (y & z); 
}

/**
 * @brief Parity function, used in SHA and MD.
 */
template <class T>
constexpr T parity(T x, T y, T z)
{ 
    return x ^ y ^ z; 
}

/**
 * @brief Increment counter bytes in a block, used in block-cipher mode
 * such as CTR and GCM.
 * 
 * @tparam L Length of counter in bytes, default is 4
 * @tparam B Total block length in bytes, default is 16 
 * @param block Pointer to the beginning of a block, not counter
 */
template<size_t L = 4, size_t B = 16>
constexpr void incc(byte *block)
{
    size_t i = B;
    while (++block[--i] == 0 && i >= B - L);
}

template<class H>
struct Eater {
    void operator()(const void *in, size_t len, byte *out)
    {
        auto &impl = static_cast<H&>(*this);
        impl.init();
        impl.feed(in, len);
        impl.stop(out);
    }
private:
    friend H;
};

}

#endif
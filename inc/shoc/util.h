#ifndef SHOC_UTIL_H
#define SHOC_UTIL_H

#include "utl/bit.h"
#include "utl/str.h"
#include "utl/log.h"

namespace shoc {

using std::swap;
using utl::countof;

/**
 * @brief Alias for underlying byte type.
 * 
 */
using byte = uint8_t;

/**
 * @brief Input (const) span of bytes.
 * 
 * @tparam N Size
 */
template<size_t N = std::dynamic_extent>
struct span_i : std::span<const byte, N> {
    using base = std::span<const byte, N>;
    using base::base;
    span_i(const void* str, size_t len) : base{reinterpret_cast<const byte*>(str), len} {}
    span_i(std::string_view str) : base{reinterpret_cast<const byte*>(str.data()), str.size()} {}
};

/**
 * @brief Output (non-const) span of bytes.
 * 
 * @tparam N Size
 */
template<size_t N = std::dynamic_extent>
using span_o = std::span<byte, N>;

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
 * @param dst Pointer to destination objects
 * @param src Pointer to source objects
 * @param cnt Number of objects
 */
template<class T>
constexpr void copy(T* dst, const T* src, size_t cnt)
{
    std::copy(src, src + cnt, dst);
}

/**
 * @brief Fill memory with given byte value.
 * 
 * @param dst Memory to fill
 * @param val Byte value
 * @param cnt Number of bytes
 */
constexpr void fill(void* dst, byte val, size_t cnt)
{
    std::fill_n(static_cast<byte*>(dst), cnt, val);
}

constexpr void fill(byte* dst, byte val, size_t cnt)
{
    std::fill_n(dst, cnt, val);
}

/**
 * @brief Reliably zero out memory region.
 * 
 * @param dst Pointer to objects to zero out
 * @param cnt Number of objects
 */
template<class T>
constexpr void zero(T* dst, size_t cnt)
{
    std::fill_n(static_cast<volatile T*>(dst), cnt, T(0));
}

/**
 * @brief XOR block of bytes with given byte.
 * 
 * @param dst Destination array
 * @param val Byte to XOR with
 */
template<size_t N>
constexpr void xorb(byte (&dst)[N], byte val)
{
    for (size_t i = 0; i < N; ++i)
        dst[i] ^= val;
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
    return 0; // TODO
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

template<class H, size_t N>
struct consumer {
    static constexpr auto hash_size = N;
public:
    constexpr void operator()(span_i<> key, span_i<> msg, span_o<hash_size> out)
    {
        impl()->init(key);
        impl()->feed(msg);
        impl()->stop(out);
    }
    constexpr void operator()(span_i<> in, span_o<hash_size> out)
    {
        impl()->init();
        impl()->feed(in);
        impl()->stop(out);
    }
    constexpr ~consumer()   { impl()->wipe(); }
private:
    constexpr auto impl()   { return static_cast<H*>(this); }
    friend H;
};



}

#endif
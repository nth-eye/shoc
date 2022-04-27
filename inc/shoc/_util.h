#ifndef SHOC_UTIL_H
#define SHOC_UTIL_H

#include <cassert>
#include <bit>
// #include <span>
#include <algorithm>

namespace shoc {

using byte = uint8_t;

constexpr void swap(auto &x, auto &y)   { std::swap(x, y); }
constexpr auto rol(auto x, int s)       { return std::rotl(x, s); }
constexpr auto ror(auto x, int s)       { return std::rotr(x, s); }
constexpr bool little_endian()          { return std::endian::native == std::endian::little; }

constexpr void copy(void *dst, const void *src, size_t count)
{
    std::copy(static_cast<const byte*>(src), static_cast<const byte*>(src) + count, static_cast<byte*>(dst));
}

constexpr void fill(void *dst, byte val, size_t count)
{
    std::fill_n(static_cast<byte*>(dst), count, val);
}

constexpr void zero(void *dst, size_t count)
{
    std::fill_n(static_cast<volatile byte*>(dst), count, 0);
}

template <class T>
constexpr T ch(T x, T y, T z)           { return (x & y) ^ (~x & z); }

template <class T>
constexpr T maj(T x, T y, T z)          { return (x & y) ^ (x & z) ^ (y & z); }

template <class T>
constexpr T parity(T x, T y, T z)       { return x ^ y ^ z; }

/**
 * @brief Shift bits left in array of integer elements from least significant bit:
 * 10000000'11100001 ==> 00000001'11000010. 
 * If you view LSB as left, then it's a right shift from GCM perspective.
 * 
 * @tparam T Integer type
 * @param x Array of integers
 * @param len Number of elements
 */
template<class T>
constexpr void shift_from_lsb(T *x, size_t len)
{
    if (!x || !len)
        return;
    for (int i = len - 1; i > 0; --i) {
        x[i] <<= 1;
        x[i] |= x[i - 1] >> (sizeof(T) * 8 - 1);
    }
    x[0] <<= 1;
}

/**
 * @brief Integer power function.
 * 
 * @tparam T Integer type
 * @param base Base
 * @param exp Exponent
 * @return 'base' to the power 'exp'
 */
template<class T>
constexpr T ipow(T base, T exp)
{
    T res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/**
 * @brief Get number of bytes required to store N bits.
 * 
 * @param n Number of bits to store
 * @return Number of bytes
 */
constexpr size_t bytes_in_bits(size_t n)
{ 
    return (n >> 3) + !!(n & 7); 
}

/**
 * @brief Integer with bit set at given position.
 * 
 * @param n Bit position
 * @return Integer with set bit
 */
constexpr unsigned bit(unsigned n) 
{ 
    return 1ul << n; 
}

/**
 * @brief Get n-th bit of a byte.
 * 
 * @param b Byte
 * @param n Bit position from LSB
 * @return Bit value
 */
constexpr bool get_bit(byte b, int n)
{ 
    return (b >> n) & 1; 
}

/**
 * @brief Set n-th bit of a byte.
 * 
 * @param b Byte
 * @param n Bit position from LSB
 */
constexpr void set_bit(byte &b, int n)
{ 
    b |= 1 << n; 
}

/**
 * @brief Clear n-th bit of a byte.
 * 
 * @param b Byte
 * @param n Bit position from LSB
 */
constexpr void clr_bit(byte &b, int n)
{ 
    b &= ~(1 << n); 
}

/**
 * @brief Get n-th bit of an array (starting from LSB).
 * 
 * @param arr Array of bytes
 * @param n Index of bit to get
 * @return true if set
 */
constexpr bool get_arr_bit(const byte *arr, int n)
{
    return get_bit(arr[n >> 3], n & 7);
}

/**
 * @brief Set n-th bit in an array of bytes (starting from LSB).
 * 
 * @param arr Array of bytes
 * @param n Index of bit to set
 */
constexpr void set_arr_bit(byte *arr, int n)
{
    set_bit(arr[n >> 3], n & 7);
}

/**
 * @brief Clear n-th bit in an array of bytes (starting from LSB).
 * 
 * @param arr Array of bytes
 * @param n Index of bit to clear
 */
constexpr void clr_arr_bit(byte *arr, int n)
{
    clr_bit(arr[n >> 3], n & 7);
}

// /**
//  * @brief Get n-th bit of an array (starting from MSB).
//  * 
//  * @param arr Array of bytes
//  * @param n Index of bit to get
//  * @return true if set
//  */
// constexpr bool get_arr_bit_r(const byte *arr, int n)
// {
//     return get_bit(arr[n >> 3], 7 - (n & 7));
// }

// /**
//  * @brief Set n-th bit in an array of bytes (starting from MSB).
//  * 
//  * @param arr Array of bytes
//  * @param n Index of bit to set
//  */
// constexpr void set_arr_bit_r(byte *arr, int n)
// {
//     set_bit(arr[n >> 3], 7 - (n & 7));
// }

// /**
//  * @brief Clear n-th bit in an array of bytes (starting from MSB).
//  * 
//  * @param arr Array of bytes
//  * @param n Index of bit to clear
//  */
// constexpr void clr_arr_bit_r(byte *arr, int n)
// {
//     clr_bit(arr[n >> 3], 7 - (n & 7));
// }

/**
 * @brief Convert string with hexadecimal characters ('0'...'F') to array of bytes.
 * All non-hex chars will be mapped as 0. String with odd length will be interpeted
 * as with prepended '0', e.g. "fff" --> "0fff". Works with both upper and lower cases.
 * If output array is too small, as much as possible will be processed.
 * 
 * @param str Input string
 * @param str_len Input string length
 * @param bin Output array
 * @param max_bin_len Output array max size 
 * @return Length of resulting array, 0 if failed
 */
constexpr size_t hex_to_bin(const char *str, size_t str_len, uint8_t *bin, size_t max_bin_len)
{
    if (!str || !bin)
        return 0;

    // Mapping of ASCII characters to bin values
    const uint8_t map[256] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  !"#$%&'()*+,-./
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0123456789:;<=>?
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // @ABCDEFGHIJKLMNO
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVWXYZ[\]^_
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // `abcdefghijklmno
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // pqrstuvwxyz{|}~.
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
    };
    size_t i = 0; 
    size_t j = 0;
    size_t bin_len = (str_len + 1) >> 1; // The output array size is half the str length (rounded up)

    if (bin_len > max_bin_len) {
        bin_len = max_bin_len;
        str_len = max_bin_len << 1;
    }
    if (str_len & 1) {
        int idx = str[0];
        bin[0] = map[idx];
        i = j = 1;
    }
    for (; i < str_len; i += 2, j++) {
        int i0 = str[i];
        int i1 = str[i + 1];
        bin[j] = (map[i0] << 4) | map[i1];
    }
    return bin_len;
}

/**
 * @brief Convert byte array to hexadecimal null-terminated string (lowercase).
 * If input is too large for output, as much bytes as possible will be processed.
 * 
 * @param bin Input array
 * @param bin_len Input array length
 * @param str Output string
 * @param max_str_len Output string maximum length, including 0-terminator
 * @return Resulting string length, 0 if failed
 */
constexpr size_t bin_to_hex(const uint8_t *bin, size_t bin_len, char *str, size_t max_str_len)
{
    const char map[]= "0123456789abcdef";

    if (!str || !bin || !bin_len || !max_str_len)
        return 0;

    size_t str_len = bin_len << 1;

    if (str_len >= max_str_len) {
        str_len = (max_str_len - 1) & ~1;
        bin_len = str_len >> 1;
    }

    for (size_t i = 0; i < bin_len; ++i) {
        *str++ = map[bin[i] >> 4];
        *str++ = map[bin[i] & 0xf];
    }
    *str = 0;

    return str_len;
}

}

#endif
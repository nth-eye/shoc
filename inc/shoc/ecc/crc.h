#ifndef SHOC_ECC_CRC_H
#define SHOC_ECC_CRC_H

#include "shoc/util.h"

namespace shoc {
namespace impl::crc {

/**
 * @brief Calculate table for reciprocal CRC. Polynomial is
 * reflected and shift changed from MSB-to-LSB to LSB-to-MSB, 
 * resulting in right instead of left shift. For more info:
 * http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html#ch73
 * 
 * @tparam T Integer type
 * @param poly Polynomial
 * @return Lookup table with 256 elements of type T
 */
template<class T>
constexpr auto crc_table(T poly)
{
    poly = bitswap(poly);
    std::array<T, 256> table = {};

    for (int dividend = 0; dividend < 256; ++dividend) {
        T remainder = dividend;
        for (int i = 8; i; --i) {
            if (remainder & 1)
                remainder = (remainder >> 1) ^ poly;
            else
                remainder = (remainder >> 1);
        }
        table[dividend] = remainder;
    }
    return table;
}

/**
 * @brief Wrapper for CRC lookup table to allow it to be 
 * statically initialized and used in constexpr functions.
 * https://stackoverflow.com/questions/53941245/static-table-inside-constexpr-function  
 * 
 * @tparam T Integer type
 * @tparam poly Polynomial
 */
template<class T, T poly>
struct Table {
    static constexpr auto _ = crc_table(poly);
};

}

/**
 * @brief Calculate running CRC without lookup table, 
 * starting with a given value.
 * 
 * @tparam T Integer type
 * @tparam poly Polynomial
 * @tparam refin Reflect input bytes
 * @param val Running CRC value
 * @param data Data to calculate CRC on
 * @param size Data size in bytes
 * @return CRC value
 */
template<class T, T poly, bool refin>
constexpr T crc_feed_slow(T val, const void *data, size_t size)
{
    constexpr int bits = sizeof(T) * 8;
    constexpr T topbit = T(1) << (bits - 1);
    auto p = static_cast<const uint8_t*>(data);

    while (size--) {
        auto b = *p++;
        if constexpr (refin)
            b = bitswap(b);
        val ^= T(b) << (bits - 8);
        for (int i = 8; i; --i)
            val = val & topbit ? (val << 1) ^ poly : (val << 1);
    }
    return val;
}

/**
 * @brief Calculate CRC without lookup table. This is 
 * obviously slower, but saves 256 x sizeof(T) bytes,
 * which can be crucial in embedded systems.
 * 
 * @tparam T Integer type
 * @tparam poly Polynomial
 * @tparam init Initial value
 * @tparam xorout Value to XOR with final result
 * @tparam refin Reflect input bytes
 * @tparam refout Reflect output value
 * @param data Data to calculate CRC on
 * @param size Data size in bytes
 * @return CRC value
 */
template<class T, T poly, T init, T xorout, bool refin, bool refout>
constexpr T crc_slow(const void *data, size_t size)
{
    auto val = crc_feed_slow<T, poly, refin>(init, data, size);
    if constexpr (refout)
        val = bitswap(val);
    return val ^ xorout;
}

/**
 * @brief Calculate running CRC using lookup table, 
 * starting with a given value.
 * 
 * @tparam T Integer type
 * @tparam poly Polynomial
 * @tparam refin Reflect input bytes
 * @param val Running CRC value
 * @param data Data to calculate CRC on
 * @param size Data size in bytes
 * @return CRC value
 */
template<class T, T poly, bool refin>
constexpr T crc_feed_fast(T val, const void *data, size_t size)
{
    auto p = static_cast<const uint8_t*>(data);

    while (size--) {
        byte b = *p++;
        if constexpr (!refin)
            b = bitswap(b);
        val = impl::crc::Table<T, poly>::_[(val ^ b) & 0xff] ^ (val >> 8);
    }
    return val;
}

/**
 * @brief Calculate CRC using lookup table. This is faster, 
 * but requires 256 x sizeof(T) bytes, which isn't always
 * possible in embedded systems.
 * 
 * @tparam T Integer type
 * @tparam poly Polynomial
 * @tparam init Initial value
 * @tparam xorout Value to XOR with final result
 * @tparam refin Reflect input bytes
 * @tparam refout Reflect output value
 * @param data Data to calculate CRC on
 * @param size Data size in bytes
 * @return CRC value
 */
template<class T, T poly, T init, T xorout, bool refin, bool refout>
constexpr T crc_fast(const void *data, size_t size)
{
    auto val = crc_feed_fast<T, poly, refin>(bitswap(init), data, size);
    if constexpr (!refout)
        val = bitswap(val);
    return val ^ xorout;
}

}

#endif
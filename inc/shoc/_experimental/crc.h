#ifndef SHOC_ECC_CRC_H
#define SHOC_ECC_CRC_H

#include "shoc/util.h"

namespace shoc {

template<class T, T poly, bool reflect>
T crc_feed_slow(T val, const void *data, size_t size)
{
    constexpr int bits = sizeof(T) * 8;
    constexpr T topbit = T(1) << (bits - 1);
    auto p = static_cast<const uint8_t*>(data);

    while (size--) {
        auto b = *p++;
        if constexpr (reflect)
            b = reflect_bits(b);
        val ^= b << (bits - 8);
        for (int i = 8; i; --i)
            val = val & topbit ? (val << 1) ^ poly : (val << 1);
    }
    return val;
}

template<class T, T poly, T init, T xorout, bool refin, bool refout>
T crc_slow(const void *data, size_t size)
{
    auto val = crc_feed_slow<T, poly, refin>(init, data, size);
    if constexpr (refout)
        val = reflect_bits(val);
    return val ^ xorout;
}

template<class T>
constexpr auto crc_table(T poly)
{
    auto repoly = reflect_bits(poly);
    std::array<T, 256> table = {};

    for (int dividend = 0; dividend < 256; ++dividend) {
        T remainder = dividend;
        for (int i = 8; i; --i) {
            if (remainder & 1)
                remainder = (remainder >> 1) ^ repoly;
            else
                remainder = (remainder >> 1);
        }
        table[dividend] = remainder;
    }
    return table;
}

template<class T, T poly, bool reflect>
T crc_feed_fast(T val, const void *data, size_t size)
{
    static constexpr auto table = crc_table(poly);
    auto p = static_cast<const uint8_t*>(data);

    while (size--) {
        byte b = *p++;
        if constexpr (!reflect)
            b = reflect_bits(b);
        val = table[(val ^ b) & 0xff] ^ (val >> 8);
        // if constexpr (refout) {
        //    val = table[(val ^ byte) & 0xff] ^ (val >> 8);
        // } else {
        //     val ^= b << (bits - 8);
        //     val = table[val >> (bits - 8)] ^ (val << 8);
        // }
    }
    return val;
}

template<class T, T poly, T init, T xorout, bool refin, bool refout>
T crc_fast(const void *data, size_t size)
{
    auto val = crc_feed_fast<T, poly, refin>(init, data, size);
    if constexpr (!refout)
        val = reflect_bits(val);
    return val ^ xorout;
}

// template<class T, T poly, T initial, T xorout>
// struct Crc : Eater<Crc> {
//     static constexpr size_t SIZE        = sizeof(T);
//     static constexpr size_t BLOCK_SIZE  = sizeof(T); // In bytes
// public:
//     void init()
//     {
//         state = initial;
//     }
//     void feed(const void *data, size_t size)
//     {
//         assert(in || !len);
//         crc_feed<T, poly>(state, data, size);
//     }
//     void stop(byte *out)
//     {
//         assert(out);
//         putle<T>(state ^ xorout, out);
//     }
// private:
//     T state;
// };

}

#endif
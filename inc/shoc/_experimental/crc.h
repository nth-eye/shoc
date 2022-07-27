#ifndef SHOC_ECC_CRC_H
#define SHOC_ECC_CRC_H

#include "shoc/util.h"

namespace shoc {

template<class T, T poly, T init>
constexpr T crc(const void *data, size_t size)
{
    constexpr int bits = sizeof(T) * 8;
    constexpr T topb = T(1) << (bits - 1);

    auto res = init;
    auto ptr = static_cast<const uint8_t*>(data);

    while (size--) {
        res ^= T(*ptr) << (bits - 8);
        for (int i = 8; i; --i)
            res = res & topb ? (res << 1) ^ poly : (res << 1);
    }
    return res;
}

}

#endif
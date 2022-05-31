#ifndef SHOC_MAC_CBC_MAC_H
#define SHOC_MAC_CBC_MAC_H

#include "shoc/util.h"

namespace shoc {

template<class E>
inline size_t cbc_mac(E &ciph, byte *buf, const byte *aad, size_t aad_len, size_t pos)
{
    auto end = aad + aad_len;

    while (aad != end) {
        buf[pos] ^= *aad++;
        if (pos == 15)
            ciph.encrypt(buf, buf);
        pos = (pos + 1) & 0xf;
    }
    return pos;
}

template<class E>
inline void cbc_mac_padded(E &ciph, byte *buf, const byte *aad, size_t aad_len, size_t start)
{
    if (size_t i = cbc_mac(ciph, buf, aad, aad_len, start)) {
        for (; i < 16; ++i)
            buf[i] ^= 0;
        ciph.encrypt(buf, buf);
    }
}

}

#endif
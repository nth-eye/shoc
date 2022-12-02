#ifndef SHOC_MAC_CBC_MAC_H
#define SHOC_MAC_CBC_MAC_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Calculate and save CBC-MAC in running block buffer.
 * 
 * @tparam E Block cipher 
 * @param ciph Cipher object, must be already initialized
 * @param buf Block buffer
 * @param aad Additional authenticated data 
 * @param pos Start position within block buffer
 * @return Last position within block buffer
 */
template<class E>
constexpr size_t cbc_mac(E& ciph, span_o<E::block_size> buf, span_i<> aad, size_t pos)
{
    for (auto it : aad) {
        buf[pos] ^= it;
        if (pos == E::block_size - 1)
            ciph.encrypt(buf, buf);
        pos = (pos + 1) % E::block_size;
    }
    return pos;
}

/**
 * @brief Calculate and save CBC-MAC in running block buffer. 
 * XOR remainder bytes with zeros.
 * 
 * @tparam E Block cipher 
 * @param ciph Cipher object, must be already initialized
 * @param buf Blockbuffer
 * @param aad Additional authenticated data 
 * @param pos Start position within block buffer
 */
template<class E>
constexpr void cbc_mac_padded(E& ciph, span_o<E::block_size> buf, span_i<> aad, size_t pos)
{
    if (size_t i = cbc_mac(ciph, buf, aad, pos)) {
        for (; i < E::block_size; ++i)
            buf[i] ^= 0;
        ciph.encrypt(buf, buf);
    }
}

}

#endif
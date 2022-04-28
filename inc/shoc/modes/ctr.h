#ifndef SHOC_CTR_H
#define SHOC_CTR_H

#include "shoc/_util.h"

namespace shoc {

template<class E, size_t L = 4>
inline void gctr(const byte *iv, const byte *in, byte *out, size_t len, E &ciph)
{
    byte buf[16];
    byte ctr[16];
    copy(ctr, iv, 16);

    for (size_t i = 0; i < len; ++i) {
        size_t idx = i & 0xf;
        if (idx == 0) {
            ciph.encrypt(ctr, buf);
            incc<L>(ctr);
        }
        *out++ = buf[idx] ^ *in++;
    }
}

/**
 * @brief Encrypt with block cipher in counter mode. Number of counter-bytes is configurable.
 * All arguments MUST be valid pointers.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size, default is 4 
 * @param key Key
 * @param iv Initial vector
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 */
template<class E, size_t L = 4>
inline void ctr_encrypt(const byte *key, const byte *iv, const byte *in, byte *out, size_t len)
{
    E ciph {key};
    gctr<E, L>(iv, in, out, len, ciph);   
}

/**
 * @brief Decrypt with block cipher in counter mode. Number of counter-bytes is configurable.
 * All arguments MUST be valid pointers.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size, default is 4 
 * @param key Key
 * @param iv Initial vector
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 */
template<class E, size_t L = 4>
inline void ctr_decrypt(const byte *key, const byte *iv, const byte *in, byte *out, size_t len)
{
    ctr_encrypt<E, L>(key, iv, in, out, len);
}

}

#endif
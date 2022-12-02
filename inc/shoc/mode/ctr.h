#ifndef SHOC_MODE_CTR_H
#define SHOC_MODE_CTR_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Basic counter mode function, used as a component in CTR and GCM modes. 
 * Counter size is configurable. All pointers MUST be valid.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size, default is 4
 * @param iv Initial vector
 * @param in Input data
 * @param out Output data
 * @param len Data length
 * @param ciph Cipher object, must be already initialized
 */
template<class E, size_t L = 4>
constexpr void ctrf(span_i<E::block_size> iv, const byte* in, byte* out, size_t len, E& ciph)
{
    byte buf[E::block_size];
    byte ctr[E::block_size];
    copy(ctr, iv.data(), iv.size());

    for (size_t i = 0; i < len; ++i) {
        auto idx = i % sizeof(buf);
        if (!idx) {
            ciph.encrypt(ctr, buf);
            incc<L, E::block_size>(ctr);
        }
        *out++ = buf[idx] ^ *in++;
    }
}

/**
 * @brief Encrypt with block cipher in counter mode. Number of counter-bytes is configurable.
 * All pointers MUST be valid. Text length can be arbitrary.
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
constexpr void ctr_encrypt(span_i<E::key_size> key, span_i<E::block_size> iv, const byte* in, byte* out, size_t len)
{
    E ciph {key};
    ctrf<E, L>(iv, in, out, len, ciph);   
}

/**
 * @brief Decrypt with block cipher in counter mode. Number of counter-bytes is configurable.
 * All pointers MUST be valid. Text length can be arbitrary.
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
constexpr void ctr_decrypt(span_i<E::key_size> key, span_i<E::block_size> iv, const byte* in, byte* out, size_t len)
{
    ctr_encrypt<E, L>(key, iv, in, out, len);
}

}

#endif
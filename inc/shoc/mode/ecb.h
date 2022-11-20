#ifndef SHOC_MODE_ECB_H
#define SHOC_MODE_ECB_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in electronic codebook mode. All 
 * pointers MUST be valid and length be multiple of E::block_size.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param in Plain text
 * @param out Cipher text 
 * @param len Text length, multiple of E::block_size
 */
template<class E>
constexpr void ecb_encrypt(span_i<E::block_size> key, const byte* in, byte* out, size_t len)
{
    assert((len % E::block_size) == 0);

    E ciph {key};
    
    for (size_t i = 0; i < len; i += E::block_size)
        ciph.encrypt(
            span_i<E::block_size>{in + i}, 
            span_o<E::block_size>{out + i});
}

/**
 * @brief Decrypt with block cipher in electronic codebook mode. All 
 * pointers MUST be valid and length is multiple of E::block_size.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length, multiple of E::block_size
 */
template<class E>
inline void ecb_decrypt(const byte *key, const byte *in, byte *out, size_t len)
{
    assert((len % E::block_size) == 0);

    E ciph {key};

    for (size_t i = 0; i < len; i += E::block_size())
        ciph.decrypt(in + i, out + i);
}

}

#endif
#ifndef SHOC_MODE_ECB_H
#define SHOC_MODE_ECB_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in electronic codebook mode.
 * All pointers MUST be valid and length is multiple of 16.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param in Plain text
 * @param out Cipher text 
 * @param len Text length, multiple of 16
 */
template<class E>
inline void ecb_encrypt(const byte *key, const byte *in, byte *out, size_t len)
{
    E ciph {key};
    
    for (size_t i = 0; i < len; i += 16)
        ciph.encrypt(in + i, out + i);
}

/**
 * @brief Decrypt with block cipher in electronic codebook mode.
 * All pointers MUST be valid and length is multiple of 16.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length, multiple of 16
 */
template<class E>
inline void ecb_decrypt(const byte *key, const byte *in, byte *out, size_t len)
{
    E ciph {key};

    for (size_t i = 0; i < len; i += 16)
        ciph.decrypt(in + i, out + i);
}

}

#endif
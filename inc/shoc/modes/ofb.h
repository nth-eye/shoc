#ifndef SHOC_OFB_H
#define SHOC_OFB_H

#include "shoc/_util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in output feedback mode.
 * All arguments MUST be valid pointers.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Plain text
 * @param out Cipher text
 * @param len Text length
 */
template<class E>
inline void ofb_encrypt(const byte *key, const byte *iv, const byte *in, byte *out, size_t len)
{
    E ciph {key};

    byte buf[16];
    copy(buf, iv, 16);

    for (size_t i = 0; i < len; ++i) {
        size_t idx = i & 0xf;
        if (idx == 0) 
            ciph.encrypt(buf, buf);
        *out++ = buf[idx] ^ *in++;
    }
}

/**
 * @brief Decrypt with block cipher in output feedback mode. 
 * All arguments MUST be valid pointers.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 */
template<class E>
inline void ofb_decrypt(const byte *key, const byte *iv, const byte *in, byte *out, size_t len)
{
    ofb_encrypt<E>(key, iv, in, out, len);
}

}

#endif
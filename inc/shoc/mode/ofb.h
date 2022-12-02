#ifndef SHOC_MODE_OFB_H
#define SHOC_MODE_OFB_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in output feedback mode.
 * All pointers MUST be valid. Text length can be arbitrary.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Plain text
 * @param out Cipher text
 * @param len Text length
 */
template<class E>
constexpr void ofb_encrypt(span_i<E::key_size> key, span_i<E::block_size> iv, const byte* in, byte* out, size_t len)
{
    E ciph {key};
    byte buf[E::block_size];
    copy(buf, iv.data(), iv.size());

    for (size_t i = 0; i < len; ++i) {
        auto idx = i % sizeof(buf);
        if (!idx) 
            ciph.encrypt(buf, buf);
        *out++ = buf[idx] ^ *in++;
    }
}

/**
 * @brief Decrypt with block cipher in output feedback mode. 
 * All pointers MUST be valid. Text length can be arbitrary.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 */
template<class E>
constexpr void ofb_decrypt(span_i<E::key_size> key, span_i<E::block_size> iv, const byte* in, byte* out, size_t len)
{
    ofb_encrypt<E>(key, iv, in, out, len);
}

}

#endif
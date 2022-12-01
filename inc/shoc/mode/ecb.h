#ifndef SHOC_MODE_ECB_H
#define SHOC_MODE_ECB_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in electronic codebook mode. All 
 * pointers MUST be valid. Text length can be arbitrary, but will be 
 * padded with zeros if not multiple of E::block_size.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param in Plain text
 * @param out Cipher text 
 * @param len Text length
 */
template<class E>
constexpr void ecb_encrypt(span_i<E::key_size> key, const byte* in, byte* out, size_t len)
{
    E ciph {key};

    for (size_t i = 0; i < len;) {
        byte buf[E::block_size]{};
        auto block_len = std::min(len - i, sizeof(buf));
        copy(buf, in + i, block_len);
        ciph.encrypt(buf, buf);
        copy(out + i, buf, block_len);
        i += block_len;
    }
}

/**
 * @brief Decrypt with block cipher in electronic codebook mode. All 
 * pointers MUST be valid. Text length can be arbitrary, but will be 
 * padded with zeros if not multiple of E::block_size.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 */
template<class E>
constexpr void ecb_decrypt(span_i<E::key_size> key, const byte *in, byte *out, size_t len)
{
    E ciph {key};

    for (size_t i = 0; i < len;) {
        byte buf[E::block_size]{};
        auto block_len = std::min(len - i, sizeof(buf));
        copy(buf, in + i, block_len);
        ciph.decrypt(buf, buf);
        copy(out + i, buf, block_len);
        i += block_len;
    }
}

}

#endif
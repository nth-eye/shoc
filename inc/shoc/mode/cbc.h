#ifndef SHOC_MODE_CBC_H
#define SHOC_MODE_CBC_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in cipher block chaining mode. All 
 * pointers MUST be valid. Text length MUST be multiple of E::block_size, 
 * otherwise last block be processed with previous block remainder.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Plain text
 * @param out Cipher text
 * @param len Text length
 */
template<class E>
constexpr void cbc_encrypt(span_i<E::key_size> key, span_i<E::block_size> iv, const byte* in, byte* out, size_t len)
{
    E ciph {key};
    byte buf[E::block_size];
    copy(buf, iv.data(), iv.size());
    
    for (size_t i = 0; i < len;) {
        auto block_len = std::min(len - i, sizeof(buf));
        xorb(buf, in + i, block_len);
        ciph.encrypt(buf, buf);
        copy(out + i, buf, block_len);
        i += block_len;
    }
}

/**
 * @brief Decrypt with block cipher in cipher block chaining mode. All 
 * pointers MUST be valid. Text length MUST be multiple of E::block_size, 
 * otherwise last block will be processed with previous block remainder.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 */
template<class E>
constexpr void cbc_decrypt(span_i<E::key_size> key, span_i<E::block_size> iv, const byte* in, byte* out, size_t len)
{
    E ciph {key};
    byte tmp_buf[E::block_size]{};
    byte xor_buf[E::block_size];
    copy(xor_buf, iv.data(), iv.size());

    for (size_t i = 0; i < len;) {
        auto block_len = std::min(len - i, E::block_size);
        copy(tmp_buf, in + i, block_len);
        ciph.decrypt(tmp_buf, tmp_buf);
        xorb(tmp_buf, xor_buf, block_len);
        copy(out + i, tmp_buf, block_len);
        copy(xor_buf, in + i, block_len);
        i += block_len;
    }
}

};

#endif
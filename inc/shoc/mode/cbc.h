#ifndef SHOC_MODE_CBC_H
#define SHOC_MODE_CBC_H

#include "shoc/util.h"

namespace shoc {

/**
 * @brief Encrypt with block cipher in cipher block chaining mode.
 * All pointers MUST be valid and length is multiple of 16.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Plain text
 * @param out Cipher text
 * @param len Text length, multiple of 16
 */
template<class E>
inline void cbc_encrypt(const byte *key, const byte *iv, const byte *in, byte *out, size_t len)
{
    E ciph {key};

    auto end = out + len;
    byte buf[16];
	copy(buf, iv, 16);
    
    for (; out < end; out += 16, in += 16) {
        xorb(buf, in);
        ciph.encrypt(buf, buf);
        copy(out, buf, 16);
    }
}

/**
 * @brief Decrypt with block cipher in cipher block chaining mode.
 * All pointers MUST be valid and length is multiple of 16.
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length, multiple of 16
 */
template<class E>
inline void cbc_decrypt(const byte *key, const byte *iv, const byte *in, byte *out, size_t len)
{
    E ciph {key};

    auto end = out + len;
    byte xor_buf[16];
    byte tmp_buf[16];
	copy(xor_buf, iv, 16);

    for (; out < end; out += 16, in += 16) {
        copy(tmp_buf, in, 16);
        ciph.decrypt(in, out);
        xorb(out, xor_buf);
        copy(xor_buf, tmp_buf, 16);
    }
}

};

#endif
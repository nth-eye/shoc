#ifndef SHOC_AES_CBC_H
#define SHOC_AES_CBC_H

#include "shoc/aes.h"

namespace shoc {

inline bool cbc_encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;
        
    if (len & 0xf)
        return false;

    uint8_t *end = out + len;
    uint8_t buf[16];
	copy(buf, iv, 16);

    Aes ctx{key};

    for (; out < end; out += 16) {
        for (size_t j = 0; j < 16; ++j) 
            buf[j] ^= *in++;
        ctx.encrypt(buf, buf);
        copy(out, buf, 16);
    }
    return true;
}

inline bool cbc_decrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;

    if (len & 0xf)
        return false;

    const uint8_t *end = in + len;
    uint8_t xor_buf[16];
    uint8_t tmp_buf[16];
	copy(xor_buf, iv, 16);

    Aes ctx{key};

    for (; in < end; in += 16) {
        copy(tmp_buf, in, 16);
        ctx.decrypt(in, out);
        for (size_t j = 0; j < 16; ++j)
            *out++ ^= xor_buf[j];
        copy(xor_buf, tmp_buf, 16);
    }
    return true;
}

};

#endif
#ifndef AES_CBC_H
#define AES_CBC_H

#include "aes.h"

namespace creep {

constexpr bool cbc_encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;
        
    if (len & 0xf)
        return false;

    uint8_t *end    = out + len;
    uint8_t buf[16] = {};
	memcpy(buf, iv, 16);

    AES ctx{key};

    for (; out < end; out += 16) {
        for (size_t j = 0; j < 16; ++j) 
            buf[j] ^= *in++;
        ctx.encrypt(buf, buf);
        memcpy(out, buf, 16);
    }
    return true;
}

constexpr bool cbc_decrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;

    if (len & 0xf)
        return false;

    const uint8_t *end  = in + len;
    uint8_t xor_buf[16] = {};
    uint8_t tmp_buf[16] = {};
	memcpy(xor_buf, iv, 16);

    AES ctx{key};

    for (; in < end; in += 16) {
        memcpy(tmp_buf, in, 16);
        ctx.decrypt(in, out);
        for (size_t j = 0; j < 16; ++j)
            *out++ ^= xor_buf[j];
        memcpy(xor_buf, tmp_buf, 16);
    }
    return true;
}

};

#endif
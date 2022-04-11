#ifndef AES_CFB_H
#define AES_CFB_H

#include "aes.h"

namespace creep {

constexpr bool cfb_encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;

    uint8_t buf[16] = {};
    memcpy(buf, iv, 16);

    AES ctx{key};

    for (size_t i = 0; i < len; ++i) {
        size_t idx = i & 0xf;
        if (idx == 0) 
            ctx.encrypt(buf, buf);
        *out++ = buf[idx] ^= *in++;
    }
    return true;
}

constexpr bool cfb_decrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;

    uint8_t tmp = 0;
    uint8_t buf[16] = {};
    memcpy(buf, iv, 16);

    AES ctx{key};

    for (size_t i = 0; i < len; ++i) {
        size_t idx = i & 0xf;
        if (idx == 0) 
            ctx.encrypt(buf, buf);
        tmp = *in++;
        *out++ = buf[idx] ^ tmp;
        buf[idx] = tmp;
    }
    return true;
}

}

#endif
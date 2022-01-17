#ifndef AES_CFB_H
#define AES_CFB_H

#include "aes.h"

namespace creep::cfb {

constexpr bool encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;

    uint8_t buf[16] = {};

    memcpy(buf, iv, 16);

    aes::AES ctx;

    ctx.init(key);

    for (size_t i = 0; i < len; ++i) {
        if (!(i & 0xf)) 
            ctx.encrypt(buf, buf);
        *out++ = (buf[i & 0xf] ^= *in++);
    }
    return true;
}

}

#endif
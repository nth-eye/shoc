#ifndef AES_OFB_H
#define AES_OFB_H

#include "aes.h"

namespace creep {

constexpr bool ofb_encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
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
        *out++ = buf[idx] ^ *in++;
    }
    return true;
}

constexpr bool ofb_decrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    return ofb_encrypt(key, iv, in, out, len);
}

}

#endif
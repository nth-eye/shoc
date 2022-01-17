#ifndef AES_CBC_H
#define AES_CBC_H

#include "aes.h"

namespace creep::cbc {

constexpr bool encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;
        
    if (len & 0xf)
        return false;

    uint8_t *end    = out + len;
    uint8_t buf[16] = {};

	memcpy(buf, iv, 16);

    aes::AES ctx;

    ctx.init(key);

    for (; out < end; out += 16) {
        for (size_t j = 0; j < 16; ++j) 
            buf[j] ^= *in++;
        ctx.encrypt(buf, buf);
        memcpy(out, buf, 16);
    }
    return true;
}

};

#endif
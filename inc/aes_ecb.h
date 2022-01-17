#ifndef AES_ECB_H
#define AES_ECB_H

#include "aes.h"

namespace creep {

constexpr bool ecb_encrypt(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !in || !out || !len)
        return false;

    if (len & 0xf)
        return false;

    AES ctx;

    ctx.init(key);
    
    for (size_t i = 0; i < len; i += 16)
        ctx.encrypt(in + i, out + i);
    return true;
} 

}

#endif
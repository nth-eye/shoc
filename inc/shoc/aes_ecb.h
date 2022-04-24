#ifndef SHOC_AES_ECB_H
#define SHOC_AES_ECB_H

#include "shoc/aes.h"

namespace shoc {

inline bool ecb_encrypt(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !in || !out || !len)
        return false;

    if (len & 0xf)
        return false;

    Aes ctx{key};
    
    for (size_t i = 0; i < len; i += 16)
        ctx.encrypt(in + i, out + i);
    return true;
}

inline bool ecb_decrypt(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !in || !out || !len)
        return false;

    if (len & 0xf)
        return false;

    Aes ctx{key};

    for (size_t i = 0; i < len; i += 16)
        ctx.decrypt(in + i, out + i);
    return true;
}

}

#endif
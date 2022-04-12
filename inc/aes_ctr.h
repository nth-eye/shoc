#ifndef AES_CTR_H
#define AES_CTR_H

#include "aes.h"

namespace creep {

constexpr bool ctr_encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    if (!key || !iv || !in || !out || !len)
        return false;

    uint8_t buf[16]     = {};
    uint8_t nonce[16]   = {};
    uint64_t counter    = 
        (static_cast<uint64_t>(iv[12]) << 24) | 
        (static_cast<uint64_t>(iv[13]) << 16) |
        (static_cast<uint64_t>(iv[14]) << 8)  | 
        (static_cast<uint64_t>(iv[15]));
    memcpy(nonce, iv, 16);

    AES ctx{key};

    auto end = out + len;
    auto idx = 0;
    
    while (out < end) {
        if ((idx &= 0xf) == 0) {
            ctx.encrypt(nonce, buf);
            ++counter;
            nonce[12] = counter >> 24;
            nonce[13] = counter >> 16;
            nonce[14] = counter >> 8;
            nonce[15] = counter;
        }
        *out++ = buf[idx++] ^ *in++;
    }
    return true;       
}

constexpr bool ctr_decrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *in, uint8_t *out, size_t len)
{
    return ctr_encrypt(key, iv, in, out, len);
}

}

#endif
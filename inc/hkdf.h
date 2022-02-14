#ifndef HKDF_H
#define HKDF_H

#include "hmac.h"

namespace creep {

template<class H>
bool hkdf(
    uint8_t *okm, size_t okm_len, 
    const void *ikm, size_t ikm_len, 
    const void *salt, size_t salt_len,
    const void *info, size_t info_len)
{
    const uint8_t default_salt[H::SIZE] = {};
    uint8_t prk[H::SIZE];
    uint8_t t[H::SIZE];
    HMAC<H> hmac;

    if (!salt || !salt_len) {
        salt = default_salt;
        salt_len = sizeof(default_salt);
    }
    hmac.init(salt, salt_len);
    hmac.update(ikm, ikm_len);
    hmac.final(prk);

    size_t offset = 0;
    size_t iterations = (okm_len + H::SIZE - 1) / H::SIZE;
    if (iterations > 255)
        return false;

    for (uint8_t i = 1; i <= iterations; ++i, offset += H::SIZE) {
        
        hmac.init(prk, sizeof(prk));
        if (i > 1)
            hmac.update(t, sizeof(t));
        hmac.update(info, info_len);
        hmac.update(&i, 1);
        hmac.final(t);

        if (i == iterations)
            memcpy(&okm[offset], t, okm_len - offset);
        else
            memcpy(&okm[offset], t, H::SIZE);
    }
    return true;
}

}

#endif
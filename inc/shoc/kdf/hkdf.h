#ifndef SHOC_KDF_HKDF_H
#define SHOC_KDF_HKDF_H

#include "shoc/mac/hmac.h"

namespace shoc {

template<class H>
bool hkdf(
    byte *okm, size_t okm_len, 
    const void *ikm, size_t ikm_len, 
    const void *salt, size_t salt_len,
    const void *info, size_t info_len)
{
    const byte default_salt[H::SIZE] = {};
    byte prk[H::SIZE];
    byte t[H::SIZE];
    Hmac<H> hmac;

    if (!salt || !salt_len) {
        salt = default_salt;
        salt_len = sizeof(default_salt);
    }
    hmac.init(salt, salt_len);
    hmac.feed(ikm, ikm_len);
    hmac.stop(prk);

    size_t offset = 0;
    size_t iterations = (okm_len + H::SIZE - 1) / H::SIZE;
    if (iterations > 255)
        return false;

    for (byte i = 1; i <= iterations; ++i, offset += H::SIZE) {
        
        hmac.init(prk, sizeof(prk));
        if (i > 1)
            hmac.feed(t, sizeof(t));
        hmac.feed(info, info_len);
        hmac.feed(&i, 1);
        hmac.stop(t);

        if (i == iterations)
            copy(&okm[offset], t, okm_len - offset);
        else
            copy(&okm[offset], t, H::SIZE);
    }
    return true;
}

}

#endif
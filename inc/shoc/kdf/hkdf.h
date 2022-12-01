#ifndef SHOC_KDF_HKDF_H
#define SHOC_KDF_HKDF_H

#include "shoc/mac/hmac.h"

namespace shoc {
namespace impl::hkdf {
template<size_t N>
inline constexpr byte default_salt[N] = {};
}

template<class H>
constexpr bool hkdf(
    span_o<> okm,
    span_i<> ikm,
    span_i<> salt,
    span_i<> info)
{
    hmac_ctx<H> hmac;
    byte prk[H::hash_size];
    byte t[H::hash_size];

    if (salt.empty())
        salt = impl::hkdf::default_salt<H::hash_size>;

    hmac.init(salt);
    hmac.feed(ikm);
    hmac.stop(prk);

    size_t offset = 0;
    size_t iterations = (okm.size() + H::hash_size - 1) / H::hash_size;

    if (iterations > 255)
        return false;

    for (byte i = 1; i <= iterations; ++i, offset += H::hash_size) {
        
        hmac.init(prk);
        if (i > 1)
            hmac.feed(t);
        hmac.feed(info);
        hmac.feed({&i, 1});
        hmac.stop(t);

        if (i == iterations)
            copy(&okm[offset], t, okm.size() - offset);
        else
            copy(&okm[offset], t, H::hash_size);
    }
    return true;
}

}

#endif
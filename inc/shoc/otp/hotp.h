#ifndef SHOC_OTP_HOTP_H
#define SHOC_OTP_HOTP_H

#include "shoc/mac/hmac.h"

namespace shoc {

template<class H>
constexpr uint32_t hotp(span_i<> key, uint64_t count, int mod)
{
    static_assert(H::hash_size >= 20, "HOTP hash algorithm must provide at least 20 byte long digest");

    byte digest[H::hash_size];

    if (std::is_constant_evaluated()) {
        byte rawcnt[8];
        for (size_t i = 0; i < 8; ++i) {
            if constexpr (little_endian())
                rawcnt[i] = count >> (8 * (7 - i));
            else
                rawcnt[i] = count >> (8 * i);
        }
        hmac<H>(key, rawcnt, digest);
    } else {
        if constexpr (little_endian()) {
            count = (count & 0x00000000ffffffff) << 32 | (count & 0xffffffff00000000) >> 32;
            count = (count & 0x0000ffff0000ffff) << 16 | (count & 0xffff0000ffff0000) >> 16;
            count = (count & 0x00ff00ff00ff00ff) <<  8 | (count & 0xff00ff00ff00ff00) >>  8;
        }
        hmac<H>(key, {&count, sizeof(count)}, digest);
    }
    uint32_t offset = digest[H::hash_size - 1] & 0x0f;
    uint32_t bin_code = 
            (digest[offset]     & 0x7f) << 24 |
            (digest[offset + 1] & 0xff) << 16 |
            (digest[offset + 2] & 0xff) <<  8 |
            (digest[offset + 3] & 0xff);
    return bin_code % utl::ipow(10, mod);
}

}

#endif
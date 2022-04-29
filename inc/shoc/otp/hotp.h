#ifndef SHOC_OTP_HOTP_H
#define SHOC_OTP_HOTP_H

#include "shoc/mac/hmac.h"

namespace shoc {

template<class H>
uint32_t hotp(const void *key, size_t key_len, uint64_t count, int mod)
{
    static_assert(H::SIZE >= 20, "HOTP hash algorithm must provide at least 20 byte long value");

    byte digest[H::SIZE];

    if constexpr (little_endian()) {
        count = (count & 0x00000000ffffffff) << 32 | (count & 0xffffffff00000000) >> 32;
        count = (count & 0x0000ffff0000ffff) << 16 | (count & 0xffff0000ffff0000) >> 16;
        count = (count & 0x00ff00ff00ff00ff) <<  8 | (count & 0xff00ff00ff00ff00) >>  8;
    }
    hmac<H>(&count, sizeof(count), key, key_len, digest);

    uint32_t offset = digest[H::SIZE - 1] & 0x0f;
    uint32_t bin_code = 
            (digest[offset]     & 0x7f) << 24 |
            (digest[offset + 1] & 0xff) << 16 |
            (digest[offset + 2] & 0xff) <<  8 |
            (digest[offset + 3] & 0xff);

    return bin_code % ipow(10, mod);
}

}

#endif
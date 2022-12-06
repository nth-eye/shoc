#ifndef SHOC_OTP_HOTP_H
#define SHOC_OTP_HOTP_H

#include "shoc/mac/hmac.h"

namespace shoc {

template<class H>
constexpr uint32_t hotp(span_i<> key, uint64_t count, int mod)
{
    static_assert(H::hash_size >= 20, "HOTP hash algorithm must provide at least 20 byte long digest");

    byte digest[H::hash_size];

    if constexpr (little_endian())
        count = byteswap(count);

    if (std::is_constant_evaluated()) {
        auto rawcnt = std::bit_cast<std::array<byte, sizeof(count)>>(count);
        hmac<H>(key, rawcnt, digest);
    } else {
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
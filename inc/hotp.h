#ifndef HOTP_H
#define HOTP_H

#include "hmac.h"

namespace creep {

// constexpr int ipow(int base, int exp)
// {
//     int result = 1;
//     while (1) {
//         if (exp & 1)
//             result *= base;
//         exp >>= 1;
//         if (!exp)
//             break;
//         base *= base;
//     }
//     return result;
// }

// template<class H>
// constexpr uint32_t hotp(H &hash, void *key, size_t key_len, uint64_t count, int mod)
// {
//     static_assert(H::HASH_SIZE >= 20, "HOTP hash algorithm must provide at least 20 byte long value");

//     uint8_t digest[H::HASH_SIZE];

// #if 'ABCD' == 0x41424344
//     count = (count & 0x00000000ffffffff) << 32 | (count & 0xffffffff00000000) >> 32;
//     count = (count & 0x0000ffff0000ffff) << 16 | (count & 0xffff0000ffff0000) >> 16;
//     count = (count & 0x00ff00ff00ff00ff) <<  8 | (count & 0xff00ff00ff00ff00) >>  8;
// #endif
//     hmac(hash, &count, sizeof(count), key, key_len, digest);

//     uint32_t offset = digest[H::HASH_SIZE - 1] & 0x0f;
//     uint32_t bin_code = 
//             (digest[offset]   & 0x7f) << 24 |
//             (digest[offset+1] & 0xff) << 16 |
//             (digest[offset+2] & 0xff) <<  8 |
//             (digest[offset+3] & 0xff);

//     return bin_code % ipow(10, mod);
// }

}

#endif
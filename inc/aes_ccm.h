#ifndef AES_CCM
#define AES_CCM

#include "aes.h"
#include <cstdio>
#include <cctype>

namespace creep {

inline void util_log_hex(const void *data, size_t len, const char *str)
{
    printf("[%08llx] \"%s\" - %lu bytes \n", (long long unsigned) data, str, len);

    if (!data || !len)
        return;

    const uint8_t *p = static_cast<const uint8_t*>(data);

    for (size_t i = 0; i < len; ++i) {

        if (!(i & 15))
            printf("[%08llx]  ", (long long unsigned) p + i);

        printf("%02x ", p[i]);
        
        if ((i & 7) == 7)
            putchar(' ');

        if ((i & 15) == 15) {
            
            printf("|");

            for (int j = 15; j >= 0; --j) {

                char c = p[i - j];

                if (isprint(c))
                    printf("%c", c);
                else
                    printf(".");
            }
            printf("|\n");
        }
    }

    int rem = len - ((len >> 4) << 4);

    if (rem) {

        printf("%*c |", (16 - rem) * 3 + ((~rem & 8) >> 3), ' ');

        for (int j = rem; j; --j) {

            char c = p[len - j];

            if (isprint(c))
                printf("%c", c);
            else
                printf(".");
        }

        for (int j = 0; j < 16 - rem; ++j)
            putchar('.');

        printf("|\n");
    }
}

inline size_t cbc_mac(AES &ctx, uint8_t *buf, const uint8_t *aad, size_t aad_len, size_t pos)
{
    auto end = aad + aad_len;

    while (aad != end) {
        buf[pos] ^= *aad++;
        if (pos == 15) {
            util_log_hex(buf, 16, "After xor");
            ctx.encrypt(buf, buf);
            util_log_hex(buf, 16, "After AES");
        }
        pos = (pos + 1) & 0xf;
    }
    return pos;
}

inline void cbc_mac_padded(AES &ctx, uint8_t *buf, const uint8_t *aad, size_t aad_len, size_t start)
{
    if (size_t i = cbc_mac(ctx, buf, aad, aad_len, start)) {
        for (; i < 16; ++i)
            buf[i] ^= 0;
        util_log_hex(buf, 16, "After xor");
        ctx.encrypt(buf, buf);
        util_log_hex(buf, 16, "After AES");
    }
}

inline void ccm_ctr(AES &ctx, uint8_t *a_0, const uint8_t *in, uint8_t *out, size_t len)
{
    uint8_t buf[16]     = {};
    uint8_t a_i[16]     = {};
    uint16_t counter    = 
        (static_cast<uint64_t>(a_0[14]) << 8)  | 
        (static_cast<uint64_t>(a_0[15]));
    memcpy(a_i, a_0, 16);

    auto end = out + len;
    auto idx = 0;

    util_log_hex(a_i, sizeof(a_i), "CTR Start");
    
    while (out < end) {
        if ((idx &= 0xf) == 0) {
            ++counter;
            a_i[14] = counter >> 8;
            a_i[15] = counter;
            ctx.encrypt(a_i, buf);
            util_log_hex(buf, sizeof(buf), "CTR");
        }
        *out++ = buf[idx++] ^ *in++;
    }
}

inline bool ccm_encrypt(
    const uint8_t *key, 
    const uint8_t *nonce, size_t nonce_len,
    const uint8_t *aad, size_t aad_len,
    const uint8_t *in, 
    uint8_t *out, size_t len,
    uint8_t *tag, size_t tag_len)
{
    if (!key || !nonce || !in || !out || !tag || !len)
        return false;

    if (aad && !aad_len)
        return false;

    if (nonce_len > 13 || nonce_len < 7)
        return false;

    if (tag_len > 16 || tag_len < 4 || tag_len & 1)
        return false;

    size_t l = 15 - nonce_len;
    size_t m = tag_len;
    uint8_t block[16] = {};

    printf("L:  %u \n", l);
    printf("M:  %u \n", m);

    // ANCHOR: Authentication

    block[0] =  (aad_len ? 0x40 : 0)    | 
                (((m - 2) / 2) << 3)    |
                (l - 1);

    memcpy(&block[1], nonce, nonce_len);

    for (size_t 
            i = nonce_len + 1, 
            j = l - 1; 
        i < 16; ++i) 
    {
        block[i] = len >> (8 * j--);
    }

    AES ctx{key};

    util_log_hex(block, sizeof(block), "CBC IV in");
    ctx.encrypt(block, block);
    util_log_hex(block, sizeof(block), "CBC IV out");

    if (aad_len) {
        size_t start;
        if (aad_len < 65536 - 256) {
            block[0] ^= aad_len >> 8;
            block[1] ^= aad_len;
            start = 2;
        } else if (aad_len < 4294967296) {
            block[0] ^= 0xff;
            block[1] ^= 0xfe;
            block[2] ^= aad_len >> 24;
            block[3] ^= aad_len >> 16;
            block[4] ^= aad_len >> 8;
            block[5] ^= aad_len;
            start = 6;
        } else {
            block[0] ^= 0xff;
            block[1] ^= 0xff;
            block[2] ^= aad_len >> 56;
            block[3] ^= aad_len >> 48;
            block[4] ^= aad_len >> 40;
            block[5] ^= aad_len >> 32;
            block[6] ^= aad_len >> 24;
            block[7] ^= aad_len >> 16;
            block[8] ^= aad_len >> 8;
            block[9] ^= aad_len;
            start = 10;
        }
        cbc_mac_padded(ctx, block, aad, aad_len, start);
    }

    if (len) 
        cbc_mac_padded(ctx, block, in, len, 0);

    memcpy(tag, block, tag_len);

    util_log_hex(tag, tag_len, "CBC-MAC");

    // ANCHOR: Encryption

    block[0] = l - 1;
    memcpy(&block[1], nonce, nonce_len);
    memset(&block[1 + nonce_len], 0, l);
    ccm_ctr(ctx, block, in, out, len);
    memset(&block[1 + nonce_len], 0, l);

    ctx.encrypt(block, block);

    for (size_t i = 0; i < m; ++i)
        tag[i] ^= block[i];
    
    util_log_hex(tag, m, "CTR MAC");
    util_log_hex(out, len, "OUT");

    return true;
}

}

#endif
#ifndef AES_CCM
#define AES_CCM

#include "aes.h"

namespace creep {

constexpr bool ccm_cbc_mac(AES &ctx, uint8_t *tag, uint8_t *aad, size_t aad_len, size_t start)
{
    size_t i = start;
    size_t idx;
    for (; i < aad_len + start; ++i) {
        idx = i & 0xf;
		tag[idx] ^= *aad++;
		if (idx == 0)
			ctx.encrypt(tag, tag);
	}
    idx = (i - 1) & 0xf;
    if (idx) {
        for (size_t j = idx + 1; j < 16; ++j)
            tag[j] ^= 0;
        ctx.encrypt(tag, tag);
    }
}

constexpr bool ccm_encrypt(
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

    uint8_t block[16]   = {};
    uint8_t tag[16]     = {};

    // ANCHOR: Authentication

    block[0] = 
        (aad_len ? 0x40 : 0)    | 
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

    ctx.encrypt(buf, tag);

    if (aad_len) {
        size_t start;
        if (aad_len < 65536 - 256) {
            tag[0] ^= aad_len >> 8;
            tag[1] ^= aad_len;
            start = 2;
        } else if (aad_len < 4294967296) {
            tag[0] ^= 0xff;
            tag[1] ^= 0xfe;
            tag[2] ^= aad_len >> 24;
            tag[3] ^= aad_len >> 16;
            tag[4] ^= aad_len >> 8;
            tag[5] ^= aad_len;
            start = 6;
        } else {
            tag[0] ^= 0xff;
            tag[1] ^= 0xff;
            tag[2] ^= aad_len >> 56;
            tag[3] ^= aad_len >> 48;
            tag[4] ^= aad_len >> 40;
            tag[5] ^= aad_len >> 32;
            tag[6] ^= aad_len >> 24;
            tag[7] ^= aad_len >> 16;
            tag[8] ^= aad_len >> 8;
            tag[9] ^= aad_len;
            start = 10;
        }
        ccm_cbc_mac(ctx, tag, aad, aad_len, start);
    }

    if (len > 0) {
		ccm_cbc_mac(ctx, tag, in, len, 0);
	}

    // ANCHOR: Encryption

    block[0] = l - 1;

    

    return true;
}

}

#endif AES_CCM
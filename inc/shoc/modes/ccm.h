#ifndef SHOC_CCM_H
#define SHOC_CCM_H

#include "shoc/modes/cbc_mac.h"
#include "shoc/modes/ctr.h"

namespace shoc {

template<class E, size_t L>
inline void ccm_ctr(E &ciph, byte *a_0, const byte *nonce, const byte *in, byte *out, size_t len)
{
    static_assert(L > 1 && L < 9, "invalid length field size");

    constexpr size_t N = 15 - L;    // Nonce length
    constexpr size_t L_IDX = N + 1; // Start of length field 

    a_0[0] = L - 1;
    copy(&a_0[1], nonce, N);
    fill(&a_0[L_IDX], 0, L);

    byte buf[16];
    byte a_i[16];
    copy(a_i, a_0, 16);

    auto end = out + len;
    auto idx = 0;
    
    while (out < end) {
        if ((idx &= 0xf) == 0) {
            incc<L>(a_i);
            ciph.encrypt(a_i, buf);
        }
        *out++ = buf[idx++] ^ *in++;
    }

    fill(a_0 + L_IDX, 0, L);
    ciph.encrypt(a_0, a_0);
}

template<class E, size_t L>
inline void ccm_auth(E &ciph, byte *block, const byte *nonce, const byte *in, size_t len, const byte *aad, size_t aad_len, size_t tag_len)
{
    static_assert(L > 1 && L < 9, "invalid length field size");

    constexpr size_t N = 15 - L; // Nonce length

    block[0] =  (aad_len ? 0x40 : 0x00)     | 
                (((tag_len - 2) / 2) << 3)  |
                (L - 1);

    copy(&block[1], nonce, N);

    for (size_t 
            i = N + 1, // Start of length field
            j = L - 1; 
        i < 16; ++i) 
    {
        block[i] = len >> (8 * j--);
    }
    ciph.encrypt(block, block);

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
        cbc_mac_padded(ciph, block, aad, aad_len, start);
    }

    if (len) 
        cbc_mac_padded(ciph, block, in, len, 0);
}

/**
 * @brief 
 * 
 * @tparam E 
 * @tparam L 
 * @param key 
 * @param nonce 
 * @param aad 
 * @param aad_len 
 * @param tag 
 * @param tag_len 
 * @param in 
 * @param out 
 * @param len 
 * @return true 
 * @return false 
 */
template<class E, size_t L = 2>
inline bool ccm_encrypt(
    const byte *key, 
    const byte *nonce,
    const byte *aad, size_t aad_len,
          byte *tag, size_t tag_len,
    const byte *in, 
          byte *out, size_t len)
{
    if (!len)
        return false;

    if (aad && !aad_len)
        return false;

    if (tag_len > 16 || 
        tag_len < 4  || 
        tag_len & 1)
        return false;

    E ciph {key};

    byte block[16];

    ccm_auth<L>(ciph, block, nonce, in, len, aad, aad_len, tag_len);
    copy(tag, block, tag_len);
    ccm_ctr<L>(ciph, block, nonce, in, out, len);
    xorb(tag, block, tag_len);

    return true;
}

/**
 * @brief 
 * 
 * @tparam E 
 * @tparam L 
 * @param key 
 * @param nonce 
 * @param aad 
 * @param aad_len 
 * @param tag 
 * @param tag_len 
 * @param in 
 * @param out 
 * @param len 
 * @return true 
 * @return false 
 */
template<class E, size_t L = 2>
inline bool ccm_decrypt(
    const byte *key, 
    const byte *nonce,
    const byte *aad, size_t aad_len,
    const byte *tag, size_t tag_len,
    const byte *in,
          byte *out, size_t len)
{
    if (!len)
        return false;

    if (aad && !aad_len)
        return false;

    if (tag_len > 16 || 
        tag_len < 4  || 
        tag_len & 1)
        return false;

    E ciph {key};

    byte block[16];
    byte mac[16];

    ccm_ctr<L>(ciph, block, nonce, in, out, len);

    for (size_t i = 0; i < tag_len; ++i)
        mac[i] = block[i] ^ tag[i];

    ccm_auth<L>(ciph, block, nonce, in, len, aad, aad_len, tag_len);

    if (memcmp(mac, block, tag_len)) {
        zero(out, len);
        return false;
    }
    return true;
}

}

#endif
#ifndef SHOC_MODE_GCM_H
#define SHOC_MODE_GCM_H

#include "shoc/mode/ctr.h"

namespace shoc {

/**
 * @brief Multiplication in Galois field 2^128. 
 * 
 * @param x First 128-bit vector multiplicand
 * @param y Second 128-bit vector multiplier
 * @param z Output 128-bit vector
 */
constexpr void gmul(const byte *x, const byte *y, byte *z)
{
    byte v[16];

    zero(z, 16);
    copy(v, y, 16);

    for (int i = 0; i < 16; ++i) {
        for (int j = 7; j >= 0; --j) {
            if (get_bit(x[i], j)) {
                xorb(z, v);
            }
            if (get_bit(v[15], 0)) {
                shift_right_reflected(v);
                v[0] ^= 0xe1;
            } else {
                shift_right_reflected(v);
            }
        }
    }
}

constexpr void ghash(const byte *h, const byte *x, size_t x_len, byte *y)
{
    byte t[16];
    auto blocks = x_len >> 4;
    auto remain = x_len & 0xf; 

    for (size_t i = 0; i < blocks; ++i, x += 16) {
        xorb(y, x);
        gmul(y, h, t);
        copy(y, t, 16);
    }

    if (remain) {
        copy(t, x, remain);
        zero(t + remain, 16 - remain);
        xorb(y, t);
        gmul(y, h, t);
		copy(y, t, 16);
    }
}

// inline void gcm_ghash(
//     const byte *h, 
//     const byte *aad, size_t aad_len, 
//     const byte *txt, size_t txt_len,
//     byte *out)
// {
//     zero(out, 16);

//     ghash(h, aad, aad_len, out);
//     ghash(h, txt, txt_len, out);
// }

/**
 * @brief Encrypt with block cipher in Galois counter mode. All pointers MUST be 
 * valid when relevant lenth is not 0. Tag length MUST be {4, 8, 12, 13, 14, 15, 16}:
 * https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38d.pdf
 * 
 * @tparam E BLock cipher
 * @param key Key
 * @param iv Initial vector
 * @param iv_len Initial vector length
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag Output tag
 * @param tag_len Output tag length
 * @param in Plain text
 * @param out Cipher text
 * @param len Text length
 */
template<class E>
inline bool gcm_encrypt(
    const byte *key,
    const byte *iv,  size_t iv_len,
    const byte *aad, size_t aad_len,
          byte *tag, size_t tag_len,
    const byte *in,
          byte *out, size_t len)
{
    if (tag_len > 16 || 
        tag_len < 4  || (tag_len < 12 && tag_len & 3))
        return false;

    // 1. Init hash subkey

    byte h[16] = {};

    E ciph {key};

    ciph.encrypt(h, h);

    // 2. Prepare J0

    byte j0[16] = {};

    if (iv_len == 12) {
        copy(j0, iv, 12);
        j0[15] = 0x01;
    } else {
        byte pad[16] = {};
        auto pad_len = 16 - (iv_len & 0xf);
        ghash(h, iv, iv_len, j0);
        ghash(h, pad, pad_len, j0);
    }

    // 3. Increment J0 and pass to GCTR

    byte ij0[16];

    copy(ij0, j0, 16);
    incc(ij0);
    ctrf(ij0, in, out, len, ciph);

    // 4. Apply GHASH to [pad(aad) + pad(ciphertext) + 64-bit(aad_len * 8), 64-bit(len * 8))]

    byte s[16] = {};
    byte len_buf[16];

    putbe(uint64_t(aad_len) * 8, len_buf);
    putbe(uint64_t(len) * 8, len_buf + 8);

    ghash(h, aad, aad_len, s);
    ghash(h, out, len, s);
    ghash(h, len_buf, 16, s);

    // 5. Generate tag and truncate to desired length

    byte t[16];

    ctrf(j0, s, t, 16, ciph);
    copy(tag, t, tag_len);

    return true;
}

template<class E>
inline bool gcm_decrypt(
    const byte *key,
    const byte *iv,  size_t iv_len,
    const byte *aad, size_t aad_len,
    const byte *tag, size_t tag_len,
    const byte *in,
          byte *out, size_t len)
{
    if (tag_len > 16 || 
        tag_len < 4  || (tag_len < 12 && tag_len & 3))
        return false;

    
}

}

#endif
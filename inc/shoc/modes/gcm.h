#ifndef SHOC_GCM_H
#define SHOC_GCM_H

#include "shoc/_util.h"

namespace shoc {

/**
 * @brief Multiplication in Galois field 2^128. 
 * 
 * @param x First 128-bit vector multiplicand
 * @param y Second 128-bit vector multiplier
 * @param z Output 128-bit vector
 */
inline void gmul(const byte *x, const byte *y, byte *z)
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

inline void ghash(const byte *h, const byte *x, size_t x_len, byte *y)
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

template<class E>
inline void gctr(const byte *icb, const byte *x, size_t x_len, byte *y, E &ciph)
{
    byte cb[16];
    byte tmp[16];
    auto blocks = x_len >> 4;
    auto remain = x_len & 0xf;

    copy(cb, icb, 16);

    for (size_t i = 0; i < blocks; ++i, x += 16, y += 16) {
        ciph.encrypt(cb, y);
        xorb(y, x);
        incc(cb);
    }

    if (remain) {
        ciph.encrypt(cb, tmp);
        for (size_t i = 0; i < remain; ++i)
			*y++ = *x++ ^ tmp[i];
    }
}

template<class E>
inline void gctr_2(const byte *icb, const byte *x, size_t x_len, byte *y, E &ciph)
{
    byte cb[16];
    byte tmp[16];

    copy(cb, icb, 16);

    auto end = y + x_len;
    auto idx = 0;

    while (y < end) {
        if ((idx &= 0xf) == 0) {
            ciph.encrypt(cb, tmp);
            incc(cb);
        }
        *y++ = tmp[idx++] ^ *x++;
    }
}

inline void gcm_ghash(
    const byte *h, 
    const byte *aad, size_t aad_len, 
    const byte *txt, size_t txt_len,
    byte *out)
{
    zero(out, 16);

    ghash(h, aad, aad_len, out);
    ghash(h, txt, txt_len, out);
}

template<class E>
inline void gcm_encrypt(
    const byte *key,
    const byte *iv, size_t iv_len,
    const byte *aad, size_t aad_len,
    const byte *in, size_t len,
    byte *out,
    byte *tag, size_t tag_len)
{
    // 1. Init hash subkey

    byte h[16] = {};
    byte y[16];

    E ciph {key};

    ciph.encrypt(h, h);

    // 2. Prepare J0

    byte j0[16] = {};
    byte pad[16] = {};
    auto pad_len = 16 - (iv_len & 0xf);

    if (iv_len == 12) {
        copy(j0, in, 12);
        j0[15] = 0x01;
    } else {
        ghash(h, iv, iv_len, j0);
        ghash(h, pad, pad_len, j0);
    }

    // 3. Increment J0 and pass to GCTR

    byte ij0[16];

    copy(ij0, j0, 16);
    incc(ij0);
    gctr(ij0, in, len, ciph);

    // 4.

    
}

}

#endif
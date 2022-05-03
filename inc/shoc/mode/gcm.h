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

inline void gcm_ghash(
    const byte *h, 
    const byte *aad, size_t aad_len, 
    const byte *txt, size_t txt_len,
    byte *out)
{
    // Apply GHASH to [pad(aad) + pad(ciphertext) + 64-bit(aad_len * 8), 64-bit(len * 8))]

    byte len[16];

    zero(out, 16);

    putbe(uint64_t(aad_len * 8), len);
    putbe(uint64_t(txt_len * 8), len + 8);

    ghash(h, aad, aad_len, out);
    ghash(h, txt, txt_len, out);
    ghash(h, len, 16, out);
}

template<class E>
inline void gcm_init(const byte *iv, size_t iv_len, byte *h, byte *j0, E &ciph)
{
    // Init hash subkey

    zero(h, 16);
    ciph.encrypt(h, h);

    // Prepare J0

    zero(j0, 16);

    if (iv_len == 12) {
        copy(j0, iv, 12);
        j0[15] = 0x01;
    } else {

        // NOTE: I'm pretty sure the GCM paper has error on Page 15, Algorithm 4: GCM-AE, Step 2:
        // 
        // "Otherwise, the IV is padded with the minimum number of '0' bits, possibly none, so 
        // that the length of the resulting string is a multiple of 128 bits (the block size);
        // this string in turn is appended with 64 additional '0' bits, followed by the 64-bit 
        // representation of the length of the IV"
        // 
        // https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38d.pdf
        //
        // So, first padding with '0' till (N*128)-bits before 64-bit ['0'] and [iv_len] blocks 
        // is done in code commented below, but it gives incorrect results with test vectors.

        // byte pad[16] = {};
        // auto pad_len = 16 - (iv_len & 0xf);
        // ghash(h, iv, iv_len, j0);
        // ghash(h, pad, pad_len, j0);
        // putbe(uint64_t(iv_len * 8), pad + 8);
        // ghash(h, pad, 16, j0);

        byte pad[16] = {};
        putbe(uint64_t(iv_len * 8), pad + 8);
        ghash(h, iv, iv_len, j0);
        ghash(h, pad, 16, j0);
    }
}

template<class E>
inline void gcm_gctr(const byte *j0, const byte *in, byte *out, size_t len, E &ciph)
{
    // Increment J0 and pass to GCTR

    byte ij0[16];

    copy(ij0, j0, 16);
    incc(ij0);
    ctrf(ij0, in, out, len, ciph);
}

/**
 * @brief Encrypt with block cipher in Galois counter mode. All pointers MUST be 
 * valid when relevant length is not 0. Tag length MUST be {4, 8, 12, 13, 14, 15, 16}:
 * https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38d.pdf
 * 
 * @tparam E BLock cipher
 * @param key Key
 * @param iv Initial vector
 * @param iv_len Initial vector length
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag Output tag
 * @param tag_len Output tag desired length
 * @param in Plain text
 * @param out Cipher text
 * @param len Text length
 * @return true on success, false if tag length is invalid
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

    E ciph {key};

    byte h[16];
    byte j[16];
    byte s[16];
    byte t[16];

    gcm_init(iv, iv_len, h, j, ciph);           // 1. Init hash subkey and prepare J0
    gcm_gctr(j, in, out, len, ciph);            // 2. Increment J0 and pass to GCTR
    gcm_ghash(h, aad, aad_len, out, len, s);    // 3. Apply GHASH to [pad(aad) + pad(ciphertext) + 64-bit(aad_len * 8), 64-bit(len * 8))]
    ctrf(j, s, t, sizeof(t), ciph);             // 4. Generate full tag
    copy(tag, t, tag_len);                      // 5. Truncate tag to the desired length

    return true;
}

/**
 * @brief Decrypt with block cipher in Galois counter mode. All pointers MUST be 
 * valid when relevant length is not 0. Tag length MUST be {4, 8, 12, 13, 14, 15, 16}:
 * https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38d.pdf
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param iv_len Initial vector length
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag Input tag
 * @param tag_len Input tag length
 * @param in Cipher text
 * @param out Plain text
 * @param len Text length
 * @return true on success, false if tag length is invalid or authentication failed
 */
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

    E ciph {key};

    byte h[16];
    byte j[16];
    byte s[16];
    byte t[16];

    gcm_init(iv, iv_len, h, j, ciph);           // 1. Init hash subkey and prepare J0
    gcm_ghash(h, aad, aad_len, out, len, s);    // 2. Apply GHASH to [pad(aad) + pad(ciphertext) + 64-bit(aad_len * 8), 64-bit(len * 8))]
    gcm_gctr(j, in, out, len, ciph);            // 3. Increment J0 and pass to GCTR
    ctrf(j, s, t, sizeof(t), ciph);             // 4. Generate full tag

    if (memcmp(t, tag, tag_len)) {              // 5. Compare tag with the given
        zero(out, len);
        return false;
    }
    return true;
}

}

#endif
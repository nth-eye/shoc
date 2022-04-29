#ifndef SHOC_MODE_CCM_H
#define SHOC_MODE_CCM_H

#include "shoc/mac/cbc_mac.h"
#include "shoc/mode/ctr.h"

namespace shoc {

/**
 * @brief Basic counter mode function, has small differences from ctrf(...) 
 * and used specifically in CCM.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size
 * @param ciph Cipher object, must be already initialized
 * @param a_0 Initial block
 * @param nonce Nonce
 * @param in Data input
 * @param out Data output
 * @param len Data length
 */
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

    for (size_t i = 0; i < len; ++i) {
        size_t idx = i & 0xf;
        if (idx == 0) {
            incc<L>(a_i);
            ciph.encrypt(a_i, buf);
        }
        *out++ = buf[idx] ^ *in++;
    }
    fill(a_0 + L_IDX, 0, L);
    ciph.encrypt(a_0, a_0);
}

/**
 * @brief CCM authentication function. Used internally in CCM encrypt/decrypt process.
 * 
 * @tparam E Block cipher 
 * @tparam L Counter size
 * @param ciph  Cipher object, must be already initialized
 * @param block Initial block
 * @param nonce Nonce
 * @param in Input data
 * @param len Input length
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag_len Tag length, used to construct initial block
 */
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
 * @brief Encrypt with block cipher in counter with CBC-MAC mode. 
 * Number of counter-bytes is configurable. All pointers MUST be valid, 
 * except when relevant length is 0.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size, default is 2
 * @param key Key
 * @param nonce Nonce, MUST be of length 15 - L
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag Output tag
 * @param tag_len Output tag desired length
 * @param in Plain text
 * @param out Cipher txt
 * @param len Text length
 * @return true on success, false if tag length is invalid
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
    if (tag_len > 16 || 
        tag_len < 4  || 
        tag_len & 1)
        return false;

    E ciph {key};

    byte block[16];

    ccm_auth<E, L>(ciph, block, nonce, in, len, aad, aad_len, tag_len);
    copy(tag, block, tag_len);
    ccm_ctr<E, L>(ciph, block, nonce, in, out, len);
    xorb(tag, block, tag_len);

    return true;
}

/**
 * @brief Decrypt with block cipher in counter with CBC-MAC mode. 
 * Number of counter-bytes is configurable. All pointers MUST be valid, 
 * except when relevant length is 0.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size, default is 2
 * @param key Key
 * @param nonce Nonce, MUST be of length 15 - L
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag Input tag
 * @param tag_len Input tag length
 * @param in Cipher text
 * @param out Plain txt
 * @param len Text length
 * @return true on success, false if tag length is invalid or authentication failed
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
    if (tag_len > 16 || 
        tag_len < 4  || 
        tag_len & 1)
        return false;

    E ciph {key};

    byte block[16];
    byte mac[16];

    ccm_ctr<E, L>(ciph, block, nonce, in, out, len);

    for (size_t i = 0; i < tag_len; ++i)
        mac[i] = block[i] ^ tag[i];

    ccm_auth<E, L>(ciph, block, nonce, in, len, aad, aad_len, tag_len);

    if (memcmp(mac, block, tag_len)) {
        zero(out, len);
        return false;
    }
    return true;
}

}

#endif
#ifndef SHOC_MODE_CCM_H
#define SHOC_MODE_CCM_H

#include "shoc/mac/cbc_mac.h"

namespace shoc {

/**
 * @brief Check if CCM tag length is invalid.
 * 
 * @param len Tag length
 * @return true if invalid
 */
constexpr bool ccm_tag_length_invalid(size_t len)
{
    return len > 16 || len < 4 || len & 1;
}

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
constexpr void ccm_ctr(E& ciph, 
    span_o<16> a_0, 
    span_i<15 - L> nonce, 
    const byte* in, 
          byte* out, size_t len)
{
    static_assert(L > 1 && L < 9, "invalid length field size");
    static_assert(E::block_size == 16, "CCM mode defined only for 128-bit block ciphers");

    a_0[0] = L - 1;
    copy(&a_0[1], nonce.data(), nonce.size());
    fill(&a_0[nonce.size() + 1], 0, L);

    byte buf[16];
    byte a_i[16];
    copy(a_i, a_0.data(), a_0.size());

    for (size_t i = 0; i < len; ++i) {
        size_t idx = i & 0xf;
        if (idx == 0) {
            incc<L>(a_i);
            ciph.encrypt(a_i, buf);
        }
        *out++ = buf[idx] ^ *in++;
    }
    fill(&a_0[nonce.size() + 1], 0, L);
    ciph.encrypt(a_0, a_0);
}

/**
 * @brief CCM authentication function. Used internally in CCM encrypt/decrypt process.
 * 
 * @tparam E Block cipher 
 * @tparam L Counter size
 * @param ciph Cipher object, must be already initialized
 * @param block Initial block
 * @param nonce Nonce
 * @param in Input data
 * @param aad Additional authenticated data
 * @param tag_len Tag length, used to construct initial block
 */
template<class E, size_t L>
constexpr void ccm_auth(E& ciph, 
    span_o<16> block,
    span_i<15 - L> nonce, 
    span_i<> in,
    span_i<> aad,
    size_t tag_len)
{
    static_assert(L > 1 && L < 9, "invalid length field size");
    static_assert(E::block_size == 16, "CCM mode defined only for 128-bit block ciphers");

    block[0] =  (aad.size() ? 0x40 : 0x00)  | 
                (((tag_len - 2) / 2) << 3)  |
                (L - 1);
    copy(&block[1], nonce.data(), nonce.size());

    for (size_t i = nonce.size() + 1, j = L - 1; i < 16; ++i)
        block[i] = in.size() >> (8 * j--);

    ciph.encrypt(block, block);

    if (aad.size()) {
        size_t start;
        if (aad.size() < 65536 - 256) {
            block[0] ^= aad.size() >> 8;
            block[1] ^= aad.size();
            start = 2;
        } else if (aad.size() < 4294967296) {
            block[0] ^= 0xff;
            block[1] ^= 0xfe;
            block[2] ^= aad.size() >> 24;
            block[3] ^= aad.size() >> 16;
            block[4] ^= aad.size() >> 8;
            block[5] ^= aad.size();
            start = 6;
        } else {
            block[0] ^= 0xff;
            block[1] ^= 0xff;
            block[2] ^= aad.size() >> 56;
            block[3] ^= aad.size() >> 48;
            block[4] ^= aad.size() >> 40;
            block[5] ^= aad.size() >> 32;
            block[6] ^= aad.size() >> 24;
            block[7] ^= aad.size() >> 16;
            block[8] ^= aad.size() >> 8;
            block[9] ^= aad.size();
            start = 10;
        }
        cbc_mac_padded(ciph, block, aad, start);
    }

    if (in.size()) 
        cbc_mac_padded(ciph, block, in, 0);
}

/**
 * @brief Encrypt with block cipher in counter with CBC-MAC mode. 
 * Number of counter-bytes is configurable. All pointers MUST be valid, 
 * except when relevant length is 0. Text length can be arbitrary.
 * 
 * @tparam E Block cipher
 * @tparam L Counter size, default is 2
 * @param key Key
 * @param nonce Nonce, MUST be of length 15 - L
 * @param aad Additional authenticated data
 * @param aad.size() Additional authenticated data length
 * @param tag Output tag
 * @param in Plain text
 * @param out Cipher txt
 * @param len Text length
 * @return true on success, false if tag length is invalid
 */
template<class E, size_t L = 2>
constexpr bool ccm_encrypt(
    span_i<E::key_size> key, 
    span_i<15 - L> nonce,
    span_i<> aad,
    span_o<> tag,
    const byte *in, 
          byte *out, size_t len)
{
    if (ccm_tag_length_invalid(tag.size()))
        return false;

    E ciph {key};
    byte block[16];

    ccm_auth<E, L>(ciph, block, nonce, {in, len}, aad, tag.size());
    copy(tag.data(), block, tag.size());
    ccm_ctr<E, L>(ciph, block, nonce, in, out, len);
    xorb(tag.data(), block, tag.size());

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
 * @param tag Input tag
 * @param in Cipher text
 * @param out Plain txt
 * @param len Text length
 * @return true on success, false if tag length is invalid or authentication failed
 */
template<class E, size_t L = 2>
constexpr bool ccm_decrypt(
    span_i<E::key_size> key, 
    span_i<15 - L> nonce,
    span_i<> aad, 
    span_i<> tag,
    const byte *in,
          byte *out, size_t len)
{
    if (ccm_tag_length_invalid(tag.size()))
        return false;

    E ciph {key};
    byte block[16];
    byte mac[16];

    ccm_ctr<E, L>(ciph, block, nonce, in, out, len);

    for (size_t i = 0; i < tag.size(); ++i)
        mac[i] = block[i] ^ tag[i];

    ccm_auth<E, L>(ciph, block, nonce, {out, len}, aad, tag.size());

    if (memcmp(mac, block, tag.size())) {
        zero(out, len);
        return false;
    }
    return true;
}

}

#endif
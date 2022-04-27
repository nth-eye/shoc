#ifndef SHOC_AES_GCM_H
#define SHOC_AES_GCM_H

#include "shoc/aes.h"

namespace shoc {

constexpr void xor_block(byte *x, const byte *y)
{
    for (int i = 0; i < 16; ++i)
        x[i] ^= y[i];
}

#define WPA_GET_BE32(a) ((((uint32_t) (a)[0]) << 24) | (((uint32_t) (a)[1]) << 16) | \
			 (((uint32_t) (a)[2]) << 8) | ((uint32_t) (a)[3]))

#define WPA_PUT_BE32(a, val) 	                        \
    do {                                                \
        (a)[0] = (byte) ((((uint32_t) (val)) >> 24) & 0xff);   \
        (a)[1] = (byte) ((((uint32_t) (val)) >> 16) & 0xff);   \
        (a)[2] = (byte) ((((uint32_t) (val)) >> 8) & 0xff);    \
        (a)[3] = (byte) (((uint32_t) (val)) & 0xff);           \
    } while (0)
        
inline void shift_right_block(byte *v)
{
	uint32_t val;
	val = WPA_GET_BE32(v + 12);
	val >>= 1;
	if (v[11] & 0x01)
		val |= 0x80000000;
	WPA_PUT_BE32(v + 12, val);
	val = WPA_GET_BE32(v + 8);
	val >>= 1;
	if (v[7] & 0x01)
		val |= 0x80000000;
	WPA_PUT_BE32(v + 8, val);
	val = WPA_GET_BE32(v + 4);
	val >>= 1;
	if (v[3] & 0x01)
		val |= 0x80000000;
	WPA_PUT_BE32(v + 4, val);
	val = WPA_GET_BE32(v);
	val >>= 1;
	WPA_PUT_BE32(v, val);
}

/**
 * @brief Multiplication in Galois field 2 ^ 128. 
 * 
 * @param x First 128-bit vector multiplicand
 * @param y Second 128-bit vector multiplier
 * @param z Output 128-bit vector
 */
inline void gf128_mul_1(const byte *x, const byte *y, byte *z)
{
    byte v[16];

    zero(z,     16);
    copy(v, y,  16);

    for (int i = 0; i < 128; ++i) {
        if (get_arr_bit(x, i))
            xor_block(z, v);
        if (get_arr_bit(v, 127)) {
            shift_from_lsb(v, sizeof(v));
            v[0] ^= 0b10000111;
        } else {
            shift_from_lsb(v, sizeof(v));
        }
    }
}

/**
 * @brief Multiplication in Galois field 2 ^ 128. 
 * 
 * @param x First 128-bit vector multiplicand
 * @param y Second 128-bit vector multiplier
 * @param z Output 128-bit vector
 */
inline void gf128_mul_2(const byte *x, const byte *y, byte *z)
{
    byte v[16];

    zero(z,     16);
    copy(v, y,  16);

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (get_bit(x[i], 7 - j)) {
                xor_block(z, v);
            }
            if (get_bit(v[15], 0)) {
                shift_right_block(v);
                v[0] ^= 0xe1;
            } else {
                shift_right_block(v);
            }
        }
    }
}

/**
 * @brief Multiplication in Galois field 2 ^ 128. 
 * 
 * @param x First 128-bit vector multiplicand
 * @param y Second 128-bit vector multiplier
 * @param z Output 128-bit vector
 */
inline void gf128_mul_3(const byte *x, const byte *y, byte *z)
{
    byte v[16];

	zero(z,     16);
	copy(v, y,  16);

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            if (x[i] & bit(7 - j)) {
                xor_block(z, v);
            }
            if (v[15] & 0x01) {
                shift_right_block(v);
                v[0] ^= 0xe1;
            } else {
                shift_right_block(v);
            }
        }
    }
}

inline void ghash(byte *y, size_t len)
{
    // byte tmp[16];
    // auto blocks = len >> 4;

    // for (int i = 0; i < blocks; ++i) {

    //     gf128_mul_3(y, h, tmp);
    //     copy(y, tmp, 16);
    // }
}

// inline bool gcm_encrypt(
//     const byte *key, 
//     const byte *iv, size_t iv_len,
//     const byte *aad, size_t aad_len,
//     const byte *in, size_t len,
//     byte *out,
//     byte *tag, size_t tag_len)
// {
    
// }

// inline bool gcm_decrypt(const byte *key, )
// {

// }

}

#endif
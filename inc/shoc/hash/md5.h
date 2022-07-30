#ifndef SHOC_HASH_MD5_H
#define SHOC_HASH_MD5_H

#include "shoc/util.h"

namespace shoc {

struct Md5 : Eater<Md5> {
    static constexpr size_t SIZE        = 16;
    static constexpr size_t STATE_SIZE  = 4;    // In words
    static constexpr size_t BLOCK_SIZE  = 64;   // In bytes
public:
    void init();
    void feed(const void *in, size_t len);
    void stop(byte *out);
private:
    void pad();
    void step();
private:
    using word = uint32_t;
private:  
    word length_low;
    word length_high;
    word state[STATE_SIZE];
    byte block[BLOCK_SIZE];
    byte block_idx;
};

inline void Md5::init()
{
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;

    block_idx = length_high = length_low = 0;
}

inline void Md5::feed(const void *in, size_t len)
{
    assert(in || !len);

    auto p = static_cast<const byte*>(in);

    while (len--) {

        block[block_idx++] = *p++;

        if ((length_low += 8) == 0)
            length_high += 1;
        if (block_idx == BLOCK_SIZE)
            step();
    }
}

inline void Md5::stop(byte *out)
{
    assert(out);
    pad();

    for (size_t i = 0, j = 0; i < STATE_SIZE; ++i, j += 4) {
        out[j + 0] = state[i] >> 0;
        out[j + 1] = state[i] >> 8;
        out[j + 2] = state[i] >> 16;
        out[j + 3] = state[i] >> 24;
    }
    zero(this, sizeof(*this));
}

inline void Md5::pad()
{
    static constexpr size_t pad_start = BLOCK_SIZE - 8;

    block[block_idx++] = 0x80;

    if (block_idx > pad_start) {
        fill(block + block_idx, 0, BLOCK_SIZE - block_idx);
        step();
    }
    fill(block + block_idx, 0, pad_start - block_idx);

    for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
        block[BLOCK_SIZE - 8 + i] = length_low  >> j;
        block[BLOCK_SIZE - 4 + i] = length_high >> j;
    }
    step();
}

inline void Md5::step()
{
    word a = state[0];
    word b = state[1];
    word c = state[2];
    word d = state[3];
    word buf[16];

    for (size_t i = 0, j = 0; j < BLOCK_SIZE; ++i, j +=4) {
        buf[i]  = block[j + 0] << 0;
        buf[i] |= block[j + 1] << 8;
        buf[i] |= block[j + 2] << 16;
        buf[i] |= block[j + 3] << 24;
    }

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) parity(x, y, z)
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define FF(a, b, c, d, x, s, ac)    \
    a += F(b, c, d) + x + ac;       \
    a = rol(a, s);                  \
    a += b;

#define GG(a, b, c, d, x, s, ac)    \
    a += G(b, c, d) + x + ac;       \
    a = rol(a, s);                  \
    a += b;

#define HH(a, b, c, d, x, s, ac)    \
    a += H(b, c, d) + x + ac;       \
    a = rol(a, s);                  \
    a += b;

#define II(a, b, c, d, x, s, ac)    \
    a += I(b, c, d) + x + ac;       \
    a = rol(a, s);                  \
    a += b;

    enum {
        S11 = 7,
        S12 = 12,
        S13 = 17,
        S14 = 22,
        S21 = 5,
        S22 = 9,
        S23 = 14,
        S24 = 20,
        S31 = 4,
        S32 = 11,
        S33 = 16,
        S34 = 23,
        S41 = 6,
        S42 = 10,
        S43 = 15,
        S44 = 21,
    };

    FF(a, b, c, d, buf[ 0], S11, 0xd76aa478);
    FF(d, a, b, c, buf[ 1], S12, 0xe8c7b756);
    FF(c, d, a, b, buf[ 2], S13, 0x242070db);
    FF(b, c, d, a, buf[ 3], S14, 0xc1bdceee);
    FF(a, b, c, d, buf[ 4], S11, 0xf57c0faf);
    FF(d, a, b, c, buf[ 5], S12, 0x4787c62a);
    FF(c, d, a, b, buf[ 6], S13, 0xa8304613);
    FF(b, c, d, a, buf[ 7], S14, 0xfd469501);
    FF(a, b, c, d, buf[ 8], S11, 0x698098d8);
    FF(d, a, b, c, buf[ 9], S12, 0x8b44f7af);
    FF(c, d, a, b, buf[10], S13, 0xffff5bb1);
    FF(b, c, d, a, buf[11], S14, 0x895cd7be);
    FF(a, b, c, d, buf[12], S11, 0x6b901122);
    FF(d, a, b, c, buf[13], S12, 0xfd987193);
    FF(c, d, a, b, buf[14], S13, 0xa679438e);
    FF(b, c, d, a, buf[15], S14, 0x49b40821);

    GG(a, b, c, d, buf[ 1], S21, 0xf61e2562);
    GG(d, a, b, c, buf[ 6], S22, 0xc040b340);
    GG(c, d, a, b, buf[11], S23, 0x265e5a51);
    GG(b, c, d, a, buf[ 0], S24, 0xe9b6c7aa);
    GG(a, b, c, d, buf[ 5], S21, 0xd62f105d);
    GG(d, a, b, c, buf[10], S22, 0x02441453);
    GG(c, d, a, b, buf[15], S23, 0xd8a1e681);
    GG(b, c, d, a, buf[ 4], S24, 0xe7d3fbc8);
    GG(a, b, c, d, buf[ 9], S21, 0x21e1cde6);
    GG(d, a, b, c, buf[14], S22, 0xc33707d6);
    GG(c, d, a, b, buf[ 3], S23, 0xf4d50d87);
    GG(b, c, d, a, buf[ 8], S24, 0x455a14ed);
    GG(a, b, c, d, buf[13], S21, 0xa9e3e905);
    GG(d, a, b, c, buf[ 2], S22, 0xfcefa3f8);
    GG(c, d, a, b, buf[ 7], S23, 0x676f02d9);
    GG(b, c, d, a, buf[12], S24, 0x8d2a4c8a);

    HH(a, b, c, d, buf[ 5], S31, 0xfffa3942);
    HH(d, a, b, c, buf[ 8], S32, 0x8771f681);
    HH(c, d, a, b, buf[11], S33, 0x6d9d6122);
    HH(b, c, d, a, buf[14], S34, 0xfde5380c);
    HH(a, b, c, d, buf[ 1], S31, 0xa4beea44);
    HH(d, a, b, c, buf[ 4], S32, 0x4bdecfa9);
    HH(c, d, a, b, buf[ 7], S33, 0xf6bb4b60);
    HH(b, c, d, a, buf[10], S34, 0xbebfbc70);
    HH(a, b, c, d, buf[13], S31, 0x289b7ec6);
    HH(d, a, b, c, buf[ 0], S32, 0xeaa127fa);
    HH(c, d, a, b, buf[ 3], S33, 0xd4ef3085);
    HH(b, c, d, a, buf[ 6], S34, 0x04881d05);
    HH(a, b, c, d, buf[ 9], S31, 0xd9d4d039);
    HH(d, a, b, c, buf[12], S32, 0xe6db99e5);
    HH(c, d, a, b, buf[15], S33, 0x1fa27cf8);
    HH(b, c, d, a, buf[ 2], S34, 0xc4ac5665);

    II(a, b, c, d, buf[ 0], S41, 0xf4292244);
    II(d, a, b, c, buf[ 7], S42, 0x432aff97);
    II(c, d, a, b, buf[14], S43, 0xab9423a7);
    II(b, c, d, a, buf[ 5], S44, 0xfc93a039);
    II(a, b, c, d, buf[12], S41, 0x655b59c3);
    II(d, a, b, c, buf[ 3], S42, 0x8f0ccc92);
    II(c, d, a, b, buf[10], S43, 0xffeff47d);
    II(b, c, d, a, buf[ 1], S44, 0x85845dd1);
    II(a, b, c, d, buf[ 8], S41, 0x6fa87e4f);
    II(d, a, b, c, buf[15], S42, 0xfe2ce6e0);
    II(c, d, a, b, buf[ 6], S43, 0xa3014314);
    II(b, c, d, a, buf[13], S44, 0x4e0811a1);
    II(a, b, c, d, buf[ 4], S41, 0xf7537e82);
    II(d, a, b, c, buf[11], S42, 0xbd3af235);
    II(c, d, a, b, buf[ 2], S43, 0x2ad7d2bb);
    II(b, c, d, a, buf[ 9], S44, 0xeb86d391);

#undef F
#undef G
#undef H
#undef I
#undef FF
#undef GG
#undef HH
#undef II

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    block_idx = 0;
}

}

#endif
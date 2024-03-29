#ifndef SHOC_HASH_MD4_H
#define SHOC_HASH_MD4_H

#include "shoc/util.h"

namespace shoc {

struct Md4 : Eater<Md4> {
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

inline void Md4::init()
{
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;

    block_idx = length_high = length_low = 0;
}

inline void Md4::feed(const void *in, size_t len)
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

inline void Md4::stop(byte *out)
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

inline void Md4::pad()
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

inline void Md4::step()
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

#define FF(a, b, c, d, x, s)                \
    a += ch(b, c, d) + x;                   \
    a = rol(a, s);

#define GG(a, b, c, d, x, s)                \
    a += maj(b, c, d) + x + 0x5a827999;     \
    a = rol(a, s);

#define HH(a, b, c, d, x, s)                \
    a += parity(b, c, d) + x + 0x6ed9eba1;  \
    a = rol(a, s);

    enum { 
        S11 = 3,
        S12 = 7,
        S13 = 11,
        S14 = 19,
        S21 = 3,
        S22 = 5,
        S23 = 9,
        S24 = 13,
        S31 = 3,
        S32 = 9,
        S33 = 11,
        S34 = 15,
    };

    FF(a, b, c, d, buf[ 0], S11);
    FF(d, a, b, c, buf[ 1], S12);
    FF(c, d, a, b, buf[ 2], S13);
    FF(b, c, d, a, buf[ 3], S14);
    FF(a, b, c, d, buf[ 4], S11);
    FF(d, a, b, c, buf[ 5], S12);
    FF(c, d, a, b, buf[ 6], S13);
    FF(b, c, d, a, buf[ 7], S14);
    FF(a, b, c, d, buf[ 8], S11);
    FF(d, a, b, c, buf[ 9], S12);
    FF(c, d, a, b, buf[10], S13);
    FF(b, c, d, a, buf[11], S14);
    FF(a, b, c, d, buf[12], S11);
    FF(d, a, b, c, buf[13], S12);
    FF(c, d, a, b, buf[14], S13);
    FF(b, c, d, a, buf[15], S14);

    GG(a, b, c, d, buf[ 0], S21);
    GG(d, a, b, c, buf[ 4], S22);
    GG(c, d, a, b, buf[ 8], S23);
    GG(b, c, d, a, buf[12], S24);
    GG(a, b, c, d, buf[ 1], S21);
    GG(d, a, b, c, buf[ 5], S22);
    GG(c, d, a, b, buf[ 9], S23);
    GG(b, c, d, a, buf[13], S24);
    GG(a, b, c, d, buf[ 2], S21);
    GG(d, a, b, c, buf[ 6], S22);
    GG(c, d, a, b, buf[10], S23);
    GG(b, c, d, a, buf[14], S24);
    GG(a, b, c, d, buf[ 3], S21);
    GG(d, a, b, c, buf[ 7], S22);
    GG(c, d, a, b, buf[11], S23);
    GG(b, c, d, a, buf[15], S24);

    HH(a, b, c, d, buf[ 0], S31);
    HH(d, a, b, c, buf[ 8], S32);
    HH(c, d, a, b, buf[ 4], S33);
    HH(b, c, d, a, buf[12], S34);
    HH(a, b, c, d, buf[ 2], S31);
    HH(d, a, b, c, buf[10], S32);
    HH(c, d, a, b, buf[ 6], S33);
    HH(b, c, d, a, buf[14], S34);
    HH(a, b, c, d, buf[ 1], S31);
    HH(d, a, b, c, buf[ 9], S32);
    HH(c, d, a, b, buf[ 5], S33);
    HH(b, c, d, a, buf[13], S34);
    HH(a, b, c, d, buf[ 3], S31);
    HH(d, a, b, c, buf[11], S32);
    HH(c, d, a, b, buf[ 7], S33);
    HH(b, c, d, a, buf[15], S34);

#undef FF
#undef GG
#undef HH

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    block_idx = 0;
}

}

#endif
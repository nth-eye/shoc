#ifndef SHOC_HASH_MD4_H
#define SHOC_HASH_MD4_H

#include "shoc/util.h"

namespace shoc {

struct md4 : consumer<md4, 16> {
    static constexpr size_t state_size = 4;     // In words
    static constexpr size_t block_size = 64;    // In bytes
private:
    static constexpr size_t pad_start = block_size - 8;
public:
    constexpr void init();
    constexpr void feed(span_i<> in);
    constexpr void stop(span_o<hash_size> out);
private:
    constexpr void pad();
    constexpr void step();
private:
    using word = uint32_t;
private:    
    word length_low;
    word length_high;
    word state[state_size];
    byte block[block_size];
    byte block_idx;
};

constexpr void md4::init()
{
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;

    block_idx = length_high = length_low = 0;
}

constexpr void md4::feed(span_i<> in)
{
    for (auto it : in) {
        block[block_idx++] = it;
        if ((length_low += 8) == 0)
            length_high += 1;
        if (block_idx == block_size)
            step();
    }
}

constexpr void md4::stop(span_o<hash_size> out)
{
    pad();
    for (size_t i = 0, j = 0; i < state_size; ++i, j += 4) {
        out[j + 0] = state[i] >> 0;
        out[j + 1] = state[i] >> 8;
        out[j + 2] = state[i] >> 16;
        out[j + 3] = state[i] >> 24;
    }
    zero(this, sizeof(*this));
}

constexpr void md4::pad()
{
    block[block_idx++] = 0x80;

    if (block_idx > pad_start) {
        fill(block + block_idx, 0, block_size - block_idx);
        step();
    }
    fill(block + block_idx, 0, pad_start - block_idx);

    for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
        block[block_size - 8 + i] = length_low  >> j;
        block[block_size - 4 + i] = length_high >> j;
    }
    step();
}

constexpr void md4::step()
{
    word a = state[0];
    word b = state[1];
    word c = state[2];
    word d = state[3];
    word buf[16];

    for (size_t i = 0, j = 0; j < block_size; ++i, j +=4) {
        buf[i]  = block[j + 0] << 0;
        buf[i] |= block[j + 1] << 8;
        buf[i] |= block[j + 2] << 16;
        buf[i] |= block[j + 3] << 24;
    }
    auto ff = [] (word& a, word b, word c, word d, word x, int s) {
        a += ch(b, c, d) + x;
        a = rol(a, s);
    };
    auto gg = [] (word& a, word b, word c, word d, word x, int s) {
        a += maj(b, c, d) + x + 0x5a827999;
        a = rol(a, s);
    };
    auto hh = [] (word& a, word b, word c, word d, word x, int s) {
        a += parity(b, c, d) + x + 0x6ed9eba1;
        a = rol(a, s);
    };
    enum { 
        s11 = 3, s12 = 7, s13 = 11, s14 = 19,
        s21 = 3, s22 = 5, s23 = 9,  s24 = 13,
        s31 = 3, s32 = 9, s33 = 11, s34 = 15,
    };

    ff(a, b, c, d, buf[ 0], s11);
    ff(d, a, b, c, buf[ 1], s12);
    ff(c, d, a, b, buf[ 2], s13);
    ff(b, c, d, a, buf[ 3], s14);
    ff(a, b, c, d, buf[ 4], s11);
    ff(d, a, b, c, buf[ 5], s12);
    ff(c, d, a, b, buf[ 6], s13);
    ff(b, c, d, a, buf[ 7], s14);
    ff(a, b, c, d, buf[ 8], s11);
    ff(d, a, b, c, buf[ 9], s12);
    ff(c, d, a, b, buf[10], s13);
    ff(b, c, d, a, buf[11], s14);
    ff(a, b, c, d, buf[12], s11);
    ff(d, a, b, c, buf[13], s12);
    ff(c, d, a, b, buf[14], s13);
    ff(b, c, d, a, buf[15], s14);

    gg(a, b, c, d, buf[ 0], s21);
    gg(d, a, b, c, buf[ 4], s22);
    gg(c, d, a, b, buf[ 8], s23);
    gg(b, c, d, a, buf[12], s24);
    gg(a, b, c, d, buf[ 1], s21);
    gg(d, a, b, c, buf[ 5], s22);
    gg(c, d, a, b, buf[ 9], s23);
    gg(b, c, d, a, buf[13], s24);
    gg(a, b, c, d, buf[ 2], s21);
    gg(d, a, b, c, buf[ 6], s22);
    gg(c, d, a, b, buf[10], s23);
    gg(b, c, d, a, buf[14], s24);
    gg(a, b, c, d, buf[ 3], s21);
    gg(d, a, b, c, buf[ 7], s22);
    gg(c, d, a, b, buf[11], s23);
    gg(b, c, d, a, buf[15], s24);

    hh(a, b, c, d, buf[ 0], s31);
    hh(d, a, b, c, buf[ 8], s32);
    hh(c, d, a, b, buf[ 4], s33);
    hh(b, c, d, a, buf[12], s34);
    hh(a, b, c, d, buf[ 2], s31);
    hh(d, a, b, c, buf[10], s32);
    hh(c, d, a, b, buf[ 6], s33);
    hh(b, c, d, a, buf[14], s34);
    hh(a, b, c, d, buf[ 1], s31);
    hh(d, a, b, c, buf[ 9], s32);
    hh(c, d, a, b, buf[ 5], s33);
    hh(b, c, d, a, buf[13], s34);
    hh(a, b, c, d, buf[ 3], s31);
    hh(d, a, b, c, buf[11], s32);
    hh(c, d, a, b, buf[ 7], s33);
    hh(b, c, d, a, buf[15], s34);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    block_idx = 0;

    zero(buf, sizeof(buf));
}

}

#endif
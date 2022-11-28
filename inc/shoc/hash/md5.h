#ifndef SHOC_HASH_MD5_H
#define SHOC_HASH_MD5_H

#include "shoc/util.h"

namespace shoc {

struct md5 : consumer<md5, 16> {
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
    template<auto Fn>
    static constexpr void round(word& a, word b, word c, word d, word x, int s, word ac)
    {
        a += Fn(b, c, d) + x + ac;
        a = rol(a, s);
        a += b;
    }
private:  
    word length_low;
    word length_high;
    word state[state_size];
    byte block[block_size];
    byte block_idx;
};

constexpr void md5::init()
{
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;

    block_idx = length_high = length_low = 0;
}

constexpr void md5::feed(span_i<> in)
{
    for (auto it : in) {
        block[block_idx++] = it;
        if ((length_low += 8) == 0)
            length_high += 1;
        if (block_idx == block_size)
            step();
    }
}

constexpr void md5::stop(span_o<hash_size> out)
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

constexpr void md5::pad()
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

constexpr void md5::step()
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
    enum {
        s11 = 7, s12 = 12,  s13 = 17, s14 = 22,
        s21 = 5, s22 = 9,   s23 = 14, s24 = 20,
        s31 = 4, s32 = 11,  s33 = 16, s34 = 23,
        s41 = 6, s42 = 10,  s43 = 15, s44 = 21,
    };
    auto f = [] (word x, word y, word z) { return (x & y) | (~x & z); };
    auto g = [] (word x, word y, word z) { return (x & z) | (y & ~z); };
    auto h = [] (word x, word y, word z) { return parity(x, y, z); };
    auto i = [] (word x, word y, word z) { return y ^ (x | ~z); };

    round<f>(a, b, c, d, buf[ 0], s11, 0xd76aa478);
    round<f>(d, a, b, c, buf[ 1], s12, 0xe8c7b756);
    round<f>(c, d, a, b, buf[ 2], s13, 0x242070db);
    round<f>(b, c, d, a, buf[ 3], s14, 0xc1bdceee);
    round<f>(a, b, c, d, buf[ 4], s11, 0xf57c0faf);
    round<f>(d, a, b, c, buf[ 5], s12, 0x4787c62a);
    round<f>(c, d, a, b, buf[ 6], s13, 0xa8304613);
    round<f>(b, c, d, a, buf[ 7], s14, 0xfd469501);
    round<f>(a, b, c, d, buf[ 8], s11, 0x698098d8);
    round<f>(d, a, b, c, buf[ 9], s12, 0x8b44f7af);
    round<f>(c, d, a, b, buf[10], s13, 0xffff5bb1);
    round<f>(b, c, d, a, buf[11], s14, 0x895cd7be);
    round<f>(a, b, c, d, buf[12], s11, 0x6b901122);
    round<f>(d, a, b, c, buf[13], s12, 0xfd987193);
    round<f>(c, d, a, b, buf[14], s13, 0xa679438e);
    round<f>(b, c, d, a, buf[15], s14, 0x49b40821);

    round<g>(a, b, c, d, buf[ 1], s21, 0xf61e2562);
    round<g>(d, a, b, c, buf[ 6], s22, 0xc040b340);
    round<g>(c, d, a, b, buf[11], s23, 0x265e5a51);
    round<g>(b, c, d, a, buf[ 0], s24, 0xe9b6c7aa);
    round<g>(a, b, c, d, buf[ 5], s21, 0xd62f105d);
    round<g>(d, a, b, c, buf[10], s22, 0x02441453);
    round<g>(c, d, a, b, buf[15], s23, 0xd8a1e681);
    round<g>(b, c, d, a, buf[ 4], s24, 0xe7d3fbc8);
    round<g>(a, b, c, d, buf[ 9], s21, 0x21e1cde6);
    round<g>(d, a, b, c, buf[14], s22, 0xc33707d6);
    round<g>(c, d, a, b, buf[ 3], s23, 0xf4d50d87);
    round<g>(b, c, d, a, buf[ 8], s24, 0x455a14ed);
    round<g>(a, b, c, d, buf[13], s21, 0xa9e3e905);
    round<g>(d, a, b, c, buf[ 2], s22, 0xfcefa3f8);
    round<g>(c, d, a, b, buf[ 7], s23, 0x676f02d9);
    round<g>(b, c, d, a, buf[12], s24, 0x8d2a4c8a);

    round<h>(a, b, c, d, buf[ 5], s31, 0xfffa3942);
    round<h>(d, a, b, c, buf[ 8], s32, 0x8771f681);
    round<h>(c, d, a, b, buf[11], s33, 0x6d9d6122);
    round<h>(b, c, d, a, buf[14], s34, 0xfde5380c);
    round<h>(a, b, c, d, buf[ 1], s31, 0xa4beea44);
    round<h>(d, a, b, c, buf[ 4], s32, 0x4bdecfa9);
    round<h>(c, d, a, b, buf[ 7], s33, 0xf6bb4b60);
    round<h>(b, c, d, a, buf[10], s34, 0xbebfbc70);
    round<h>(a, b, c, d, buf[13], s31, 0x289b7ec6);
    round<h>(d, a, b, c, buf[ 0], s32, 0xeaa127fa);
    round<h>(c, d, a, b, buf[ 3], s33, 0xd4ef3085);
    round<h>(b, c, d, a, buf[ 6], s34, 0x04881d05);
    round<h>(a, b, c, d, buf[ 9], s31, 0xd9d4d039);
    round<h>(d, a, b, c, buf[12], s32, 0xe6db99e5);
    round<h>(c, d, a, b, buf[15], s33, 0x1fa27cf8);
    round<h>(b, c, d, a, buf[ 2], s34, 0xc4ac5665);

    round<i>(a, b, c, d, buf[ 0], s41, 0xf4292244);
    round<i>(d, a, b, c, buf[ 7], s42, 0x432aff97);
    round<i>(c, d, a, b, buf[14], s43, 0xab9423a7);
    round<i>(b, c, d, a, buf[ 5], s44, 0xfc93a039);
    round<i>(a, b, c, d, buf[12], s41, 0x655b59c3);
    round<i>(d, a, b, c, buf[ 3], s42, 0x8f0ccc92);
    round<i>(c, d, a, b, buf[10], s43, 0xffeff47d);
    round<i>(b, c, d, a, buf[ 1], s44, 0x85845dd1);
    round<i>(a, b, c, d, buf[ 8], s41, 0x6fa87e4f);
    round<i>(d, a, b, c, buf[15], s42, 0xfe2ce6e0);
    round<i>(c, d, a, b, buf[ 6], s43, 0xa3014314);
    round<i>(b, c, d, a, buf[13], s44, 0x4e0811a1);
    round<i>(a, b, c, d, buf[ 4], s41, 0xf7537e82);
    round<i>(d, a, b, c, buf[11], s42, 0xbd3af235);
    round<i>(c, d, a, b, buf[ 2], s43, 0x2ad7d2bb);
    round<i>(b, c, d, a, buf[ 9], s44, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    block_idx = 0;

    zero(buf, sizeof(buf));
}

}

#endif
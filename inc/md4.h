#ifndef MD4_H
#define MD4_H

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace creep {

struct MD4 {

    static constexpr size_t SIZE = 16;

    using byte = uint8_t;
    using word = uint32_t;

    void init();
    bool update(const void *in, size_t len);
    bool final(byte out[SIZE]);
    bool operator()(const void *in, size_t len, byte out[SIZE]);
private:
    void pad();
    void process();

    static constexpr size_t STATE_SIZE  = 4;    // In words
    static constexpr size_t BLOCK_SIZE  = 64;   // In bytes
    static constexpr size_t PAD_START   = BLOCK_SIZE - 8;

    static constexpr word rol(word x, int shift)            { return (x << shift) | (x >> (32 - shift)); }
    static constexpr word ch(word x, word y, word z)        { return (x & y) | (~x & z); }
    static constexpr word maj(word x, word y, word z)       { return (x & y) | (x & z) | (y & z); }
    static constexpr word parity(word x, word y, word z)    { return x ^ y ^ z; }
    
    word length_low;
    word length_high;
    word state[STATE_SIZE];
    byte block[BLOCK_SIZE];
    byte block_idx;
};

bool MD4::operator()(const void *in, size_t len, byte out[SIZE])
{
    init();
    return update(in, len) && final(out);
}

void MD4::init()
{
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;

    block_idx = length_high = length_low = 0;
}

bool MD4::update(const void *in, size_t len)
{
    if (!in)
        return false;

    auto p = static_cast<const uint8_t*>(in);

    while (len--) {

        block[block_idx++] = *p++;

        if ((length_low += 8) == 0)
            length_high += 1;

        if (block_idx == BLOCK_SIZE)
            process();
    }
    return true;
}

bool MD4::final(byte out[SIZE])
{
    if (!out)
        return false;

    pad();

    for (size_t i = 0, j = 0; i < STATE_SIZE; ++i, j += 4) {
        out[j + 0] = state[i] >> 0;
        out[j + 1] = state[i] >> 8;
        out[j + 2] = state[i] >> 16;
        out[j + 3] = state[i] >> 24;
    }
    memset(this, 0, sizeof(*this));

    return true;
}

void MD4::pad()
{
    block[block_idx++] = 0x80;

    if (block_idx > PAD_START) {
        memset(block + block_idx, 0, BLOCK_SIZE - block_idx);
        process();
    }
    memset(block + block_idx, 0, PAD_START - block_idx);

    for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
        block[BLOCK_SIZE - 8 + i] = length_low  >> j;
        block[BLOCK_SIZE - 4 + i] = length_high >> j;
    }
    process();
}

void MD4::process()
{
    enum { a, b, c, d };

    word var[STATE_SIZE];
    word buf[16];

    memcpy(var, state, sizeof(state));

    for (size_t i = 0, j = 0; j < BLOCK_SIZE; ++i, j +=4) {
        buf[i]  = block[j + 0] << 0;
        buf[i] |= block[j + 1] << 8;
        buf[i] |= block[j + 2] << 16;
        buf[i] |= block[j + 3] << 24;
    }

    #define FF(a, b, c, d, x, s) { \
        var[a] += ch(var[b], var[c], var[d]) + x; \
        var[a] = rol(var[a], s); \
    }
    #define GG(a, b, c, d, x, s) { \
        var[a] += maj(var[b], var[c], var[d]) + x + 0x5a827999; \
        var[a] = rol(var[a], s); \
    }
    #define HH(a, b, c, d, x, s) { \
        var[a] += parity(var[b], var[c], var[d]) + x + 0x6ed9eba1; \
        var[a] = rol(var[a], s); \
    }

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

    FF(a, b, c, d, buf[ 0], S11); /* 1 */
    FF(d, a, b, c, buf[ 1], S12); /* 2 */
    FF(c, d, a, b, buf[ 2], S13); /* 3 */
    FF(b, c, d, a, buf[ 3], S14); /* 4 */
    FF(a, b, c, d, buf[ 4], S11); /* 5 */
    FF(d, a, b, c, buf[ 5], S12); /* 6 */
    FF(c, d, a, b, buf[ 6], S13); /* 7 */
    FF(b, c, d, a, buf[ 7], S14); /* 8 */
    FF(a, b, c, d, buf[ 8], S11); /* 9 */
    FF(d, a, b, c, buf[ 9], S12); /* 10 */
    FF(c, d, a, b, buf[10], S13); /* 11 */
    FF(b, c, d, a, buf[11], S14); /* 12 */
    FF(a, b, c, d, buf[12], S11); /* 13 */
    FF(d, a, b, c, buf[13], S12); /* 14 */
    FF(c, d, a, b, buf[14], S13); /* 15 */
    FF(b, c, d, a, buf[15], S14); /* 16 */

    GG(a, b, c, d, buf[ 0], S21); /* 17 */
    GG(d, a, b, c, buf[ 4], S22); /* 18 */
    GG(c, d, a, b, buf[ 8], S23); /* 19 */
    GG(b, c, d, a, buf[12], S24); /* 20 */
    GG(a, b, c, d, buf[ 1], S21); /* 21 */
    GG(d, a, b, c, buf[ 5], S22); /* 22 */
    GG(c, d, a, b, buf[ 9], S23); /* 23 */
    GG(b, c, d, a, buf[13], S24); /* 24 */
    GG(a, b, c, d, buf[ 2], S21); /* 25 */
    GG(d, a, b, c, buf[ 6], S22); /* 26 */
    GG(c, d, a, b, buf[10], S23); /* 27 */
    GG(b, c, d, a, buf[14], S24); /* 28 */
    GG(a, b, c, d, buf[ 3], S21); /* 29 */
    GG(d, a, b, c, buf[ 7], S22); /* 30 */
    GG(c, d, a, b, buf[11], S23); /* 31 */
    GG(b, c, d, a, buf[15], S24); /* 32 */

    HH(a, b, c, d, buf[ 0], S31); /* 33 */
    HH(d, a, b, c, buf[ 8], S32); /* 34 */
    HH(c, d, a, b, buf[ 4], S33); /* 35 */
    HH(b, c, d, a, buf[12], S34); /* 36 */
    HH(a, b, c, d, buf[ 2], S31); /* 37 */
    HH(d, a, b, c, buf[10], S32); /* 38 */
    HH(c, d, a, b, buf[ 6], S33); /* 39 */
    HH(b, c, d, a, buf[14], S34); /* 40 */
    HH(a, b, c, d, buf[ 1], S31); /* 41 */
    HH(d, a, b, c, buf[ 9], S32); /* 42 */
    HH(c, d, a, b, buf[ 5], S33); /* 43 */
    HH(b, c, d, a, buf[13], S34); /* 44 */
    HH(a, b, c, d, buf[ 3], S31); /* 45 */
    HH(d, a, b, c, buf[11], S32); /* 46 */
    HH(c, d, a, b, buf[ 7], S33); /* 47 */
    HH(b, c, d, a, buf[15], S34); /* 48 */

    for (size_t i = 0; i < STATE_SIZE; ++i)
        state[i] += var[i];

    block_idx = 0;
}

}

#endif
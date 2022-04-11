#ifndef SHA1_H
#define SHA1_H

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace creep {

struct SHA1 {

    static constexpr size_t SIZE        = 20;
    static constexpr size_t STATE_SIZE  = 5;    // In words
    static constexpr size_t BLOCK_SIZE  = 64;   // In bytes
    static constexpr size_t PAD_START   = BLOCK_SIZE - 8;

    using byte = uint8_t;
    using word = uint32_t;

    void init();
    bool update(const void *in, size_t len);
    bool final(byte out[SIZE]);
    bool operator()(const void *in, size_t len, byte out[SIZE]);
private:
    void pad();
    void process();

    static constexpr word rol(word x, int shift)            { return (x << shift) | (x >> (32 - shift)); }
    static constexpr word ch(word x, word y, word z)        { return (x & y) ^ (~x & z); }
    static constexpr word maj(word x, word y, word z)       { return (x & y) ^ (x & z) ^ (y & z); }
    static constexpr word parity(word x, word y, word z)    { return x ^ y ^ z; }

    word state[STATE_SIZE];
    word length_low;
    word length_high;
    byte block[BLOCK_SIZE];
    byte block_idx;
};

inline void SHA1::init()
{
    state[0] = 0x67452301u;
    state[1] = 0xefcdab89u;
    state[2] = 0x98badcfeu;
    state[3] = 0x10325476u;
    state[4] = 0xc3d2e1f0u;

    block_idx = length_high = length_low = 0;
}

inline bool SHA1::update(const void *in, size_t len)
{
    if (!in)
        return false;

    auto p = static_cast<const uint8_t*>(in);

    while (len--) {

        block[block_idx++] = *p++;

        if ((length_low  += 8)  == 0 && 
            (length_high += 1)  == 0) 
        {
            return false;
        }
        if (block_idx == BLOCK_SIZE)
            process();
    }
    return true;
}

inline bool SHA1::final(byte out[SIZE])
{
    if (!out)
        return false;

    pad();

    for (size_t i = 0, j = 0; i < STATE_SIZE; ++i, j += 4) {
        out[j + 0] = state[i] >> 24;
        out[j + 1] = state[i] >> 16;
        out[j + 2] = state[i] >> 8;
        out[j + 3] = state[i] >> 0;
    }
    memset(this, 0, sizeof(*this));

    return true;
}

inline bool SHA1::operator()(const void *in, size_t len, byte out[SIZE])
{
    init();
    return update(in, len) && final(out);
}

inline void SHA1::pad()
{
    block[block_idx++] = 0x80;

    if (block_idx > PAD_START) {
        memset(block + block_idx, 0, BLOCK_SIZE - block_idx);
        process();
    }
    memset(block + block_idx, 0, PAD_START - block_idx);

    for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
        block[BLOCK_SIZE - 1 - i] = length_low  >> j;
        block[BLOCK_SIZE - 5 - i] = length_high >> j;
    }
    process();
}

inline void SHA1::process()
{
    enum { a, b, c, d, e };

    word w[80];
    word var[STATE_SIZE];
    
    memcpy(var, state, sizeof(state));

    for (size_t t = 0; t < 16; ++t) {
        w[t]  = block[t * 4]     << 24;
        w[t] |= block[t * 4 + 1] << 16;
        w[t] |= block[t * 4 + 2] << 8;
        w[t] |= block[t * 4 + 3];
    }
    for (size_t t = 16; t < 80; ++t)
        w[t] = rol(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16], 1);

#define SHA_1_ROUND(start, end, K, ...)                                 \
    for (size_t t = start; t < end; ++t) {                              \
        word tmp = rol(var[a], 5) + var[e] + w[t] + K + (__VA_ARGS__);  \
        var[e] = var[d];                                                \
        var[d] = var[c];                                                \
        var[c] = rol(var[b], 30);                                       \
        var[b] = var[a];                                                \
        var[a] = tmp;                                                   \
    }
    SHA_1_ROUND(0,  20, 0x5a827999, ch(var[b], var[c], var[d]))
    SHA_1_ROUND(20, 40, 0x6ed9eba1, parity(var[b], var[c], var[d]))
    SHA_1_ROUND(40, 60, 0x8f1bbcdc, maj(var[b], var[c], var[d]))
    SHA_1_ROUND(60, 80, 0xca62c1d6, parity(var[b], var[c], var[d]))
#undef SHA_1_ROUND

    for (size_t i = 0; i < STATE_SIZE; ++i)
        state[i] += var[i];

    block_idx = 0;
}

}

#endif
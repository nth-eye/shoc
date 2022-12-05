#ifndef SHOC_CIPHER_CHACHA_H
#define SHOC_CIPHER_CHACHA_H

#include "shoc/util.h"

namespace shoc {
namespace impl::chacha {

using word = uint32_t;

// constexpr void quarter_round(word& a, word& b, word& c, word& d)
// {
//     a += b; d ^= a; d = rol(d, 16);
//     c += d; b ^= c; b = rol(b, 12);
//     a += b; d ^= a; d = rol(d, 8);
//     c += d; b ^= c; b = rol(b, 7);
// }

struct context {

    static constexpr size_t key_size    = 32;
    static constexpr size_t nonce_size  = 12;
public:
    constexpr context() = default;
    constexpr context(span_i<key_size> key, span_i<nonce_size> nonce) { init(key, nonce); }
    // constexpr ~context()                    { deinit(); }
public:
    constexpr void init(span_i<key_size> key, span_i<nonce_size> nonce);
    constexpr void deinit();
    // constexpr void encrypt(span_i<block_size> in, span_o<block_size> out);
    // constexpr void decrypt(span_i<block_size> in, span_o<block_size> out);
private:
    constexpr void quarter_round(size_t a, size_t b, size_t c, size_t d);
    constexpr void round();
private:
    word state[16];
};

constexpr void context::init(span_i<key_size> key, span_i<nonce_size> nonce)
{
    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;

    auto ptr = key.data();

    for (size_t i = 4; i < 12; ++i, ptr += sizeof(word))
        state[i] = getle<word>(ptr);

    state[12] = 0;

    ptr = nonce.data();

    for (size_t i = 13; i < 16; ++i, ptr += sizeof(word))
        state[i] = getle<word>(ptr);
}

constexpr void context::quarter_round(size_t a, size_t b, size_t c, size_t d)
{
    state[a] += state[b]; state[d] ^= state[a]; state[d] = rol(state[d], 16);
    state[c] += state[d]; state[b] ^= state[c]; state[b] = rol(state[b], 12);
    state[a] += state[b]; state[d] ^= state[a]; state[d] = rol(state[d], 8);
    state[c] += state[d]; state[b] ^= state[c]; state[b] = rol(state[b], 7);
}

constexpr void context::round()
{
    for (int i = 0; i < 10; ++i) {
        quarter_round(0, 4, 8,  12);
        quarter_round(1, 5, 9,  13);
        quarter_round(2, 6, 10, 14);
        quarter_round(3, 7, 11, 15);
        quarter_round(0, 5, 10, 15);
        quarter_round(1, 6, 11, 12);
        quarter_round(2, 7, 8,  13);
        quarter_round(3, 4, 9,  14);
    }
}

}

}

#endif
#ifndef SHOC_HASH_GIMLI_H
#define SHOC_HASH_GIMLI_H

#include "shoc/util.h"

namespace shoc {

struct gimli : consumer<gimli, 32> {
    static constexpr size_t state_size = 48;    // In bytes
    static constexpr size_t block_size = 16;    // In bytes
public:
    constexpr void init();
    constexpr void feed(span_i<> in);
    constexpr void stop(span_o<hash_size> out);
    constexpr void wipe();
private:
    constexpr void step();
private:
    using word = uint32_t;
private:
    union {
        word state32[3][4];
        byte state8[state_size];
    };
    byte idx;
};

constexpr void gimli::init()
{
    wipe();
}

constexpr void gimli::feed(span_i<> in)
{
    for (auto it : in) {
        state8[idx++] ^= it;
        if (idx == block_size) {
            idx = 0;
            step();
        }
    }
}

constexpr void gimli::stop(span_o<hash_size> out)
{
    state8[idx] ^= 0x1f;
    state8[15]  ^= 0x80;
    step();
    copy(&out[0], state8, 16);
    step();
    copy(&out[16], state8, 16);
    wipe();
}

constexpr void gimli::wipe()
{
    zero(state8, sizeof(state8));
    idx = 0;
}

constexpr void gimli::step()
{
    word x, y, z;

    for (int r = 24; r > 0; --r) {
        for (int c = 0; c < 4; ++c) {
            x = rol(state32[0][c], 24);
            y = rol(state32[1][c], 9);
            z =     state32[2][c];
            state32[2][c] = x ^ (z << 1) ^ ((y & z) << 2);
            state32[1][c] = x ^ y ^ ((x | z) << 1);
            state32[0][c] = y ^ z ^ ((x & y) << 3);
        }
        switch (r & 3) {
            case 2:
                swap(state32[0][0], state32[0][2]);
                swap(state32[0][1], state32[0][3]);
                break;
            case 0:
                swap(state32[0][0], state32[0][1]);
                swap(state32[0][2], state32[0][3]);
                state32[0][0] ^= 0x9e377900 | r;
        }
    }
}

}

#endif
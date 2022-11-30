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
    auto step_impl = [](decltype(state32)& buf) {
        word x, y, z;
        for (int r = 24; r > 0; --r) {
            for (int c = 0; c < 4; ++c) {
                x = rol(buf[0][c], 24);
                y = rol(buf[1][c], 9);
                z =     buf[2][c];
                buf[2][c] = x ^ (z << 1) ^ ((y & z) << 2);
                buf[1][c] = x ^ y ^ ((x | z) << 1);
                buf[0][c] = y ^ z ^ ((x & y) << 3);
            }
            switch (r & 3) {
                case 2:
                    swap(buf[0][0], buf[0][2]);
                    swap(buf[0][1], buf[0][3]);
                    break;
                case 0:
                    swap(buf[0][0], buf[0][1]);
                    swap(buf[0][2], buf[0][3]);
                    buf[0][0] ^= 0x9e377900 | r;
            }
        }
    };

    if (std::is_constant_evaluated()) {
        decltype(state32) tmp;
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                tmp[i][j] = 
                    word(state8[i * 16 + j * 4 + 0] <<  0) | 
                    word(state8[i * 16 + j * 4 + 1] <<  8) | 
                    word(state8[i * 16 + j * 4 + 2] << 16) | 
                    word(state8[i * 16 + j * 4 + 3] << 24);
            }
        }
        step_impl(tmp);
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                state8[i * 16 + j * 4 + 0] = (tmp[i][j] >>  0) & 0xff;
                state8[i * 16 + j * 4 + 1] = (tmp[i][j] >>  8) & 0xff;
                state8[i * 16 + j * 4 + 2] = (tmp[i][j] >> 16) & 0xff;
                state8[i * 16 + j * 4 + 3] = (tmp[i][j] >> 24) & 0xff;
            }
        }
        return;
    }
    step_impl(state32);
}

}

#endif
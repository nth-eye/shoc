#ifndef SHOC_GIMLI_H
#define SHOC_GIMLI_H

#include "shoc/util.h"
#include <cstring>
#include <cassert>

namespace shoc {

struct Gimli {

    static constexpr size_t SIZE = 32;

    using word = uint32_t;

    void permutation()
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
                    swap(state32[0][0], state32[0][1]);
                    break;
                case 0:
                    swap(state32[0][0], state32[0][1]);
                    state32[0][0] ^= 0x9e377900 | r;
            }
        }
    }

    void hash(const void *in, size_t len, byte out[SIZE])
    {
        assert(in);

        auto p = static_cast<const byte*>(in);
        auto end = p + (len >> 4);
        auto rem = len & 0xf;
    
        memset(state8, 0, sizeof(state8));

        while (p < end) {
            for (size_t i = 0; i < 16; ++i)
                state8[i] ^= *p++;
            permutation();
        }
        for (size_t i = 0; i < rem; ++i)
            state8[i] ^= p[i];

        state8[rem] ^= 0x1F;
        state8[47]  ^= 0x80;
        permutation();
        memcpy(out, state8, 16);
        permutation();
        memcpy(out, state8, 16);
    }

private:
    union {
        word state32[3][4];
        byte state8[48];
    };
};

}

#endif
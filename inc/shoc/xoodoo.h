#ifndef SHOC_XOODOO_H
#define SHOC_XOODOO_H

#include "shoc/_util.h"

namespace shoc {

struct Xoodoo {

    using word = uint32_t;

    static constexpr auto ROW = 3;
    static constexpr auto COL = 4;
    static constexpr auto WORDS = ROW * COL;
    static constexpr auto BYTES = WORDS * sizeof(word);
    static constexpr auto ROUNDS = 12;

    void permute()
    {
        for (int i = 0; i < ROUNDS; ++i)
            round(i);
    }

    void round(int r)
    {
        theta();
        rhowest();
        iota(rc[r]);
        chi();
        rhoeast();
    }

    void theta()
    {
        word p[COL];
        word e[COL];

        for (int i = 0; i < COL; ++i)
            p[i] = u32[0][i] ^ u32[1][i] ^ u32[2][i];

        for (int i = 0; i < COL; ++i)
            e[i] = rol(p[(i - 1) & 3], 5) ^ rol(p[(i - 1) & 3], 14);

        for (int i = 0; i < ROW; ++i)
            for (int j = 0; j < COL; ++j)
                u32[i][j] ^= e[i];
    }

    void rhowest()
    {
        word b[ROW][COL];
        for (int i = 0; i < COL; ++i) {
            b[0][i] =     u32[0][i];
            b[1][i] =     u32[1][(i - 1) & 3];
            b[2][i] = rol(u32[2][i], 11);
        }
        copy(u32, b, sizeof(u32));
    }

    void iota(word c)
    {
        u32[0][0] ^= c;
    }

    void chi()
    {
        word b[ROW][COL];
        for (int i = 0; i < ROW; ++i)
            for (int j = 0; j < COL; ++j)
                b[i][j] = u32[i][j] ^ 
                        (~u32[(i + 1) % 3][j] & 
                          u32[(i + 2) % 3][j]);
        copy(u32, b, sizeof(u32));
    }

    void rhoeast()
    {
        word b[ROW][COL];
        for (int i = 0; i < COL; ++i) {
            b[0][i] =     u32[0][i];
            b[1][i] = rol(u32[1][i], 1);
            b[2][i] = rol(u32[2][(i + 2) & 3], 8);
        }
        copy(u32, b, sizeof(u32));
    }

    static constexpr word rc[ROUNDS] = {
        0x00000058, 0x00000038, 0x000003c0, 0x000000d0,
        0x00000120, 0x00000014, 0x00000060, 0x0000002c,
        0x00000380, 0x000000f0, 0x000001a0, 0x00000012,
    };

    union {
        word u32[ROW][COL];
        byte u8[BYTES];
    };
};

struct Cyclist {

    

};

}

#endif
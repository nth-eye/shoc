#ifndef SHOC_HASH_GIMLI_H
#define SHOC_HASH_GIMLI_H

#include "shoc/util.h"

// namespace shoc {

// struct Gimli : consumer<Gimli> {
//     static constexpr size_t SIZE        = 32;
//     static constexpr size_t STATE_SIZE  = 48;   // In bytes
//     static constexpr size_t BLOCK_SIZE  = 16;   // In bytes
// public:
//     void init();
//     void feed(const void *in, size_t len);
//     void stop(byte *out);
// private:
//     void step();
// private:
//     using word = uint32_t;
// private:
//     union {
//         word state32[3][4];
//         byte state8[STATE_SIZE];
//     };
//     byte idx;
// };

// inline void Gimli::init()
// {
//     fill(state8, 0, sizeof(state8));
//     idx = 0;
// }

// inline void Gimli::feed(const void *in, size_t len)
// {
//     assert(in || !len);

//     auto p = static_cast<const byte*>(in);

//     while (len--) {
//         state8[idx++] ^= *p++;
//         if (idx == BLOCK_SIZE) {
//             idx = 0;
//             step();
//         }
//     }
// }

// inline void Gimli::stop(byte *out)
// {
//     assert(out);

//     state8[idx] ^= 0x1F;
//     state8[15]  ^= 0x80;
//     step();
//     copy(out, state8, 16);
//     step();
//     copy(out + 16, state8, 16);
//     zero(this, sizeof(*this));
// }

// inline void Gimli::step()
// {
//     word x, y, z;

//     for (int r = 24; r > 0; --r) {
//         for (int c = 0; c < 4; ++c) {
//             x = rol(state32[0][c], 24);
//             y = rol(state32[1][c], 9);
//             z =     state32[2][c];
//             state32[2][c] = x ^ (z << 1) ^ ((y & z) << 2);
//             state32[1][c] = x ^ y ^ ((x | z) << 1);
//             state32[0][c] = y ^ z ^ ((x & y) << 3);
//         }
//         switch (r & 3) {
//             case 2:
//                 swap(state32[0][0], state32[0][2]);
//                 swap(state32[0][1], state32[0][3]);
//                 break;
//             case 0:
//                 swap(state32[0][0], state32[0][1]);
//                 swap(state32[0][2], state32[0][3]);
//                 state32[0][0] ^= 0x9e377900 | r;
//         }
//     }
// }

// }

#endif
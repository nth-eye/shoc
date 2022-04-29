#ifndef SHOC_HASH_MD5_H
#define SHOC_HASH_MD5_H

#include "shoc/_util.h"

namespace shoc {

// struct Md5 {

//     using word = uint32_t;

//     static constexpr size_t SIZE = 16;

//     void init();
//     void update(const void *in, size_t len);
//     void final(byte out[SIZE]);
//     void operator()(const void *in, size_t len, byte out[SIZE]);
// private:
//     void pad();
//     void step();

//     static constexpr size_t STATE_SIZE      = 4;    // In words
//     static constexpr size_t BLOCK_SIZE      = 64;   // In bytes
//     static constexpr size_t LENGTH_START    = BLOCK_SIZE - 8;

//     word length_low;
//     word length_high;
//     word state[STATE_SIZE];
//     byte block[BLOCK_SIZE];
//     byte block_idx;
// };

// void Md5::init()
// {
//     state[0] = 0x67452301;
//     state[1] = 0xefcdab89;
//     state[2] = 0x98badcfe;
//     state[3] = 0x10325476;

//     block_idx = length_high = length_low = 0;
// }

// void Md5::update(const void *in, size_t len)
// {
//     assert(in);

//     auto p = static_cast<const byte*>(in);

//     while (len--) {

//         block[block_idx++] = *p++;

//         if ((length_low += 8) == 0)
//             length_high += 1;
//         if (block_idx == BLOCK_SIZE)
//             step();
//     }
// }

// void Md5::final(byte out[SIZE])
// {

// }

// void Md5::operator()(const void *in, size_t len, byte out[SIZE])
// {

// }

// void Md5::pad()
// {
//     // block[block_idx++] = 0x80;

//     // if (block_idx > LENGTH_START) {
//     //     fill(block + block_idx, 0, BLOCK_SIZE - block_idx);
//     //     transform();
//     // }
//     // fill(block + block_idx, 0, LENGTH_START - block_idx);

//     // for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
//     //     block[BLOCK_SIZE - 8 + i] = length_low  >> j;
//     //     block[BLOCK_SIZE - 4 + i] = length_high >> j;
//     // }
//     // transform();
// }

// void Md5::step()
// {

// }

}

#endif
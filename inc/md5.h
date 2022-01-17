#ifndef MD5_H
#define MD5_H

#include <cstdint>
#include <cstddef>
#include <cstring>

// namespace creep {

// struct MD5 {

//     static constexpr size_t SIZE = 16;

//     using byte = uint8_t;
//     using word = uint32_t;

//     void init();
//     bool update(const void *in, size_t len);
//     bool final(byte out[SIZE]);
//     bool operator()(const void *in, size_t len, byte out[SIZE]);
// private:
//     void pad();
//     void process();

//     static constexpr size_t STATE_SIZE      = 4;    // In words
//     static constexpr size_t BLOCK_SIZE      = 64;   // In bytes
//     static constexpr size_t LENGTH_START    = BLOCK_SIZE - 8;

//     word length_low;
//     word length_high;
//     word state[STATE_SIZE];
//     byte block[BLOCK_SIZE];
//     byte block_idx;
// };

// void MD5::init()
// {
//     state[0] = 0x67452301;
//     state[1] = 0xefcdab89;
//     state[2] = 0x98badcfe;
//     state[3] = 0x10325476;

//     block_idx = length_high = length_low = 0;
// }

// bool MD5::update(const void *in, size_t len)
// {
//     if (!in)
//         return false;

//     auto p = static_cast<const uint8_t*>(in);

//     while (len--) {

//         block[block_idx++] = *p++;

//         if ((length_low += 8) == 0)
//             length_high += 1;

//         if (block_idx == BLOCK_SIZE)
//             process();
//     }
//     return true;
// }

// bool MD5::final(byte out[SIZE])
// {

// }

// bool MD5::operator()(const void *in, size_t len, byte out[SIZE])
// {

// }

// void MD5::pad()
// {
//     // block[block_idx++] = 0x80;

//     // if (block_idx > LENGTH_START) {
//     //     memset(block + block_idx, 0, BLOCK_SIZE - block_idx);
//     //     transform();
//     // }
//     // memset(block + block_idx, 0, LENGTH_START - block_idx);

//     // for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
//     //     block[BLOCK_SIZE - 8 + i] = length_low  >> j;
//     //     block[BLOCK_SIZE - 4 + i] = length_high >> j;
//     // }
//     // transform();
// }

// void MD5::process()
// {

// }

// }

#endif
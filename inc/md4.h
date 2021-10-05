#ifndef MD4_H
#define MD4_H

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace md4 {

struct MD4 {

    using byte = uint8_t;
    using word = uint32_t;

    static constexpr size_t HASH_SIZE       = 16;   // In bytes
    static constexpr size_t STATE_SIZE      = 4;    // In words
    static constexpr size_t BLOCK_SIZE      = 64;   // In bytes
    static constexpr size_t LEN_START_BYTE  = BLOCK_SIZE - 8;

    void init()
    {
        constexpr word h[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

        for (size_t i = 0; i < STATE_SIZE; ++i)
            state[i] = h[i];

        block_idx = length_high = length_low = 0;
    }
    bool update(const void *data, size_t len)
    {
        const uint8_t *p = (uint8_t*) data;

        while (len--) {

            block[block_idx++] = *p++;

            if ((length_low += 8) == 0)
                length_high += 1;

            if (block_idx == BLOCK_SIZE)
                transform();
        }
        return true;
    }
    bool final(byte *digest)
    {
        if (!digest)
            return false;

        pad();

        

        return true;
    }
private:
    void pad()
    {
        block[block_idx++] = 0x80;

        if (block_idx > LEN_START_BYTE) {
            while (block_idx < BLOCK_SIZE)
                block[block_idx++] = 0;
            transform();
        } 
        while (block_idx < LEN_START_BYTE)
            block[block_idx++] = 0;

        for (size_t i = 0, j = 0; i < 4; ++i, j += 8) {
            block[BLOCK_SIZE - 1 - i] = length_low  >> j;
            block[BLOCK_SIZE - 5 - i] = length_high >> j;
        }
        transform();
    }
    void transform()
    {

    }
    
    word state[STATE_SIZE];
    word length_low;
    word length_high;
    byte block[BLOCK_SIZE];
    byte block_idx;
};

}

#endif
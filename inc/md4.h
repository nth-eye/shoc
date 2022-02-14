#ifndef MD4_H
#define MD4_H

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace creep {

struct MD4 {

    static constexpr size_t SIZE = 16;

    using byte = uint8_t;
    using word = uint32_t;

    void init();
    bool update(const void *in, size_t len);
    bool final(byte out[SIZE]);
    bool operator()(const void *in, size_t len, byte out[SIZE]);
private:
    void pad();
    void process();

    static constexpr size_t STATE_SIZE  = 4;    // In words
    static constexpr size_t BLOCK_SIZE  = 64;   // In bytes
    static constexpr size_t PAD_START   = BLOCK_SIZE - 8;

    static constexpr word rol(word x, int shift)            { return (x << shift) | (x >> (32 - shift)); }
    static constexpr word ch(word x, word y, word z)        { return (x & y) | (~x & z); }
    static constexpr word maj(word x, word y, word z)       { return (x & y) | (x & z) | (y & z); }
    static constexpr word parity(word x, word y, word z)    { return x ^ y ^ z; }
    
    word length_low;
    word length_high;
    word state[STATE_SIZE];
    byte block[BLOCK_SIZE];
    byte block_idx;
};

}

#endif
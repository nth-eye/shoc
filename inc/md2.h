#ifndef MD2_H
#define MD2_H

#include <cstdint>
#include <cstddef>
#include <cstring>

namespace creep {

struct MD2 {

    static constexpr size_t SIZE = 16;

    using byte = uint8_t;

    void init();
    bool update(const void *in, size_t len);
    bool final(byte out[SIZE]);
    bool operator()(const void *in, size_t len, byte out[SIZE]);
private:
    void process();

    byte checksum[16];
    byte state[16];
    byte block[16];
    byte block_idx;
};

}

#endif
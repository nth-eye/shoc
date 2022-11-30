#ifndef SHOC_MAC_HMAC_H
#define SHOC_MAC_HMAC_H

#include "shoc/util.h"

namespace shoc {

template<class H>
constexpr void hmac(span_i<> msg, span_i<> key, span_o<H::hash_size> digest)
{
    H hash;
    byte k_ipad[H::block_size] = {};
    byte k_opad[H::block_size] = {};
    byte tk[H::hash_size] = {};

    auto key_ptr = key.data();
    auto key_len = key.size();

    if (key_len > H::block_size) {
        hash.init();
        hash.feed(key);
        hash.stop(tk);
        key_ptr = tk;
        key_len = H::hash_size;
    }
    copy(k_ipad, key_ptr, key_len);
    copy(k_opad, key_ptr, key_len);

    for (size_t i = 0; i < H::block_size; ++i) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }
    hash.init();
    hash.feed(k_ipad);
    hash.feed(msg);
    hash.stop(digest);
    hash.init();
    hash.feed(k_opad);
    hash.feed(digest);
    hash.stop(digest);
}

// template<class H>
// struct Hmac {
//     void init(const void *key, size_t key_len);
//     void feed(const void *msg, size_t msg_len);
//     void stop(byte *out);
//     void operator()(const void *msg, size_t msg_len, const void *key, size_t key_len, byte *out);
// private:
//     H hash;
//     byte k_pad[H::block_size];
//     byte tk[H::SIZE];
// };

// template<class H>
// void Hmac<H>::init(const void *key, size_t key_len)
// {
//     if (key_len > H::block_size) {

//         hash.init();
//         hash.feed(key, key_len);
//         hash.stop(tk);

//         key = tk;
//         key_len = H::SIZE;
//     }

//     fill(k_pad, 0, sizeof(k_pad));
//     copy(k_pad, key, key_len);
//     for (auto &it : k_pad)
//         it ^= 0x36;

//     hash.init();
//     hash.feed(k_pad, sizeof(k_pad));

//     fill(k_pad, 0, sizeof(k_pad));
//     copy(k_pad, key, key_len);
//     for (auto &it : k_pad)
//         it ^= 0x5C;
// }

// template<class H>
// void Hmac<H>::feed(const void *msg, size_t msg_len)
// {
//     hash.feed(msg, msg_len);
// }

// template<class H>
// void Hmac<H>::stop(byte *out)
// {
//     hash.stop(out);
//     hash.init();
//     hash.feed(k_pad, sizeof(k_pad));
//     hash.feed(out, H::SIZE);
//     hash.stop(out);
// }

// template<class H>
// void Hmac<H>::operator()(const void *msg, size_t msg_len, const void *key, size_t key_len, byte *out)
// {
//     init(key, key_len), feed(msg, msg_len), stop(out);
// }

}

#endif
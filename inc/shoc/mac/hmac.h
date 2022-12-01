#ifndef SHOC_MAC_HMAC_H
#define SHOC_MAC_HMAC_H

#include "shoc/util.h"

namespace shoc {

template<class H>
constexpr void hmac(span_i<> key, span_i<> msg, span_o<H::hash_size> digest)
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
        key_len = sizeof(tk);
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

template<class H>
struct hmac_ctx : consumer<hmac_ctx<H>, H::hash_size> {
    constexpr void init(span_i<> key);
    constexpr void feed(span_i<> msg);
    constexpr void stop(span_o<H::hash_size> out);
    constexpr void wipe();
private:
    H hash;
    byte k_pad[H::block_size];
};

template<class H>
constexpr void hmac_ctx<H>::init(span_i<> key)
{
    auto key_ptr = key.data();
    auto key_len = key.size();
    byte tk[H::hash_size];

    if (key_len > H::block_size) {
        hash.init();
        hash.feed(key);
        hash.stop(tk);
        key_ptr = tk;
        key_len = sizeof(tk);
    }
    zero(k_pad, sizeof(k_pad));
    copy(k_pad, key_ptr, key_len);
    xorb(k_pad, 0x36);

    hash.init();
    hash.feed(k_pad);

    zero(k_pad, sizeof(k_pad));
    copy(k_pad, key_ptr, key_len);
    xorb(k_pad, 0x5c);
}

template<class H>
constexpr void hmac_ctx<H>::feed(span_i<> msg)
{
    hash.feed(msg);
}

template<class H>
constexpr void hmac_ctx<H>::stop(span_o<H::hash_size> out)
{
    hash.stop(out);
    hash.init();
    hash.feed(k_pad);
    hash.feed(out);
    hash.stop(out);
}

template<class H>
constexpr void hmac_ctx<H>::wipe()
{
    zero(k_pad, sizeof(k_pad));
}

}

#endif
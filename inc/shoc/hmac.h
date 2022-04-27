#ifndef SHOC_HMAC_H
#define SHOC_HMAC_H

#include "shoc/_util.h"

namespace shoc {

template<class H>
void hmac(const void *msg, size_t msg_len, const void *key, size_t key_len, byte *digest)
{
    H hash;
    byte k_ipad[H::BLOCK_SIZE] = {};
    byte k_opad[H::BLOCK_SIZE] = {};
    byte tk[H::SIZE] = {};

    if (key_len > H::BLOCK_SIZE) {

        hash.init();
        hash.update(key, key_len);
        hash.final(tk);

        key = tk;
        key_len = H::SIZE;
    }

    copy(k_ipad, key, key_len);
    copy(k_opad, key, key_len);

    for (size_t i = 0; i < H::BLOCK_SIZE; ++i) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5C;
    }

    hash.init();
    hash.update(k_ipad, H::BLOCK_SIZE);
    hash.update(msg, msg_len);
    hash.final(digest);

    hash.init();
    hash.update(k_opad, H::BLOCK_SIZE);
    hash.update(digest, H::SIZE);
    hash.final(digest);
}

template<class H>
struct Hmac {
    void init(const void *key, size_t key_len);
    void update(const void *msg, size_t msg_len);
    void final(byte *out);
    void operator()(const void *msg, size_t msg_len, const void *key, size_t key_len, byte *out);
private:
    H hash;
    byte k_pad[H::BLOCK_SIZE];
    byte tk[H::SIZE];
};

template<class H>
void Hmac<H>::init(const void *key, size_t key_len)
{
    if (key_len > H::BLOCK_SIZE) {

        hash.init();
        hash.update(key, key_len);
        hash.final(tk);

        key = tk;
        key_len = H::SIZE;
    }

    fill(k_pad, 0, sizeof(k_pad));
    copy(k_pad, key, key_len);
    for (auto &it : k_pad)
        it ^= 0x36;

    hash.init();
    hash.update(k_pad, sizeof(k_pad));

    fill(k_pad, 0, sizeof(k_pad));
    copy(k_pad, key, key_len);
    for (auto &it : k_pad)
        it ^= 0x5C;
}

template<class H>
void Hmac<H>::update(const void *msg, size_t msg_len)
{
    hash.update(msg, msg_len);
}

template<class H>
void Hmac<H>::final(byte *out)
{
    hash.final(out);
    hash.init();
    hash.update(k_pad, sizeof(k_pad));
    hash.update(out, H::SIZE);
    hash.final(out);
}

template<class H>
void Hmac<H>::operator()(const void *msg, size_t msg_len, const void *key, size_t key_len, byte *out)
{
    init(key, key_len), update(msg, msg_len), final(out);
}

}

#endif
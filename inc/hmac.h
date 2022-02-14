#ifndef HMAC_H
#define HMAC_H

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace creep {

template<class H>
void hmac(const void *msg, size_t msg_len, const void *key, size_t key_len, uint8_t *digest)
{
    H hash;
    uint8_t k_ipad[H::BLOCK_SIZE] = {};
    uint8_t k_opad[H::BLOCK_SIZE] = {};
    uint8_t tk[H::SIZE] = {};

    if (key_len > H::BLOCK_SIZE) {

        hash.init();
        hash.update(key, key_len);
        hash.final(tk);

        key = tk;
        key_len = H::SIZE;
    }

    memcpy(k_ipad, key, key_len);
    memcpy(k_opad, key, key_len);

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
struct HMAC {

    void init(const void *key, size_t key_len);
    void update(const void *msg, size_t msg_len);
    void final(uint8_t *out);
    void operator()(const void *msg, size_t msg_len, const void *key, size_t key_len, uint8_t *out);
private:
    H hash;
    uint8_t k_pad[H::BLOCK_SIZE] = {};
    uint8_t tk[H::SIZE] = {};
};

template<class H>
void HMAC<H>::init(const void *key, size_t key_len)
{
    if (key_len > H::BLOCK_SIZE) {

        hash.init();
        hash.update(key, key_len);
        hash.final(tk);

        key = tk;
        key_len = H::SIZE;
    }

    memset(k_pad, 0, sizeof(k_pad));
    memcpy(k_pad, key, key_len);
    for (auto &it : k_pad)
        it ^= 0x36;

    hash.init();
    hash.update(k_pad, sizeof(k_pad));

    memset(k_pad, 0, sizeof(k_pad));
    memcpy(k_pad, key, key_len);
    for (auto &it : k_pad)
        it ^= 0x5C;
}

template<class H>
void HMAC<H>::update(const void *msg, size_t msg_len)
{
    hash.update(msg, msg_len);
}

template<class H>
void HMAC<H>::final(uint8_t *out)
{
    hash.final(out);
    hash.init();
    hash.update(k_pad, sizeof(k_pad));
    hash.update(out, H::SIZE);
    hash.final(out);
}

template<class H>
void HMAC<H>::operator()(const void *msg, size_t msg_len, const void *key, size_t key_len, uint8_t *out)
{
    init(key, key_len);
    update(msg, msg_len);
    final(out);
}

}

#endif
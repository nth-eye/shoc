#ifndef HMAC_H
#define HMAC_H

#include <cstddef>
#include <cstdint>

namespace creep {

// template<class H>
// constexpr void hmac(H &hash, const void *msg, size_t msg_len, const void *key, size_t key_len, uint8_t *digest)
// {
//     uint8_t k_ipad[H::BLOCK_SIZE] = {};
//     uint8_t k_opad[H::BLOCK_SIZE] = {};
//     uint8_t tk[H::HASH_SIZE] = {};

//     if (key_len > H::BLOCK_SIZE) {

//         hash.init();
//         hash.update(key, key_len);
//         hash.final(tk);

//         key = tk;
//         key_len = H::HASH_SIZE;
//     }

//     memcpy(k_ipad, key, key_len);
//     memcpy(k_opad, key, key_len);

//     for (size_t i = 0; i < H::BLOCK_SIZE; ++i) {
//         k_ipad[i] ^= 0x36;
//         k_opad[i] ^= 0x5C;
//     }

//     hash.init();
//     hash.update(k_ipad, H::BLOCK_SIZE);
//     hash.update(msg, msg_len);
//     hash.final(digest);

//     hash.init();
//     hash.update(k_opad, H::BLOCK_SIZE);
//     hash.update(digest, H::HASH_SIZE);
//     hash.final(digest);
// }

}

#endif
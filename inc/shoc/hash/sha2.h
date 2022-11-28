#ifndef SHOC_HASH_SHA2_H
#define SHOC_HASH_SHA2_H

#include "shoc/util.h"

namespace shoc {

// constexpr int SHA2_WORD64_FLAG = 0x1000;

// enum Sha2Type {
//     SHA_224     = 224,
//     SHA_256     = 256,
//     SHA_384     = 384 | SHA2_WORD64_FLAG,
//     SHA_512     = 512 | SHA2_WORD64_FLAG,
//     SHA_512_224 = 224 | SHA2_WORD64_FLAG,
//     SHA_512_256 = 256 | SHA2_WORD64_FLAG,
// };

// constexpr int operator&(Sha2Type &lhs, int rhs)     { return int(lhs) & rhs; }

// template <int T> struct word_size                   { using type = uint32_t; };
// template <>      struct word_size<SHA2_WORD64_FLAG> { using type = uint64_t; };

// template<Sha2Type T>
// struct Sha2 : consumer<Sha2<T>> {
// private:
//     using word = typename word_size<T & SHA2_WORD64_FLAG>::type;
// public:
//     static constexpr size_t SIZE        = (T & ~SHA2_WORD64_FLAG) / 8;
//     static constexpr size_t BLOCK_SIZE  = 16 * sizeof(word);
//     static constexpr size_t STATE_SIZE  = 8;
// public:
//     void init();
//     void feed(const void *in, size_t len);
//     void stop(byte *out);
// private:
//     void pad();
//     void step();
// private:
//     constexpr uint32_t sigma_0(uint32_t x)   { return ror(x, 7)  ^ ror(x, 18) ^ (x >> 3);  }
//     constexpr uint32_t sigma_1(uint32_t x)   { return ror(x, 17) ^ ror(x, 19) ^ (x >> 10); }
//     constexpr uint64_t sigma_0(uint64_t x)   { return ror(x, 1)  ^ ror(x, 8)  ^ (x >> 7);  }
//     constexpr uint64_t sigma_1(uint64_t x)   { return ror(x, 19) ^ ror(x, 61) ^ (x >> 6);  }
//     constexpr uint32_t sum_0(uint32_t x)     { return ror(x, 2)  ^ ror(x, 13) ^ ror(x, 22); }
//     constexpr uint32_t sum_1(uint32_t x)     { return ror(x, 6)  ^ ror(x, 11) ^ ror(x, 25); }
//     constexpr uint64_t sum_0(uint64_t x)     { return ror(x, 28) ^ ror(x, 34) ^ ror(x, 39); }
//     constexpr uint64_t sum_1(uint64_t x)     { return ror(x, 14) ^ ror(x, 18) ^ ror(x, 41); }
// private:
//     word length_low;
//     word length_high;
//     word state[STATE_SIZE];
//     byte block[BLOCK_SIZE];
//     byte block_idx;
// };

// template<Sha2Type T>
// void Sha2<T>::init()
// {
// #define INIT_HASH(...) {                            \
//     static constexpr uint64_t h[] = {__VA_ARGS__};  \
//     for (size_t i = 0; i < STATE_SIZE; ++i)         \
//         state[i] = h[i]; }

//     switch (T) {
//         case SHA_224:       INIT_HASH(0xc1059ed8u, 0x367cd507u, 0x3070dd17u, 0xf70e5939u, 
//                                       0xffc00b31u, 0x68581511u, 0x64f98fa7u, 0xbefa4fa4u) break;
//         case SHA_256:       INIT_HASH(0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au, 
//                                       0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u) break;
//         case SHA_384:       INIT_HASH(0xcbbb9d5dc1059ed8u, 0x629a292a367cd507u, 0x9159015a3070dd17u, 0x152fecd8f70e5939u, 
//                                       0x67332667ffc00b31u, 0x8eb44a8768581511u, 0xdb0c2e0d64f98fa7u, 0x47b5481dbefa4fa4u) break;
//         case SHA_512:       INIT_HASH(0x6a09e667f3bcc908u, 0xbb67ae8584caa73bu, 0x3c6ef372fe94f82bu, 0xa54ff53a5f1d36f1u, 
//                                       0x510e527fade682d1u, 0x9b05688c2b3e6c1fu, 0x1f83d9abfb41bd6bu, 0x5be0cd19137e2179u) break;
//         case SHA_512_224:   INIT_HASH(0x8C3D37C819544DA2u, 0x73E1996689DCD4D6u, 0x1DFAB7AE32FF9C82u, 0x679DD514582F9FCFu, 
//                                       0x0F6D2B697BD44DA8u, 0x77E36F7304C48942u, 0x3F9D85A86A1D36C8u, 0x1112E6AD91D692A1u) break;
//         case SHA_512_256:   INIT_HASH(0x22312194FC2BF72Cu, 0x9F555FA3C84C64C2u, 0x2393B86B6F53B151u, 0x963877195940EABDu, 
//                                       0x96283EE2A88EFFE3u, 0xBE5E1E2553863992u, 0x2B0199FC2C85B8AAu, 0x0EB72DDC81C52CA2u) break;
//     }
//     block_idx = length_high = length_low = 0;
// #undef INIT_HASH
// }

// template<Sha2Type T>
// void Sha2<T>::feed(const void *in, size_t len)
// {
//     assert(in || !len);

//     auto p = static_cast<const byte*>(in);

//     while (len--) {

//         block[block_idx++] = *p++;

//         if ((length_low += 8) == 0)
//             length_high += 1;
//         if (block_idx == BLOCK_SIZE)
//             step();
//     }
// }

// template<Sha2Type T>
// void Sha2<T>::stop(byte *out)
// {
//     assert(out);
//     pad();

//     for (int i = 0, j = -8; i < (int) SIZE; ++i, j -= 8)
//         out[i] = state[i / sizeof(word)] >> (j & ((sizeof(word) * 8) - 1));

//     zero(this, sizeof(*this));
// }

// template<Sha2Type T>
// void Sha2<T>::pad()
// {
//     static constexpr size_t pad_start = BLOCK_SIZE - sizeof(word) * 2;

//     block[block_idx++] = 0x80;

//     if (block_idx > pad_start) {
//         fill(block + block_idx, 0, BLOCK_SIZE - block_idx);
//         step();
//     }
//     fill(block + block_idx, 0, pad_start - block_idx);

//     for (size_t i = 0, j = 0; i < sizeof(word); ++i, j += 8) {
//         block[BLOCK_SIZE - 1 - i]               = length_low  >> j;
//         block[BLOCK_SIZE - 1 - i - sizeof(word)]   = length_high >> j;
//     }
//     step();
// }

// template<Sha2Type T>
// void Sha2<T>::step()
// {
//     enum { a, b, c, d, e, f, g, h };

//     constexpr size_t N_WORDS = (T == SHA_224 || T == SHA_256) ? 64 : 80;

//     word w[N_WORDS];
//     word var[STATE_SIZE];
    
//     for (size_t i = 0; i < STATE_SIZE; ++i) 
//         var[i] = state[i];

//     if constexpr (sizeof(word) == 4) {
//         for (size_t t = 0; t < 16; ++t) {
//             w[t]  = block[t * 4]     << 24;
//             w[t] |= block[t * 4 + 1] << 16;
//             w[t] |= block[t * 4 + 2] << 8;
//             w[t] |= block[t * 4 + 3];
//         }
//     } else {
//         for (size_t t = 0; t < 16; ++t) {
//             w[t]  = word(block[t * 8])     << 56;
//             w[t] |= word(block[t * 8 + 1]) << 48;
//             w[t] |= word(block[t * 8 + 2]) << 40;
//             w[t] |= word(block[t * 8 + 3]) << 32;
//             w[t] |= word(block[t * 8 + 4]) << 24;
//             w[t] |= word(block[t * 8 + 5]) << 16;
//             w[t] |= word(block[t * 8 + 6]) << 8;
//             w[t] |= word(block[t * 8 + 7]);
//         }
//     }

// #define SHA_2_ROUND(...) {                                                              \
//     static constexpr uint64_t K[] = {__VA_ARGS__};                                      \
//     for (size_t t = 16; t < N_WORDS; ++t)                                               \
//         w[t] = sigma_1(w[t-2]) + w[t-7] + sigma_0(w[t-15]) + w[t-16];                   \
//     for (size_t t = 0; t < N_WORDS; ++t) {                                              \
//         word tmp1 = var[h] + sum_1(var[e]) + ch(var[e], var[f], var[g]) + K[t] + w[t];  \
//         word tmp2 = sum_0(var[a]) + maj(var[a], var[b], var[c]);                        \
//         var[h] = var[g];                                                                \
//         var[g] = var[f];                                                                \
//         var[f] = var[e];                                                                \
//         var[e] = var[d] + tmp1;                                                         \
//         var[d] = var[c];                                                                \
//         var[c] = var[b];                                                                \
//         var[b] = var[a];                                                                \
//         var[a] = tmp1 + tmp2;                                                           \
//     }}

//     if constexpr (T == SHA_224 || T == SHA_256) {
//         SHA_2_ROUND(    
//         0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
//         0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
//         0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
//         0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
//         0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
//         0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
//         0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
//         0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2)
//     } else {
//         SHA_2_ROUND(
//         0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 
//         0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 
//         0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 
//         0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
//         0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
//         0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
//         0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
//         0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
//         0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
//         0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
//         0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
//         0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
//         0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
//         0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
//         0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
//         0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
//         0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
//         0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
//         0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
//         0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817)
//     }
// #undef SHA_2_ROUND

//     for (size_t i = 0; i < STATE_SIZE; ++i)
//         state[i] += var[i];

//     block_idx = 0;
// }

}

#endif
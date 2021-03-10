#include <cstring>
#include <cstdio>
#include "sha.h"

constexpr const char* hash_name(SHA_Type type)
{
    switch (type) {
        case SHA_1:         return "SHA-1";
        case SHA_224:       return "SHA-224";
        case SHA_256:       return "SHA-256";
        case SHA_384:       return "SHA-384";
        case SHA_512:       return "SHA-512";
        case SHA_512_224:   return "SHA-512/224";
        case SHA_512_256:   return "SHA-512/256";
    }
    return "UNKNOWN SHA";
}

template<SHA_Type T>
constexpr void print_sha_info()
{
    printf("%s              \n\
        word:    %d\tbits   \n\
        block:   %d\tbytes  \n\
        hash:    %d\tbytes  \n\
        state:   %d\twords  \n\
        pad end: %d\tbyte   \n\
        sizeof:  %d\tbytes  \n\
        \n",                        
        hash_name(T),            
        SHA<T>::WORD_SIZE * 8,
        SHA<T>::BLOCK_SIZE,
        SHA<T>::HASH_SIZE,
        SHA<T>::STATE_SIZE,
        SHA<T>::LEN_START_BYTE,
        sizeof(SHA<T>));
}

struct Pair {
    const char *msg;
    const char *expected;
};

template<SHA_Type T>
void test(const Pair *data, size_t num)
{
    print_sha_info<T>();

    SHA<T> sha;

    uint8_t digest[SHA<T>::HASH_SIZE + 1] = {};
    char result[SHA<T>::HASH_SIZE * 2 + 1] = {};

    for (int i = 0; i < num; ++i) {
        printf("Test:     '%s'\n", data[i].msg);

        sha.init();

        if (!sha.update((const uint8_t *)(data[i].msg), strlen(data[i].msg)))
            printf("update error\n");
        
        if (sha.final(digest)) {
            printf("Result:   ");
            for (int i = 0; i < SHA<T>::HASH_SIZE ; ++i)
                sprintf(&result[i * 2], "%02x", digest[i]);
            printf("%s\n", result);
        } else {
            printf("result error\n");
        }
        printf("Expected: %s\n", data[i].expected);

        if (strcmp(data[i].expected, result))
            printf("Matches : no\n\n");
        else
            printf("Matches : yes\n\n");
    }
}
    
int main()
{
    const Pair sha_1_test[] = {
        { "abc",
            "a9993e364706816aba3e25717850c26c9cd0d89d" },
        { "",
            "da39a3ee5e6b4b0d3255bfef95601890afd80709" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "84983e441c3bd26ebaae4aa1f95129e5e54670f1" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "a49b2446a02c645bf419f995b67091253a04a259" },

        { "The quick brown fox jumps over the lazy dog",
            "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12" },
        { "The quick brown fox jumps over the lazy cog", 
            "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3" },
    };

    const Pair sha_224_test[] = {
        { "abc",
            "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7" },
        { "",
            "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3" },
    };

    const Pair sha_256_test[] = {
        { "abc",
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" },
        { "",
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1" },
    };

    const Pair sha_384_test[] = {
        { "abc",
            "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" },
        { "",
            "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039" },
    };

    const Pair sha_512_test[] = {
        { "abc",
            "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" },
        { "",
            "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909" },
    };

    const Pair sha_512_224_test[] = {
        { "abc",
            "4634270f707b6a54daae7530460842e20e37ed265ceee9a43e8924aa" },
        { "",
            "6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "e5302d6d54bb242275d1e7622d68df6eb02dedd13f564c13dbda2174" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "23fec5bb94d60b23308192640b0c453335d664734fe40e7268674af9" },
    };

    const Pair sha_512_256_test[] = {
        { "abc",
            "53048e2681941ef99b2e29b76b4c7dabe4c2d0c634fc6d46e0e2f13107e7af23" },
        { "",
            "c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a" },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "bde8e1f9f19bb9fd3406c90ec6bc47bd36d8ada9f11880dbc8a22a7078b6a461" },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            "3928e184fb8690f840da3988121d31be65cb9d3ef83ee6146feac861e19b563a" },
    };

    test<SHA_1>(sha_1_test, sizeof(sha_1_test) / sizeof(Pair));
    test<SHA_224>(sha_224_test, sizeof(sha_224_test) / sizeof(Pair));
    test<SHA_256>(sha_256_test, sizeof(sha_256_test) / sizeof(Pair));
    test<SHA_384>(sha_384_test, sizeof(sha_384_test) / sizeof(Pair));
    test<SHA_512>(sha_512_test, sizeof(sha_512_test) / sizeof(Pair));
    test<SHA_512_224>(sha_512_224_test, sizeof(sha_512_224_test) / sizeof(Pair));
    test<SHA_512_256>(sha_512_256_test, sizeof(sha_512_256_test) / sizeof(Pair));
}
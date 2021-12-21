#include <cstring>
#include <cstdio>
#include <ctime>

#include "md2.h"
#include "md4.h"
#include "md5.h"
#include "sha1.h"
#include "sha2.h"
#include "hotp.h"

#define SIZE(x) (sizeof(x) / sizeof(x[0]))
#define EXEC_TIME(N, fn, ...)       \
{                                   \
    clock_t begin = clock();        \
    for (size_t i = 0; i < N; ++i)  \
        fn(__VA_ARGS__);            \
    clock_t end = clock();          \
    printf("[" #fn "] " #N " avg exec time: %lu clock_t \n", (end - begin) / N); \
}

struct Pair {
    const char *msg;
    const char *expected;
};

using namespace creep;

template<class Hash>
void test(const Pair *data, size_t num)
{
    printf("\n%s \n", __PRETTY_FUNCTION__);

    Hash hasher;

    uint8_t digest[Hash::HASH_SIZE]     = {};
    char    result[Hash::HASH_SIZE * 2] = {};

    for (size_t i = 0; i < num; ++i) {

        hasher.init();

        if (!hasher.update(data[i].msg, strlen(data[i].msg))) {
            printf("----error update:   '%s'\n", data[i].msg);
            continue;
        }

        if (!hasher.final(digest)) {
            printf("----error final:    '%s'\n", data[i].msg);
            continue;
        }
        
        for (size_t i = 0; i < sizeof(digest) ; ++i)
            sprintf(&result[i * 2], "%02x", digest[i]);

        if (strcmp(data[i].expected, result)) {
            printf("\n----Test:       '%s'", data[i].msg);
            printf("\n----Result:     %s", result);
            printf("\n----Expected:   %s\n", data[i].expected);
        }
    }
}

void hmac_sha_1_test()
{
    constexpr const char *key = "key";
    constexpr const char *msg = "The quick brown fox jumps over the lazy dog";
    constexpr const char *expected = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";

    printf("Test:    '%s'\n\n", key);

    SHA1        sha;
    uint8_t     digest[20] = {};
    char        result[20 * 2] = {};

    hmac(sha, msg, strlen(msg), key, strlen(key), digest);

    printf("Result:   ");
    for (int i = 0; i < 20; ++i)
        sprintf(&result[i * 2], "%02x", digest[i]);
    printf("%s\n", result);
    printf("Expected: %s\n", expected);
    printf("Matches:  %s\n\n", strcmp(expected, result) ? "no" : "yes");
}

void hotp_sha_1_test()
{
    SHA1 sha;

    constexpr const char *key = "12345678901234567890";
    constexpr const uint32_t expected[] = { 755224, 287082, 359152, 969429, 338314, 254676, 287922, 162583, 399871, 520489 };

    printf("Test:    '%s'\n\n", key);
    for (uint64_t i = 0; i < 10; ++i) {

        uint32_t code = hotp(sha, (uint8_t*) key, strlen(key), i, 6);

        printf("Counter:  %lu\n", i);
        printf("Result:   %u\n", code);
        printf("Correct:  %s\n\n", code == expected[i] ? "yes" : "no");
    }
}

#include <ctime>

template<size_t N = 1, class Fn, class ...Args>
clock_t measure_time(Fn &&fn, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        fn(args...);
    clock_t end = clock();

    return (end - begin); // / N;
}

// constexpr const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

// static MD2 hash;
// static uint8_t digest[MD2::SIZE];

// void md2_test_1()
// {
//     hash.init();
//     hash.update_1(str, sizeof(str));
//     hash.final_1(digest);
// }

// void md2_test_2()
// {
//     hash.init();
//     hash.update_2(str, sizeof(str));
//     hash.final_2(digest);
// }
    
int main()
{
    // printf("1: %lu clock_t \n", measure_time<1000000>(md2_test_1));
    // printf("2: %lu clock_t \n", measure_time<1000000>(md2_test_2));
    // printf("1: %lu clock_t \n", measure_time<1000000>(md2_test_1));
    // printf("2: %lu clock_t \n", measure_time<1000000>(md2_test_2));
    // printf("1: %lu clock_t \n", measure_time<1000000>(md2_test_1));

    // // ANCHOR: SHA-1

    // const Pair sha_1_test[] = {
    //     { "abc",
    //         "a9993e364706816aba3e25717850c26c9cd0d89d" },
    //     { "",
    //         "da39a3ee5e6b4b0d3255bfef95601890afd80709" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "84983e441c3bd26ebaae4aa1f95129e5e54670f1" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "a49b2446a02c645bf419f995b67091253a04a259" },
    //     { "The quick brown fox jumps over the lazy dog",
    //         "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12" },
    //     { "The quick brown fox jumps over the lazy cog", 
    //         "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3" },
    // };

    // test<SHA1>(sha_1_test, SIZE(sha_1_test));

    // // ANCHOR: SHA-224

    // const Pair sha_224_test[] = {
    //     { "abc",
    //         "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7" },
    //     { "",
    //         "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3" },
    // };

    // test<SHA2<SHA_224>>(sha_224_test, SIZE(sha_224_test));

    // // ANCHOR: SHA-256

    // const Pair sha_256_test[] = {
    //     { "abc",
    //         "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" },
    //     { "",
    //         "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1" },
    // };

    // test<SHA2<SHA_256>>(sha_256_test, SIZE(sha_256_test));

    // // ANCHOR: SHA-384

    // const Pair sha_384_test[] = {
    //     { "abc",
    //         "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" },
    //     { "",
    //         "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039" },
    // };

    // test<SHA2<SHA_384>>(sha_384_test, SIZE(sha_384_test));

    // // ANCHOR: SHA-512

    // const Pair sha_512_test[] = {
    //     { "abc",
    //         "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" },
    //     { "",
    //         "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909" },
    // };

    // test<SHA2<SHA_512>>(sha_512_test, SIZE(sha_512_test));

    // // ANCHOR: SHA-512/224

    // const Pair sha_512_224_test[] = {
    //     { "abc",
    //         "4634270f707b6a54daae7530460842e20e37ed265ceee9a43e8924aa" },
    //     { "",
    //         "6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "e5302d6d54bb242275d1e7622d68df6eb02dedd13f564c13dbda2174" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "23fec5bb94d60b23308192640b0c453335d664734fe40e7268674af9" },
    // };

    // test<SHA2<SHA_512_224>>(sha_512_224_test, SIZE(sha_512_224_test));
    
    // // ANCHOR: SHA-512/256

    // const Pair sha_512_256_test[] = {
    //     { "abc",
    //         "53048e2681941ef99b2e29b76b4c7dabe4c2d0c634fc6d46e0e2f13107e7af23" },
    //     { "",
    //         "c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a" },
    //     { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    //         "bde8e1f9f19bb9fd3406c90ec6bc47bd36d8ada9f11880dbc8a22a7078b6a461" },
    //     { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    //         "3928e184fb8690f840da3988121d31be65cb9d3ef83ee6146feac861e19b563a" },
    // };

    // test<SHA2<SHA_512_256>>(sha_512_256_test, SIZE(sha_512_256_test));

    // // ANCHOR: HMAC-SHA-1

    // hmac_sha_1_test();

    // // ANCHOR: HOTP-SHA-1

    // hotp_sha_1_test();

    // const char aaa[] = "aaaaaaaaaaaaaaaaa";
    // constexpr auto N = 10000000;

    // SHA1 sha1;

    // uint8_t digest[SHA1::HASH_SIZE];

    // clock_t begin = clock();
    // for (size_t i = 0; i < N; ++i) {
    //     sha1.init();
    //     sha1.update(aaa, sizeof(aaa));
    //     sha1.final(digest);
    // }
    // clock_t end = clock();

    // printf("1) digest %02x \n", digest[6]);
    // printf("1) %d avg exec time: %lu clock_t \n", N, (end - begin));
}

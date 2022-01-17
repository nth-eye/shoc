#include <gtest/gtest.h>
#include "md2.h"
#include "md4.h"
#include "md5.h"

using namespace creep;

struct Pair {
    const char *msg;
    const char *exp;
};

TEST(Hash, Md2) 
{
    const Pair test[] = {
        { "",
            "8350e5a3e24c153df2275c9f80692773" },
        { "a",
            "32ec01ec4a6dac72c0ab96fb34c0b5d1" },
        { "abc",
            "da853b0d3f88d99b30283a69e6ded6bb" },
        { "message digest",
            "ab4f496bfb2a530b219ff33031fe06b0" },
        { "abcdefghijklmnopqrstuvwxyz",
            "4e8ddff3650292ab5a4108c3aa47940b" },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            "da33def2a42df13975352846c30338cd" },
        { "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "d5976f79d83d3a0dc9806c3c66f3efd8" },
    };

    MD2 hash;

    uint8_t digest[decltype(hash)::SIZE];
    char str[decltype(hash)::SIZE * 2];

    for (auto it : test) {
        hash(it.msg, strlen(it.msg), digest);
        for (size_t i = 0; i < sizeof(digest); ++i)
            sprintf(&str[i * 2], "%02x", digest[i]);
        EXPECT_STREQ(it.exp, str);
    }
}

TEST(Hash, Md4) 
{
    const Pair test[] = {
        { "",
            "31d6cfe0d16ae931b73c59d7e0c089c0" },
        { "a",
            "bde52cb31de33e46245e05fbdbd6fb24" },
        { "abc",
            "a448017aaf21d8525fc10ae87aa6729d" },
        { "message digest",
            "d9130a8164549fe818874806e1c7014b" },
        { "abcdefghijklmnopqrstuvwxyz",
            "d79e1c308aa5bbcdeea8ed63df412da9" },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            "043f8582f241db351ce627e153e7f0e4" },
        { "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "e33b4ddc9c38f2199c3e7b164fcc0536" },
    };

    MD4 hash;

    uint8_t digest[decltype(hash)::SIZE];
    char str[decltype(hash)::SIZE * 2 + 1];

    for (auto it : test) {
        hash(it.msg, strlen(it.msg), digest);
        for (size_t i = 0; i < sizeof(digest); ++i)
            sprintf(&str[i * 2], "%02x", digest[i]);
        EXPECT_STREQ(it.exp, str);
    }
}

// TEST(Hash, MD5) 
// {
//     constexpr Pair test[] = {
//         { "",
//             "31d6cfe0d16ae931b73c59d7e0c089c0" },
//         { "a",
//             "bde52cb31de33e46245e05fbdbd6fb24" },
//         { "abc",
//             "a448017aaf21d8525fc10ae87aa6729d" },
//         { "message digest",
//             "d9130a8164549fe818874806e1c7014b" },
//         { "abcdefghijklmnopqrstuvwxyz",
//             "d79e1c308aa5bbcdeea8ed63df412da9" },
//         { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
//             "043f8582f241db351ce627e153e7f0e4" },
//         { "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
//             "e33b4ddc9c38f2199c3e7b164fcc0536" },
//     };

//     MD5 hash;

//     uint8_t digest[decltype(hash)::SIZE];
//     char str[decltype(hash)::SIZE * 2];

//     for (auto it : test) {

//         hash(it.msg, strlen(it.msg), digest);
        
//         for (size_t i = 0; i < sizeof(digest); ++i)
//             sprintf(&str[i * 2], "%02x", digest[i]);

//         ASSERT_FALSE(strcmp(it.exp, str));
//     }
// }

// TEST(MAC, HMAC)
// {

// }
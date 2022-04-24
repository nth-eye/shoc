#include <gtest/gtest.h>
#include "shoc/hotp.h"

using namespace shoc;

// void hotp_sha_1_test()
// {
//     SHA1 sha;

//     constexpr const char *key = "12345678901234567890";
//     constexpr const uint32_t expected[] = { 755224, 287082, 359152, 969429, 338314, 254676, 287922, 162583, 399871, 520489 };

//     printf("Test:    '%s'\n\n", key);
//     for (uint64_t i = 0; i < 10; ++i) {

//         uint32_t code = hotp(sha, (uint8_t*) key, strlen(key), i, 6);

//         printf("Counter:  %lu\n", i);
//         printf("Result:   %u\n", code);
//         printf("Correct:  %s\n\n", code == expected[i] ? "yes" : "no");
//     }
// }
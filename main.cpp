#include <cstring>
#include <cstdio>
#include <ctime>

#include "sha3.h"
#include "hotp.h"

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

template<size_t N = 1, class Fn, class ...Args>
clock_t measure_time(Fn &&fn, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        fn(args...);
    clock_t end = clock();

    return (end - begin); // / N;
}
    
int main()
{
    // printf("%u %u %u \n", 0, Keccak<0>::B, Keccak<0>::R);
    // printf("%u %u %u \n", 1, Keccak<1>::B, Keccak<1>::R);
    // printf("%u %u %u \n", 2, Keccak<2>::B, Keccak<2>::R);
    // printf("%u %u %u \n", 3, Keccak<3>::B, Keccak<3>::R);
    // printf("%u %u %u \n", 4, Keccak<4>::B, Keccak<4>::R);
    // printf("%u %u %u \n", 5, Keccak<5>::B, Keccak<5>::R);
    // printf("%u %u %u \n", 6, Keccak<6>::B, Keccak<6>::R);
    // printf("1: %lu clock_t \n", measure_time<1000000>(md2_test_1));
    // printf("2: %lu clock_t \n", measure_time<1000000>(md2_test_2));
    // printf("1: %lu clock_t \n", measure_time<1000000>(md2_test_1));
    // printf("2: %lu clock_t \n", measure_time<1000000>(md2_test_2));
    // printf("1: %lu clock_t \n", measure_time<1000000>(md2_test_1));

    // // ANCHOR: HOTP-SHA-1

    // hotp_sha_1_test();
}

#include <cstdio>
#include <ctime>
#include "shoc/gimli.h"
#include "shoc/xoodoo.h"
// #include "shoc/gcm.h"
#include "shoc/aes.h"
#include "shoc/modes/cfb.h"
#include "shoc/modes/ofb.h"
#include "shoc/modes/ctr.h"
#include "shoc/_log.h"

template<size_t N = 1, class Fn, class ...Args>
clock_t measure_time(Fn &&fn, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        fn(args...);
    clock_t end = clock();

    return (end - begin); // / N;
}

// template<size_t N = 1, class Fn, class Ptr, class ...Args>
// clock_t measure_time(Fn &&fn, Ptr *ptr, Args &&...args)
// {
//     clock_t begin = clock();
//     for (size_t i = 0; i < N; ++i) 
//         (ptr->*fn)(args...);
//     clock_t end = clock();

//     return (end - begin); // / N;
// }
    
int main()
{
    uint8_t array[16];

    for (int i = 0xfe; auto &it : array)
        it = ++i;

    // shoc::log_hex(array, sizeof(array), "plain");
    // shoc::log_bits(array, sizeof(array) * 8, 0);

    // for (int i = 0xfe; auto &it : array)
    //     it = ++i;

    // shoc::shift_left(array);
    // shoc::log_hex(array, sizeof(array), "shifted_1");
    // shoc::log_bits(array, sizeof(array) * 8, 0);

    // for (int i = 0xfe; auto &it : array)
    //     it = ++i;

    // shoc::shift_right_reflected(array);
    // shoc::log_hex(array, sizeof(array), "shifted_2");
    // shoc::log_bits(array, sizeof(array) * 8, 0);



    // uint32_t u32[4] = { 
    //     0x020100ff, 
    //     0x06050403,
    //     0x0a090807,
    //     0x0e0d0c0b,
    // };

    // shoc::log_hex(u32, sizeof(u32), "plain_1");
    // shoc::log_bits(u32, sizeof(u32) * 8, 0);
    // shoc::shift_right(u32, sizeof(u32));
    // shoc::log_hex(u32, sizeof(u32), "shifted_2");
    // shoc::log_bits(u32, sizeof(u32) * 8, 0);

    // uint8_t key[16];
    // uint8_t icb[16];
    // uint8_t x[1000];
    // uint8_t y[1000];

    // // shoc::Aes ciph = {key};

    // printf("1: %lu clock_t \n", measure_time<1000000>(shoc::ctr_encrypt<shoc::Aes>, key, icb, x, y, sizeof(x)));
    // printf("2: %lu clock_t \n", measure_time<1000000>(shoc::ctr_encrypt_2<shoc::Aes>, key, icb, x, y, sizeof(x)));
    // printf("1: %lu clock_t \n", measure_time<1000000>(shoc::ctr_encrypt<shoc::Aes>, key, icb, x, y, sizeof(x)));
    // printf("2: %lu clock_t \n", measure_time<1000000>(shoc::ctr_encrypt_2<shoc::Aes>, key, icb, x, y, sizeof(x)));
    // printf("1: %lu clock_t \n", measure_time<1000000>(shoc::ctr_encrypt<shoc::Aes>, key, icb, x, y, sizeof(x)));
    // printf("2: %lu clock_t \n", measure_time<1000000>(shoc::ctr_encrypt_2<shoc::Aes>, key, icb, x, y, sizeof(x)));

    // shoc::log_hex(y, sizeof(y), "y");
    // shoc::log_bits(y, sizeof(y) * 8, 0);

    // uint8_t x[16];
    // uint8_t y[16];
    // uint8_t z[16];

    // shoc::fill(x, 1, sizeof(x));
    // shoc::fill(y, 2, sizeof(y));
    // shoc::gmul(x, y, z);

    // shoc::log_hex(x, sizeof(x), "x");
    // shoc::log_hex(y, sizeof(y), "y");
    // shoc::log_hex(z, sizeof(z), "z_1");

    // printf("1: %lu clock_t \n", measure_time<10000000>(shoc::gmul, x, y, z));
    // printf("2: %lu clock_t \n", measure_time<10000000>(shoc::gmul_arr, x, y, z));
}

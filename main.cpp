#include <cstdio>
#include <ctime>
#include "shoc/gimli.h"
#include "shoc/xoodoo.h"
#include "shoc/aes_gcm.h"
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
    // 0b10000111;
    // 0b11100001;
    // 0xe1;

    // shoc::Gimli state;

    // uint8_t data[256];
    // uint8_t digest[32];
    // size_t len = sizeof(data);

    // shoc::Xoodoo state;

    // state.theta();
    // log_hex(state.u8, sizeof(state.u8), "theta");
    // state.rhowest();
    // log_hex(state.u8, sizeof(state.u8), "rhowest");
    // state.iota(0);
    // log_hex(state.u8, sizeof(state.u8), "iota");
    // state.chi();
    // log_hex(state.u8, sizeof(state.u8), "chi");
    // state.rhoeast();
    // log_hex(state.u8, sizeof(state.u8), "rhoeast");

    uint8_t array[16];

    for (int i = 0xfe; auto &it : array)
        it = ++i;

    shoc::log_hex(array, sizeof(array), "plain_1");
    shoc::log_bits(array, sizeof(array) * 8, 0);
    shoc::shift_from_lsb(array, sizeof(array));
    shoc::log_hex(array, sizeof(array), "shifted_2");
    shoc::log_bits(array, sizeof(array) * 8, 0);

    uint32_t u32[4] = { 
        0x020100ff, 
        0x06050403,
        0x0a090807,
        0x0e0d0c0b,
    };

    // shoc::log_hex(u32, sizeof(u32), "plain_1");
    // shoc::log_bits(u32, sizeof(u32) * 8, 0);
    // shoc::shift_from_lsb(u32, sizeof(u32));
    // shoc::log_hex(u32, sizeof(u32), "shifted_2");
    // shoc::log_bits(u32, sizeof(u32) * 8, 0);

    // printf("1: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint32_t>, u32, 4));
    // printf("2: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint8_t>, (uint8_t*) u32, sizeof(u32)));
    // printf("1: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint32_t>, u32, 4));
    // printf("2: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint8_t>, (uint8_t*) u32, sizeof(u32)));
    // printf("1: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint32_t>, u32, 4));
    // printf("2: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint8_t>, (uint8_t*) u32, sizeof(u32)));

    // printf("1: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint32_t>, u32, 4));
    // printf("2: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb_2<uint32_t>, u32, 4));
    // printf("1: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint32_t>, u32, 4));
    // printf("2: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb_2<uint32_t>, u32, 4));
    // printf("1: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb<uint32_t>, u32, 4));
    // printf("2: %lu clock_t \n", measure_time<100000000>(shoc::shift_from_lsb_2<uint32_t>, u32, 4));

    shoc::log_hex(u32, sizeof(u32), "u32");
    shoc::log_bits(u32, sizeof(u32) * 8, 0);

    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));

    // for (int i = 0xfe; auto &it : array)
    //     it = ++i;

    // shoc::log_hex(array, sizeof(array), "plain_2");
    // shoc::log_bits(array, sizeof(array) * 8, 0);
    // shoc::shift_right_block(array);
    // shoc::log_hex(array, sizeof(array), "shifted_2");
    // shoc::log_bits(array, sizeof(array) * 8, 0);

    uint8_t x[16];
    uint8_t y[16];
    uint8_t z[16];

    shoc::fill(x, 1, sizeof(x));
    shoc::fill(y, 2, sizeof(y));
    shoc::gf128_mul_1(x, y, z);

    shoc::log_hex(x, sizeof(x), "x");
    shoc::log_hex(y, sizeof(y), "y");
    shoc::log_hex(z, sizeof(z), "z_1");

    shoc::fill(x, 1, sizeof(x));
    shoc::fill(y, 2, sizeof(y));
    shoc::gf128_mul_2(x, y, z);

    shoc::log_hex(x, sizeof(x), "x");
    shoc::log_hex(y, sizeof(y), "y");
    shoc::log_hex(z, sizeof(z), "z_2");

    shoc::fill(x, 1, sizeof(x));
    shoc::fill(y, 2, sizeof(y));
    shoc::gf128_mul_3(x, y, z);

    shoc::log_hex(x, sizeof(x), "x");
    shoc::log_hex(y, sizeof(y), "y");
    shoc::log_hex(z, sizeof(z), "z_3");

    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
}

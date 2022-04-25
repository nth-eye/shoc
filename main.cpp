#include <cstdio>
#include <ctime>
#include "shoc/gimli.h"
#include "shoc/xoodoo.h"

template<size_t N = 1, class Fn, class ...Args>
clock_t measure_time(Fn &&fn, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        fn(args...);
    clock_t end = clock();

    return (end - begin); // / N;
}

template<size_t N = 1, class Fn, class Ptr, class ...Args>
clock_t measure_time(Fn &&fn, Ptr *ptr, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        (ptr->*fn)(args...);
    clock_t end = clock();

    return (end - begin); // / N;
}

void log_hex(const void *data, size_t len, const char *str)
{
    printf("[%8lx]  \"%s\"\n", len, str);

    if (!data || !len)
        return;
    auto p = static_cast<const uint8_t*>(data);

    for (size_t i = 0; i < len; ++i) {

        if (!(i & 15))
            printf("[%08llx]  ", (long long unsigned) p + i);
        printf("%02x ", p[i]);
        
        if ((i & 7) == 7)
            putchar(' ');

        if ((i & 15) == 15) {
            printf("|");
            for (int j = 15; j >= 0; --j) {
                char c = p[i - j];
                printf("%c", isprint(c) ? c : '.');
            }
            printf("|\n");
        }
    }
    int rem = len - ((len >> 4) << 4);
    if (rem) {
        printf("%*c |", (16 - rem) * 3 + ((~rem & 8) >> 3), ' ');

        for (int j = rem; j; --j) {
            char c = p[len - j];
            printf("%c", isprint(c) ? c : '.');
        }
        for (int j = 0; j < 16 - rem; ++j)
            putchar('.');
        printf("|\n");
    }
}
    
int main()
{
    // shoc::Gimli state;

    // uint8_t data[256];
    // uint8_t digest[32];
    // size_t len = sizeof(data);

    shoc::Xoodoo state;

    state.theta();
    log_hex(state.u8, sizeof(state.u8), "theta");
    state.rhowest();
    log_hex(state.u8, sizeof(state.u8), "rhowest");
    state.iota(0);
    log_hex(state.u8, sizeof(state.u8), "iota");
    state.chi();
    log_hex(state.u8, sizeof(state.u8), "chi");
    state.rhoeast();
    log_hex(state.u8, sizeof(state.u8), "rhoeast");

    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round, &state));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&shoc::Xoodoo::round_2, &state));
}

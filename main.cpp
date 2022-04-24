#include <cstdio>
#include <ctime>
#include "shoc/gimli.h"

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
    
int main()
{
    // shoc::Gimli state;

    // uint8_t data[256];
    // uint8_t digest[32];
    // size_t len = sizeof(data);

    // printf("1: %lu clock_t \n", measure_time<1000000>(&shoc::Gimli::operator(), &state, data, len, digest));
    // printf("2: %lu clock_t \n", measure_time<1000000>(&shoc::Gimli::hash, &state, data, len, digest));
    // printf("1: %lu clock_t \n", measure_time<1000000>(&shoc::Gimli::operator(), &state, data, len, digest));
    // printf("2: %lu clock_t \n", measure_time<1000000>(&shoc::Gimli::hash, &state, data, len, digest));
    // printf("1: %lu clock_t \n", measure_time<1000000>(&shoc::Gimli::operator(), &state, data, len, digest));
    // printf("2: %lu clock_t \n", measure_time<1000000>(&shoc::Gimli::hash, &state, data, len, digest));
}

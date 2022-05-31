#include <cstdio>
#include <ctime>
#include "shoc/xoodoo.h"
#include "shoc/log.h"

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

}

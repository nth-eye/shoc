#include <cstdio>
#include "shoc/util.h"
#include "shoc/ecc/crc.h"
#include "shoc/_experimental/elliptic.h"
    
int main()
{
    printf("0x%04x\n", shoc::bitswap(uint16_t(0x0001)));
    printf("0x%04x\n", shoc::byteswap(uint16_t(0x0001)));

    // printf("%d %% %d = %d \n", -9, 19, shoc::ff_mod(-9, 19));
    // printf("%d ** %d = %d \n", 7, 3, shoc::ff_pow(7, 3, 19));

    using fint_t = shoc::ff_int<int, 223>;

    constexpr auto a = fint_t{0};
    constexpr auto b = fint_t{7};
    constexpr auto x = fint_t{192};
    constexpr auto y = fint_t{105};
    auto p1 = shoc::ff_point<fint_t, a, b>{x, y};

    printf("%d \n", p1.is_infinity());
}

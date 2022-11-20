#include <cstdio>
#include "shoc/util.h"
#include "shoc/ecc/crc.h"
#include "shoc/_experimental/elliptic.h"
    
int main()
{
    // printf("0x%04x\n", shoc::bitswap(uint16_t(0x0001)));
    // printf("0x%04x\n", shoc::byteswap(uint16_t(0x0001)));

    // printf("%d %% %d = %d \n", -9, 19, shoc::ff_mod(-9, 19));
    // printf("%d ** %d = %d \n", 7, 3, shoc::ff_pow(7, 3, 19));

    using fint = shoc::ff_int<int, 223>;
    using fpoint = shoc::ff_point<fint, 0, 7>;

    // auto p1 = fpoint{192, 105};
    // auto p2 = fpoint{17, 56};
    // auto p3 = p1 + p2;

    // printf("%d %d %d \n", p3.is_infinity(), p3.get_x(), p3.get_y());

    auto p = fpoint{47, 71};

    for (int i = 1; i <= 21; ++i) {
        auto r = p * i;
        printf("%2d * (%d, %d) = (%3d, %3d) | %d \n", i, p.get_x(), p.get_y(), r.get_x(), r.get_y(), r.is_infinity());
    }
}

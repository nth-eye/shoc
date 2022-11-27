#include <cstdio>
#include "shoc/util.h"
#include "shoc/ecc/crc.h"
#include "shoc/_experimental/elliptic.h"
#include "shoc/_experimental/bigint.h"
    
int main()
{
    // printf("0x%04x\n", shoc::bitswap(uint16_t(0x0001)));
    // printf("0x%04x\n", shoc::byteswap(uint16_t(0x0001)));

    // printf("%d %% %d = %d \n", -9, 19, shoc::ff_mod(-9, 19));
    // printf("%d ** %d = %d \n", 7, 3, shoc::ff_pow(7, 3, 19));

    // using fint = shoc::ff_int<int, 223>;
    // using fpoint = shoc::ff_point<fint, 0, 7>;

    // auto p1 = fpoint{192, 105};
    // auto p2 = fpoint{17, 56};
    // auto p3 = p1 + p2;

    // printf("%d %d %d \n", p3.is_infinity(), p3.get_x(), p3.get_y());

    // auto p = fpoint{47, 71};

    // for (int i = 1; i <= 21; ++i) {
    //     auto r = p * i;
    //     printf("%2d * (%d, %d) = (%3d, %3d) | %d \n", i, p.get_x(), p.get_y(), r.get_x(), r.get_y(), r.is_infinity());
    // }

    using bigint_base = uint32_t;
    using uint128 = shoc::bigint<bigint_base, 128>;

    printf("sizeof(bigint_base):    %lu \n", sizeof(bigint_base));
    printf("sizeof(uint128):        %lu \n", sizeof(uint128));
    // shoc::bigint<128> 

    uint128 a(0x1337);
    uint128 b(0xffffffffffffffffull, 1);
    uint128 c{{0xdead, 0x1}};
    uint128 d(1);
    uint128 e(0xffffffffffffffffull, 2);
    // uint128 f("12345");
    uint128 g = {{0x1337, 0x1}};

    a.print();
    // b.print();
    c.print();
    // d.print();
    // e.print();
    // f.print();
    // g.print();

    // (c - a).print();
    (a - c).print();
    // (g - c).print();
    (b + d).print();
    (e + d).print();
    (a + g).print();
}

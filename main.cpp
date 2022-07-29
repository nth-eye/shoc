#include <cstdio>
#include "shoc/util.h"
#include "shoc/ecc/crc.h"
    
int main()
{
    printf("0x%04x\n", shoc::bitswap(uint16_t(0x01)));

    printf("0x%02x\n", shoc::crc_slow<uint8_t, 0x2f, 0xff, 0xff, false, false>("123456789", 9));
    printf("0x%02x\n", shoc::crc_fast<uint8_t, 0x2f, 0xff, 0xff, false, false>("123456789", 9));

    printf("0x%04x\n", shoc::crc_slow<uint16_t, 0x8005, 0xFFFF, 0x0000, true, true>("0123456789abcdef", 16));
    printf("0x%04x\n", shoc::crc_fast<uint16_t, 0x8005, 0xFFFF, 0x0000, true, true>("0123456789abcdef", 16));

    // auto tbl = shoc::crc_table<uint16_t>(0x8005);

    // for (auto a : tbl)
    //     printf("1) 0x%04x\n", a);
}

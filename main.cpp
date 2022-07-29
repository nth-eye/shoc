#include <cstdio>
#include "shoc/util.h"
#include "shoc/ecc/crc.h"
    
int main()
{
    printf("0x%04x\n", shoc::bitswap(uint16_t(0x0001)));
    printf("0x%04x\n", shoc::byteswap(uint16_t(0x0001)));
}

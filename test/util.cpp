#include "test.h"
#include "shoc/util.h"

namespace shoc {
namespace {

}

TEST(Util, Rol)
{
    ASSERT_EQ(rol<uint8_t>(0x55, 1), 0xaa);
    ASSERT_EQ(rol<uint8_t>(0x55, 8), 0x55);
    ASSERT_EQ(rol<uint32_t>(0x00000055,  2), 0x00000154);
    ASSERT_EQ(rol<uint32_t>(0x00000055, 31), 0x8000002a);
}

TEST(Util, Ror)
{
    ASSERT_EQ(ror<uint8_t>(0x55, 1), 0xaa);
    ASSERT_EQ(ror<uint8_t>(0x55, 8), 0x55);
    ASSERT_EQ(ror<uint32_t>(0x00000055,  2), 0x40000015);
    ASSERT_EQ(ror<uint32_t>(0x00000055, 31), 0x000000aa);
}

TEST(Util, Copy)
{

}

TEST(Util, Fill)
{

}

TEST(Util, Zero)
{

}

TEST(Util, Xorb)
{
    
}

TEST(Util, Bitswap)
{
    ASSERT_EQ(bitswap<uint8_t>(0xf0), 0b00001111);
    ASSERT_EQ(bitswap<uint16_t>(0xb307), 0b11100000'11001101);
    ASSERT_EQ(bitswap<uint32_t>(0x9e9cf800), 0b00000000'00011111'00111001'01111001);
    ASSERT_EQ(bitswap<uint64_t>(0x0123456789abcdef), 0b11110111'10110011'11010101'10010001'11100110'10100010'11000100'10000000);
}

TEST(Util, Byteswap)
{
    ASSERT_EQ(byteswap<uint8_t>(0xde), 0xde);
    ASSERT_EQ(byteswap<uint16_t>(0xdead), 0xadde);
    ASSERT_EQ(byteswap<uint32_t>(0x01234567), 0x67452301);
    ASSERT_EQ(byteswap<uint64_t>(0xefcdab8967452301), 0x0123456789abcdef);
}

}
#include "test.h"
#include "shoc/cipher/chacha.h"

namespace shoc {
namespace {



}

TEST(Cipher, ChaChaQuarterRound)
{
    // impl::chacha::word a = 0x11111111;
    // impl::chacha::word b = 0x01020304;
    // impl::chacha::word c = 0x9b8d6f43;
    // impl::chacha::word d = 0x01234567;

    // impl::chacha::quarter_round(a, b, c, d);

    // ASSERT_EQ(a, 0xea2a92f4);
    // ASSERT_EQ(b, 0xcb1cf8ce);
    // ASSERT_EQ(c, 0x4581472e);
    // ASSERT_EQ(d, 0x5881c4bb);
}

TEST(Cipher, ChaCha)
{
    
}

}
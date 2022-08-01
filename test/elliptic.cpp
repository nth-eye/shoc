#include <gtest/gtest.h>
#include "shoc/_experimental/elliptic.h"

using namespace shoc;

TEST(Elliptic, A)
{
    EXPECT_EQ(ff_add(44, 33, 57), 20);
    EXPECT_EQ(ff_sub(9, 29, 57), 37);
    EXPECT_EQ(ff_add(59, 49, 57), 51);
    EXPECT_EQ(ff_sub(22, 38, 57), 41);

    EXPECT_EQ(ff_mod(95 * 45 * 31, 97), 23);
    EXPECT_EQ(ff_mod(17 * 13 * 19 * 44, 97), 68);
    EXPECT_EQ(ff_mul(ff_pow(12, 7, 97), ff_pow(77, 49, 97), 97), 63);

    EXPECT_EQ(ff_div(3, 24, 31), 4);
    EXPECT_EQ(ff_pow(17, -3, 31), 29);
    EXPECT_EQ(ff_mul(ff_pow(4, -4, 31), 11, 31), 13);
}
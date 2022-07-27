#include <gtest/gtest.h>
#include <span>
#include "shoc/otp/hotp.h"
#include "shoc/hash/sha1.h"

using namespace shoc;

TEST(Hotp, Sha1)
{
    const char *key = "12345678901234567890";
    const uint32_t exp[] = { 755224, 287082, 359152, 969429, 338314, 254676, 287922, 162583, 399871, 520489 };

    for (uint64_t i = 0; i < std::span{exp}.size(); ++i)
        EXPECT_EQ(hotp<Sha1>(key, strlen(key), i, 6), exp[i]);
}
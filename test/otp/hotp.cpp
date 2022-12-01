#include "test.h"
#include "shoc/otp/hotp.h"
#include "shoc/hash/sha1.h"

namespace shoc {

TEST(Hotp, Sha1)
{
    static constexpr byte key[]     = "12345678901234567890";
    static constexpr auto key_view  = span_i{key, sizeof(key) - 1};
    static constexpr uint32_t exp[] = { 755224, 287082, 359152, 969429, 338314, 254676, 287922, 162583, 399871, 520489 };
    static constexpr auto ret = [&] {
        std::array<uint32_t, countof(exp)> arr;
        for (uint64_t i = 0; i < countof(exp); ++i)
            arr[i] = hotp<sha1>(key_view, i, 6);
        return arr;
    }();

    for (uint64_t i = 0; i < countof(exp); ++i)
        ASSERT_EQ(ret[i], exp[i]);
        
    for (uint64_t i = 0; i < countof(exp); ++i)
        ASSERT_EQ(hotp<sha1>(key_view, i, 6), exp[i]);
}

}
#include <gtest/gtest.h>
#undef NDEBUG
#include "utl/bit_vector.h"

template<size_t N, size_t G = 4>
using bitvector8 = utl::bit_vector<uint8_t, N, G>;

template<size_t N, size_t G = 4>
using bitvector32 = utl::bit_vector<uint32_t, N, G>;

TEST(BitVector, Capacity)
{
    ASSERT_EQ((bitvector8<1, 1>::capacity()), 1);
    ASSERT_EQ((bitvector8<513, 1>::capacity()), 513);
    ASSERT_EQ((bitvector8<2049, 4>::capacity()), 2049);
    ASSERT_EQ((bitvector8<3585, 7>::capacity()), 3585);
    ASSERT_EQ((bitvector32<1024, 1>::capacity()), 1024);
    ASSERT_EQ((bitvector32<131072, 4>::capacity()), 131072);
    ASSERT_EQ((bitvector32<1015809, 31>::capacity()), 1015809);
}

TEST(BitVector, Depth)
{
    ASSERT_EQ((bitvector8<1, 1>::depth()), 1);
    ASSERT_EQ((bitvector8<8, 1>::depth()), 1);
    ASSERT_EQ((bitvector8<9, 1>::depth()), 2);
    ASSERT_EQ((bitvector8<64, 1>::depth()), 2);
    ASSERT_EQ((bitvector8<65, 1>::depth()), 3);
    ASSERT_EQ((bitvector8<512, 1>::depth()), 3);
    ASSERT_EQ((bitvector8<513, 1>::depth()), 4);

    ASSERT_EQ((bitvector8<1, 4>::depth()), 1);
    ASSERT_EQ((bitvector8<32, 4>::depth()), 1);
    ASSERT_EQ((bitvector8<33, 4>::depth()), 2);
    ASSERT_EQ((bitvector8<256, 4>::depth()), 2);
    ASSERT_EQ((bitvector8<257, 4>::depth()), 3);
    ASSERT_EQ((bitvector8<2048, 4>::depth()), 3);
    ASSERT_EQ((bitvector8<2049, 4>::depth()), 4);

    ASSERT_EQ((bitvector8<1, 7>::depth()), 1);
    ASSERT_EQ((bitvector8<56, 7>::depth()), 1);
    ASSERT_EQ((bitvector8<57, 7>::depth()), 2);
    ASSERT_EQ((bitvector8<448, 7>::depth()), 2);
    ASSERT_EQ((bitvector8<449, 7>::depth()), 3);
    ASSERT_EQ((bitvector8<3584, 7>::depth()), 3);
    ASSERT_EQ((bitvector8<3585, 7>::depth()), 4);

    ASSERT_EQ((bitvector32< 1, 1>::depth()), 1);
    ASSERT_EQ((bitvector32< 32, 1>::depth()), 1);
    ASSERT_EQ((bitvector32< 33, 1>::depth()), 2);
    ASSERT_EQ((bitvector32< 1024, 1>::depth()), 2);
    ASSERT_EQ((bitvector32< 1025, 1>::depth()), 3);
    ASSERT_EQ((bitvector32< 32768, 1>::depth()), 3);
    ASSERT_EQ((bitvector32< 32769, 1>::depth()), 4);

    ASSERT_EQ((bitvector32<1, 4>::depth()), 1);
    ASSERT_EQ((bitvector32<128, 4>::depth()), 1);
    ASSERT_EQ((bitvector32<129, 4>::depth()), 2);
    ASSERT_EQ((bitvector32<4096, 4>::depth()), 2);
    ASSERT_EQ((bitvector32<4097, 4>::depth()), 3);
    ASSERT_EQ((bitvector32<131072, 4>::depth()), 3);
    ASSERT_EQ((bitvector32<131073, 4>::depth()), 4);

    ASSERT_EQ((bitvector32<1, 31>::depth()), 1);
    ASSERT_EQ((bitvector32<992, 31>::depth()), 1);
    ASSERT_EQ((bitvector32<993, 31>::depth()), 2);
    ASSERT_EQ((bitvector32<31744, 31>::depth()), 2);
    ASSERT_EQ((bitvector32<31745, 31>::depth()), 3);
    ASSERT_EQ((bitvector32<1015808, 31>::depth()), 3);
    ASSERT_EQ((bitvector32<1015809, 31>::depth()), 4);
}

TEST(BitVector, Words)
{
    ASSERT_EQ((bitvector8<1, 1>::words()), 1);
    ASSERT_EQ((bitvector8<8, 1>::words()), 1);
    ASSERT_EQ((bitvector8<9, 1>::words()), 3);
    ASSERT_EQ((bitvector8<64, 1>::words()), 9);
    ASSERT_EQ((bitvector8<65, 1>::words()), 12);
    ASSERT_EQ((bitvector8<512, 1>::words()), 73);
    ASSERT_EQ((bitvector8<513, 1>::words()), 77);

    ASSERT_EQ((bitvector8<1, 4>::words()), 1);
    ASSERT_EQ((bitvector8<32, 4>::words()), 4);
    ASSERT_EQ((bitvector8<33, 4>::words()), 6);
    ASSERT_EQ((bitvector8<256, 4>::words()), 36);
    ASSERT_EQ((bitvector8<257, 4>::words()), 39);
    ASSERT_EQ((bitvector8<2048, 4>::words()), 292);
    ASSERT_EQ((bitvector8<2049, 4>::words()), 296);

    ASSERT_EQ((bitvector8<1, 7>::words()), 1);
    ASSERT_EQ((bitvector8<56, 7>::words()), 7);
    ASSERT_EQ((bitvector8<57, 7>::words()), 9);
    ASSERT_EQ((bitvector8<448, 7>::words()), 63);
    ASSERT_EQ((bitvector8<449, 7>::words()), 66);
    ASSERT_EQ((bitvector8<3584, 7>::words()), 511);
    ASSERT_EQ((bitvector8<3585, 7>::words()), 515);

    ASSERT_EQ((bitvector32<1, 1>::words()), 1);
    ASSERT_EQ((bitvector32<32, 1>::words()), 1);
    ASSERT_EQ((bitvector32<33, 1>::words()), 3);
    ASSERT_EQ((bitvector32<1024, 1>::words()), 33);
    ASSERT_EQ((bitvector32<1025, 1>::words()), 36);
    ASSERT_EQ((bitvector32<32768, 1>::words()), 1057);
    ASSERT_EQ((bitvector32<32769, 1>::words()), 1061);

    ASSERT_EQ((bitvector32<1, 4>::words()), 1);
    ASSERT_EQ((bitvector32<128, 4>::words()), 4);
    ASSERT_EQ((bitvector32<129, 4>::words()), 6);
    ASSERT_EQ((bitvector32<4096, 4>::words()), 132);
    ASSERT_EQ((bitvector32<4097, 4>::words()), 135);
    ASSERT_EQ((bitvector32<131072, 4>::words()), 4228);
    ASSERT_EQ((bitvector32<131073, 4>::words()), 4232);

    ASSERT_EQ((bitvector32<1, 31>::words()), 1);
    ASSERT_EQ((bitvector32<992, 31>::words()), 31);
    ASSERT_EQ((bitvector32<993, 31>::words()), 33);
    ASSERT_EQ((bitvector32<31744, 31>::words()), 1023);
    ASSERT_EQ((bitvector32<31745, 31>::words()), 1026);
    ASSERT_EQ((bitvector32<1015808, 31>::words()), 32767);
    ASSERT_EQ((bitvector32<1015809, 31>::words()), 32771);
}

TEST(BitVector, GetSetClr)
{
    static constexpr auto size = 7;
    static constexpr auto midpoint = size / 2;

    bitvector8<size> bv;

    for (size_t i = 0; i < size; ++i)
        ASSERT_EQ(bv[i], false);

    for (size_t i = 0; i < midpoint; ++i)
        bv.set(i);

    for (size_t i = midpoint + 1; i < size; ++i)
        bv.set(i);

    for (size_t i = 0; i < midpoint; ++i)
        ASSERT_EQ(bv[i], true);

    for (size_t i = midpoint + 1; i < size; ++i)
        ASSERT_EQ(bv[i], true);

    ASSERT_EQ(bv[midpoint], false);

    for (size_t i = 0; i < midpoint; ++i)
        bv.clr(i);

    for (size_t i = 0; i <= midpoint; ++i)
        ASSERT_EQ(bv[i], false);

    for (size_t i = midpoint + 1; i < size; ++i)
        ASSERT_EQ(bv[i], true);
}

TEST(BitVector, AcquireAny)
{
    bitvector32<1337> bv;

    for (size_t i = 0; i < bv.capacity(); ++i)
        ASSERT_EQ(bv.acquire_any(), i);
    for (size_t i = 0; i < bv.capacity(); ++i)
        ASSERT_EQ(bv.acquire_any(), bv.capacity());

    bv.reset();

    for (size_t i = 0; i < bv.capacity(); ++i)
        ASSERT_EQ(bv.acquire_any(), i);
    for (size_t i = 0; i < bv.capacity(); ++i)
        ASSERT_EQ(bv.acquire_any(), bv.capacity());

    bv.clr(0);
    bv.clr(228);
    bv.clr(666);
    bv.clr(bv.capacity() - 1);

    ASSERT_EQ(bv.acquire_any(), 0);
    ASSERT_EQ(bv.acquire_any(), 228);
    ASSERT_EQ(bv.acquire_any(), 666);
    ASSERT_EQ(bv.acquire_any(), bv.capacity() - 1);
    ASSERT_EQ(bv.acquire_any(), bv.capacity());
    ASSERT_EQ(bv.acquire_any(), bv.capacity());
}

TEST(BitVector, Death)
{
    bitvector8<7> bv;

    ASSERT_DEATH(bv[bv.capacity() + 1], "");
    ASSERT_DEATH(bv.clr(bv.capacity() + 1), "");
    ASSERT_DEATH(bv.set(bv.capacity() + 1), "");
}
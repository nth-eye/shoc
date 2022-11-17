#include <gtest/gtest.h>
#include "utl/math.h"
#include "_util.h"

using namespace utl;

TEST(Math, Radians)
{
    assert_near(radians(0),      0);
    assert_near(radians(15),     M_PI/12);
    assert_near(radians(30),     M_PI/6);
    assert_near(radians(45),     M_PI/4);
    assert_near(radians(90),     M_PI/2);
    assert_near(radians(180),    M_PI);
    assert_near(radians(360),    M_PI*2);
    assert_near(radians(540),    M_PI*3);
    assert_near(radians(66.6),   1.1623892818);

    assert_near(radians(-0),     0);
    assert_near(radians(-15),    -M_PI/12);
    assert_near(radians(-30),    -M_PI/6);
    assert_near(radians(-45),    -M_PI/4);
    assert_near(radians(-90),    -M_PI/2);
    assert_near(radians(-180),   -M_PI);
    assert_near(radians(-360),   -M_PI*2);
    assert_near(radians(-540),   -M_PI*3);
    assert_near(radians(-66.6),  -1.1623892818);
}

TEST(Math, Degrees)
{
    assert_near(degrees(0),          0);
    assert_near(degrees(1),          57.2957795131);
    assert_near(degrees(3.14),       179.9087476711);
    assert_near(degrees(M_PI),       180);
    assert_near(degrees(M_PI*2),     360);
    assert_near(degrees(M_PI*3),     540);
    assert_near(degrees(M_PI/4),     45);
    assert_near(degrees(M_PI/2),     90);

    assert_near(degrees(-0),         0);
    assert_near(degrees(-1),         -57.2957795131);
    assert_near(degrees(-3.14),      -179.9087476711);
    assert_near(degrees(-M_PI),      -180);
    assert_near(degrees(-M_PI*2),    -360);
    assert_near(degrees(-M_PI*3),    -540);
    assert_near(degrees(-M_PI/4),    -45);
    assert_near(degrees(-M_PI/2),    -90);
}

TEST(Math, UnsignedCeil)
{
    ASSERT_EQ(uceil(0, 0), 0);
    ASSERT_EQ(uceil(0, 1), 0);
    ASSERT_EQ(uceil(4, 2), 2);
    ASSERT_EQ(uceil(5, 2), 3);
    ASSERT_EQ(uceil(42, 11), 4); 
}

TEST(Math, IntMap)
{
    ASSERT_EQ(imap(0, 0, 1, 0, 0), 0);
    ASSERT_EQ(imap(1, 0, 1, 0, 0), 0);
    ASSERT_EQ(imap(0, 0, 1, 0, 1), 0);
    ASSERT_EQ(imap(1, 0, 1, 0, 1), 1);
    ASSERT_EQ(imap(0xff, 0x00, 0xff, 0, 100), 100);
    ASSERT_EQ(imap(0x80, 0x00, 0xff, 0, 100), 50);
    ASSERT_EQ(imap(0x40, 0x00, 0xff, 0, 100), 25);
    ASSERT_EQ(imap(0x20, 0x00, 0xff, 0, 100), 12);
    ASSERT_EQ(imap(0x42, 0x00, 0xff, 0, 1000), 258);
    ASSERT_EQ(imap(0x69, 0x00, 0xff, 0, 1000), 411);
}

TEST(Math, IntPower)
{
    ASSERT_EQ(ipow(1, 0), 1);
    ASSERT_EQ(ipow(1, 1), 1);
    ASSERT_EQ(ipow(1, 99), 1);
    ASSERT_EQ(ipow(2, 0), 1);
    ASSERT_EQ(ipow(2, 1), 2);
    ASSERT_EQ(ipow(2, 2), 4);
    ASSERT_EQ(ipow(2, 10), 1024);
    ASSERT_EQ(ipow(3ull, 20), 3486784401);
    ASSERT_EQ(ipow(2ull, 63), 9223372036854775808ull);
}

TEST(Math, IntLength)
{
    ASSERT_EQ(ilen(0), 1);
    ASSERT_EQ(ilen(1), 1);
    ASSERT_EQ(ilen(9), 1);
    ASSERT_EQ(ilen(10), 2);
    ASSERT_EQ(ilen(99), 2);
    ASSERT_EQ(ilen(100), 3);
    ASSERT_EQ(ilen(999), 3);
    ASSERT_EQ(ilen(20220805), 8);
    ASSERT_EQ(ilen(-1), 2);
    ASSERT_EQ(ilen(-777), 4);
    ASSERT_EQ(ilen(-20220805), 9);
}

TEST(Math, Factorial)
{
    ASSERT_EQ(fact(0), 1);
    ASSERT_EQ(fact(1), 1);
    ASSERT_EQ(fact(2), 2);
    ASSERT_EQ(fact(3), 6);
    ASSERT_EQ(fact(4), 24);
    ASSERT_EQ(fact(13ul), 6227020800); 
}

TEST(Math, GaloisFieldMultiplication)
{
    // TODO
}

TEST(Math, Haversine)
{
    // TODO
    // assert_near(haversine(0, 0, 0, 0, 0), 0); 
}

TEST(Math, GcsDistance)
{
    assert_near(gcs_distance( 42.42,  13.37,  66.66,  77.77), 4645479,    1);
    assert_near(gcs_distance( 24.42,  99.99, -22.11, -57.93), 17747388,   1);
    assert_near(gcs_distance(-71.45, -69.69,  76.69,  69.03), 18664684,   1);
}

TEST(Math, Inclination)
{
    // TODO
}

TEST(Math, Roll)
{
    // TODO
}

TEST(Math, Pitch)
{
    // TODO
}
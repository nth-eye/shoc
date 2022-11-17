#include <gtest/gtest.h>
#include "utl/pool.h"
#include "_util.h"

constexpr size_t test_size = 5;

using pool_t = utl::pool<tester, test_size>;

static void verify_pool(const pool_t& obj, size_t copy, size_t move, size_t taken)
{
    ASSERT_EQ(obj.capacity(), test_size);
    ASSERT_EQ(obj.left(), test_size - taken);
    ASSERT_EQ(obj.full(), taken == 0);
    ASSERT_EQ(obj.empty(), taken == test_size);

    verify_count(copy, move);
}

struct Pool : ContainerTester {
    void TearDown() override 
    {
        ContainerTester::TearDown();
    }
    void verify(size_t copy, size_t move, size_t taken)
    {
        verify_pool(p, copy, move, taken);
    }
    pool_t p;
};

TEST_F(Pool, ConstructorDefault)
{
    pool_t x;
    verify_pool(x, 0, 0, 0);
}

TEST_F(Pool, Get)
{
    {
    pool_t::return_type arr[test_size];
    for (size_t i = 0; i < p.capacity(); ++i) {
        arr[i] = p.get(i);
        ASSERT_EQ(bool(arr[i]), true);
        ASSERT_EQ((*arr[i])(), i);
        verify(0, 0, i + 1);
    }
    ASSERT_EQ(bool(p.get()), false);
    verify(0, 0, test_size);
    }
    {
    auto val = p.get(42);
    ASSERT_EQ(bool(val), true);
    ASSERT_EQ((*val)(), 42);
    verify(0, 0, 1);
    }
    verify(0, 0, 0);
}
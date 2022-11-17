#include <gtest/gtest.h>
#include "utl/ring.h"
#include "_util.h"

constexpr size_t test_size = 4;

using ring_t = utl::ring<tester, test_size, true>;

static void print_ring(const auto& r) 
{
    const auto one = r.array_one();
    const auto two = r.array_two();

    printf("+-------------+\n");
    printf("linear:     %u \n", r.linear());
    printf("plain:      %u \n", r.plain());
    printf("full:       %u \n", r.full());
    printf("head:       %lu \n", r.head);
    printf("tail:       %lu \n", r.tail);
    printf("all:        %lu \n", r.size());
    printf("one:        %lu \n", one.size());
    printf("two:        %lu \n", two.size());

    for (const auto& it : r)
        printf("%d ", it());
    printf("\n");

    for (const auto& it : one)
        printf("%d ", it());
    printf("\n");

    for (const auto& it : two)
        printf("%d ", it());
    printf("\n");

    printf("+-------------+\n");
};

static void verify_part(std::span<const tester> obj, std::initializer_list<int> arr)
{
    ASSERT_EQ(obj.size(), arr.size());
    for (size_t i = 0; auto it : arr)
        ASSERT_EQ(obj[i++](), it) << "at index " << i;
}

static void verify_ring(const ring_t& obj, 
    size_t copy, 
    size_t move, 
    std::initializer_list<int> arr,
    std::initializer_list<int> one = {},
    std::initializer_list<int> two = {})
{
    size_t size = arr.size();

    ASSERT_LE(size, test_size);
    ASSERT_EQ(obj.capacity(), test_size);
    ASSERT_EQ(obj.max_size(), test_size);
    ASSERT_EQ(obj.size(), size);
    ASSERT_EQ(obj.full(), size == test_size);
    ASSERT_EQ(obj.empty(), size == 0);
    ASSERT_EQ(obj.begin() + size, obj.end());

    if (obj.empty() == false) {
        ASSERT_EQ(&obj.front(), &*obj.begin());
        ASSERT_EQ(&obj.back(), &*(obj.end() - 1));
    }
    verify_count(copy, move);

    for (size_t i = 0; auto it : arr)
        ASSERT_EQ(obj[i++](), it) << "at index " << i;

    // printf("lin %lu \n", obj.linear());
    // printf("one %lu \n", obj.array_one().size());
    // printf("two %lu \n", obj.array_two().size());

    if (obj.linear()) {
        verify_part(obj.array_one(), arr);
        verify_part(obj.array_two(), {});
    } else {
        verify_part(obj.array_one(), one);
        verify_part(obj.array_two(), two);
    }
}

struct Ring : ContainerTester {
    void TearDown() override 
    {
        r.clear();
        ContainerTester::TearDown();
    }
    void setup(std::initializer_list<int> arr)
    {
        ASSERT_LE(arr.size(), test_size);
        r.clear();
        for (auto it : arr)
            r.emplace_back(it);
    }
    void verify(
        size_t copy, 
        size_t move, 
        std::initializer_list<int> arr, 
        std::initializer_list<int> one = {},
        std::initializer_list<int> two = {})
    {
        verify_ring(r, copy, move, arr, one, two);
    }
    ring_t r;
};

TEST_F(Ring, ConstructorDefault)
{
    ring_t x;
    verify_ring(x, 0, 0, {});
}

TEST_F(Ring, ConstructorDefaultN)
{
    ring_t x(4);
    verify_ring(x, 0, 0, {0, 0, 0, 0});
}

TEST_F(Ring, ConstructorConstReferenceN)
{
    ring_t x(4, 77);
    verify_ring(x, 4, 0, {77, 77, 77, 77});
}

TEST_F(Ring, ConstructorRange)
{
    // setup({22, 33, 44});
    // ring_t x(r.begin(), r.begin() + 2);
    // verify_ring(x, 2, 0, {22, 33});
    // verify_ring(r, 2, 0, {22, 33, 44}); // verify source didn't change
}

TEST_F(Ring, CopyConstructor)
{
    setup({66, 66, 66});
    ring_t x = r;
    verify_ring(x, 3, 0, {66, 66, 66});
    verify_ring(r, 3, 0, {66, 66, 66});
}

TEST_F(Ring, MoveConstructor)
{
    ring_t y(2, 55);
    ring_t x = std::move(y);
    verify_ring(x, 2, 2, {55, 55});
}

TEST_F(Ring, ConstructorSpan)
{
    const std::array<tester, 0> arr_0 = {};
    ring_t x = {arr_0};
    verify_ring(x, 0, 0, {}, {}, {});

    std::array<tester, 2> arr_1 = {100, 200};
    ring_t y = {arr_1};
    verify_ring(y, 2, 0, {100, 200});

    tester arr_2[4] = {999};
    ring_t z = {arr_2};
    verify_ring(z, 6, 0, {999, 0, 0, 0});
}

TEST_F(Ring, ConstructorArray)
{
    ring_t x = {{1, 1}};
    verify_ring(x, 0, 2, {1, 1});

    ring_t y = {{2, 1, 0, -1}};
    verify_ring(y, 0, 6, {2, 1, 0, -1});
}

TEST_F(Ring, AssignmentOperatorCopy)
{
    const ring_t src(4, 4);
    r = src;
    verify(8, 0, {4, 4, 4, 4});
}

TEST_F(Ring, AssignmentOperatorMove)
{
    r = ring_t(3, 3); 
    verify(3, 3, {3, 3, 3});
}

TEST_F(Ring, AssignmentOperatorSpan)
{
    tester arr_0[2] = {1, 2};
    r = arr_0;
    verify(2, 0, {1, 2});

    std::array<tester, 4> arr_1 = {4, 3, 2, 1};
    r = arr_1;
    verify(6, 0, {4, 3, 2, 1});

    const std::array<tester, 0> arr_2 = {};
    r = arr_2;
    verify(6, 0, {});
}

TEST_F(Ring, AssignmentOperatorArray)
{
    r = {666, 1, 2, 3};
    verify(0, 4, {666, 1, 2, 3});

    r = {tester(1), 2};
    verify(0, 6, {1, 2});

    r = {tester(42)};
    verify(0, 7, {42});
}

TEST_F(Ring, AssignN)
{
    r.assign(2, 2);
    verify(2, 0, {2, 2});

    r.assign(4, 4);
    verify(6, 0, {4, 4, 4, 4});

    r.assign(0, 0);
    verify(6, 0, {});
}

TEST_F(Ring, AssignRange)
{
    std::array<tester, test_size> arr = {1, 2, 3, 4};

    r.assign(arr.begin(), arr.begin() + 2);
    verify(2, 0, {1, 2});

    r.assign(arr.begin(), arr.end());
    verify(6, 0, {1, 2, 3, 4});

    r.assign(arr.begin(), arr.begin());
    verify(6, 0, {});
}

TEST_F(Ring, AssignSpan)
{
    tester arr_0[2] = {1, 2};
    r.assign(arr_0);
    verify(2, 0, {1, 2});

    std::array<tester, 4> arr_1 = {4, 3, 2, 1};
    r.assign(arr_1);
    verify(6, 0, {4, 3, 2, 1});

    const std::array<tester, 0> arr_2 = {};
    r.assign(arr_2);
    verify(6, 0, {});
}

TEST_F(Ring, AssignArray)
{
    tester arr[1] = {666};

    r.assign(std::move(arr));
    verify(0, 1, {666});

    r.assign({1, 2});
    verify(0, 3, {1, 2});

    r.assign({4, 3, 2, 1});
    verify(0, 7, {4, 3, 2, 1});
}

TEST_F(Ring, ResizeDefault)
{
    // // TODO

    // r.resize(2);
    // verify(0, 0, {0, 0});

    // r.resize(4);
    // verify(0, 0, {0, 0, 0, 0});

    // r.resize(0);
    // verify(0, 0, {});

    // for (int i = 1; i <= 7; ++i)
    //     r.put(i);

    // verify(0, 0, {4, 5, 6, 7}, {4}, {5, 6, 7});

    // r.resize(2);
    // verify(0, 0, {4, 5}, {4}, {5});
}

TEST_F(Ring, ResizeReference)
{
    // TODO

    // r.resize(2, 0);
    // verify(2, 0, {0, 0});

    // r.resize(3, 3);
    // verify(3, 0, {0, 0, 3});

    // r.resize(4, 4);
    // verify(4, 0, {0, 0, 3, 4});

    // r.resize(0, {});
    // verify(4, 0, {});
}



// TEST(ring_t, End)
// {
    // ring_t ring;

    // ASSERT_EQ(ring.pzend(), ring.pbegin());

    // ring.put(1);

    // ASSERT_EQ(ring.pzend(), ring.pbegin() + 1);

    // ring.put(2);
    // ring.put(3);
    // ring.put(4);

    // ASSERT_EQ(ring.pzend(), ring.pbegin() + 4);

    // ring.put(5);

    // ASSERT_EQ(ring.pzend(), ring.pbegin() + 1);
// }

// TEST(ring_t, EraseBegin)
// {
//     ring_t ring;

//     for (int i = 1; i <= 4; ++i)
//         ring.put(i);

//     ring.erase_begin(1);

//     verify_part(ring.array_one(), {2, 3, 4});
//     verify_part(ring.array_two(), { });
// }

// TEST(ring_t, EraseEnd)
// {

// }

TEST(Ring2, Linearize)
{
    ring_t ring;

    for (int i = 1; i <= 2; ++i)
        ring.put(i);
        
    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {1, 2});
    verify_part(ring.array_two(), { });
    ring.clear();

    for (int i = 1; i <= 4; ++i)
        ring.put(i);

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {1, 2, 3, 4});
    verify_part(ring.array_two(), { });
    ring.clear();

    for (int i = 1; i <= 4; ++i)
        ring.put(i);
    for (int i = 1; i <= 2; ++i)
        ring.pop_front();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {3, 4});
    verify_part(ring.array_two(), { });
    ring.clear();



    for (int i = 1; i <= 7; ++i)
        ring.put(i);

    ASSERT_EQ(ring.linear(), false);
    verify_part(ring.array_one(), {4});
    verify_part(ring.array_two(), {5, 6, 7});

    ring.linearize();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {4, 5, 6, 7});
    verify_part(ring.array_two(), { });
    ring.clear();

    for (int i = 1; i <= 6; ++i)
        ring.put(i);

    ASSERT_EQ(ring.linear(), false);
    verify_part(ring.array_one(), {3, 4});
    verify_part(ring.array_two(), {5, 6});

    ring.linearize();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {3, 4, 5, 6});
    verify_part(ring.array_two(), { });
    ring.clear();

    for (int i = 1; i <= 5; ++i)
        ring.put(i);
    ASSERT_EQ(ring.linear(), false);
    verify_part(ring.array_one(), {2, 3, 4});
    verify_part(ring.array_two(), {5});

    ring.linearize();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {2, 3, 4, 5});
    verify_part(ring.array_two(), { });
    ring.clear();



    for (int i = 1; i <= 5; ++i)
        ring.put(i);
    for (int i = 1; i <= 1; ++i)
        ring.pop_front();

    ASSERT_EQ(ring.linear(), false);
    verify_part(ring.array_one(), {3, 4});
    verify_part(ring.array_two(), {5});

    ring.linearize();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {3, 4, 5});
    verify_part(ring.array_two(), { });
    ring.clear();

    for (int i = 1; i <= 5; ++i)
        ring.put(i);
    for (int i = 1; i <= 2; ++i)
        ring.pop_front();

    ASSERT_EQ(ring.linear(), false);
    verify_part(ring.array_one(), {4});
    verify_part(ring.array_two(), {5});

    ring.linearize();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {4, 5});
    verify_part(ring.array_two(), { });
    ring.clear();

    for (int i = 1; i <= 6; ++i)
        ring.put(i);
    for (int i = 1; i <= 1; ++i)
        ring.pop_front();

    ASSERT_EQ(ring.linear(), false);
    verify_part(ring.array_one(), {4});
    verify_part(ring.array_two(), {5, 6});

    ring.linearize();

    ASSERT_EQ(ring.linear(), true);
    verify_part(ring.array_one(), {4, 5, 6});
    verify_part(ring.array_two(), { });
    ring.clear();


    ring.clear();
    for (int i = 1; i <= 2; ++i)
        ring.put(i);
    for (int i = 1; i <= 2; ++i)
        ring.pop_front();
    ASSERT_EQ(ring.linear(), true);
}
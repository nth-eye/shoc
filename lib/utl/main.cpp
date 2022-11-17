#include <iostream>
#include "utl/time.h"
#include "utl/vector.h"
#include "utl/ring.h"
#include "utl/pool.h"
#include "utl/log.h"
#include "utl/makima.h"

struct X {
    X() { 
        puts("default constructor"); 
    }
    X(int y) {
        puts("argument constructor");
        x = y;
    }
    ~X() {
        puts("destructor");
        x = -99;
    }
    X(const X& rhs) noexcept { 
        puts("copy constructor");
        x = rhs.x;
    }
    X(X&& rhs) noexcept { 
        puts("move constructor");
        x = rhs.x;
    }
    X& operator=(const X& rhs) noexcept {
        puts("copy assignment");
        x = rhs.x;
        return *this;
    }
    X& operator=(X&& rhs) noexcept {
        puts("move assignment");
        x = rhs.x;
        return *this;
    }
public:
    int x = 0;
};

int main()
{
    std::cout << "+-------------------+" << std::endl;

    // printf(": %3ld clock_t\n", utl::exec_time<count>(test_1));
    // printf(": %3ld clock_t\n", utl::exec_time<count>(test_1));
    // printf(": %3ld clock_t\n", utl::exec_time<count>(test_1));
    // printf(": %3ld clock_t\n", utl::exec_time<count>(test_1));

    // utl::makima<double, 16> fsr_map = {
    //     {   90,     112,    155,    240,    340,    550,    800,    1000,
    //         1300,   2500,   5000,   55000,  100000, 115000, 120000, 1000000 },
    //     {   45.0,   25.0,   20.0,   17.5,   15.0,   12.5,   10.0,   8.75,
    //         7.5,    6.25,   5.0,    3.75,   2.5,    1.25,   0.03,   0.02    },
    // };
    // auto val = double(299);
    // auto ret = fsr_map(val);
    // if (!ret) {
    //     printf("fsr_map(%.2f):    invalid \n", val);
    // } else {
    //     printf("fsr_map(%.2f):    %.2f \n", val, *ret);
    // }

    utl::ring<X, 8, true> ring; // (7, 42);
    // ring.push_back(1);
    utl::ring<X, 8, true> ring2 = ring; // (ring.cbegin(), ring.cend());

    auto print_ring = [](const auto& r) 
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
            printf("%d ", it.x);
        printf("\n");

        for (const auto& it : one)
            printf("%d ", it.x);
        printf("\n");

        for (const auto& it : two)
            printf("%d ", it.x);
        printf("\n");

        printf("+-------------+\n");
    };

    for (int i = 1; i <= 1; ++i)
        ring.put(i);
    for (int i = 1; i <= 1; ++i)
        ring.pop_front();

    // auto s = ring.begin();
    // s += 2;
    // // // --s;

    // print_ring();
    // ring.rotate(s);
    
    print_ring(ring);
    // ring.erase(ring.cbegin() + 5);
    // print_ring(ring);

    // static_assert(std::input_or_output_iterator<decltype(ring)::iterator>);
    // static_assert(std::indirectly_readable<decltype(ring)::iterator>);
    // static_assert(std::input_iterator<decltype(ring)::iterator>);

    // print_ring(ring2);

    // using namespace std::literals;

    static constexpr auto joined = utl::join_v<"hello ", "world: 0x", utl::to_string_v<0xb16b00b5, 16, true>>;

    auto integral = utl::to_string<-666>::cstr();
    auto floating = utl::to_string<-42.08>::cstr();

    printf("%s \n", joined.data());
    printf("%s \n", integral);
    printf("%s \n", floating);
}

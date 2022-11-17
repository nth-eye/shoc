#ifndef UTL_POOL_H
#define UTL_POOL_H

#include "utl/bit_vector.h"
#include "utl/storage.h"

namespace utl {

/**
 * @brief Static object pool with pre-allocated memory. Before C++23 
 * cannot be used in constexpr context because of std::unique_ptr, 
 * even though deleter doesn't deallocate any actual memory. Uses 
 * utl::bit_vector to store positions of elements.
 * 
 * @tparam T Type of elements
 * @tparam N Number of elements in pool
 * @tparam W Word type of utl::bit_vector, look its description
 * @tparam G Grow point of utl::bit_vector, look its description
 */
template<class T, size_t N, class W = uint32_t, size_t G = 4>
class pool {
    struct releaser {
        void operator()(T* obj) {
            if (obj) {
                dtor(obj);
                ptr->bits.clr(obj - ptr->buf);
                ptr->available++;
            }
        }
        pool* ptr;
    };
public:
    using size_type = size_t;
    using value_type = T;
    using return_type = std::unique_ptr<value_type, releaser>;

    // ANCHOR Constructors

    constexpr pool() = default;
    constexpr pool(const pool&) = delete;
    constexpr pool(pool&&) = delete; 
    constexpr pool& operator=(const pool&) = delete; 
    constexpr pool& operator=(pool&&) = delete; 

    // ANCHOR Capacity

    constexpr static size_type capacity()       { return N; }
    constexpr size_type left() const noexcept   { return available; }
    constexpr bool empty() const noexcept       { return available == 0; }
    constexpr bool full() const noexcept        { return available == N; }

    // ANCHOR Modifiers

    template<typename... Args>
    constexpr auto get(Args&&... args)
    {
        auto acquire = [this] (value_type* ptr) {
            return return_type(ptr, releaser{this}); 
        };
        if (empty()) return acquire(nullptr);
        --available; 
        return acquire(ctor(&buf[bits.acquire_any()], std::forward<Args>(args)...));
    }
private:
    storage<T, N> buf;
    size_type available = N;
    bit_vector<W, N, G> bits;
};

}

#endif
#ifndef UTL_BIT_VECTOR_H
#define UTL_BIT_VECTOR_H

#define UTL_BIT_VECTOR_REMAINDER_STORED false

#include "utl/bit.h"
#include <array>

namespace utl {
namespace imp {

struct bit_tree_level {
    size_t head;
    size_t size;
#if (UTL_BIT_VECTOR_REMAINDER_STORED)
    size_t remainder;
#endif
};

template<class T>
constexpr auto bit_tree_depth(size_t bits, size_t grow_point)
{
    size_t depth = 0;
    while (1) {
        ++depth;
        bits = bit_ceil<T>(bits);
        if (bits < bit_size<T>()) {
            if (bits > grow_point)
                ++depth;
            break;
        }
    }
    return depth;
}

template<class T, size_t N, size_t G>
constexpr auto bit_tree_struct()
{
    static_assert(N, "bit_vector size must be > 0");
    static_assert(G && G < bit_size<T>(), "grow point G must be [0 < G < sizeof(word) * 8]");
    std::array<bit_tree_level, bit_tree_depth<T>(N, G)> arr = {};
    size_t bits = N;
    size_t head = 0;
    for (auto& it : arr) {
#if (UTL_BIT_VECTOR_REMAINDER_STORED)
        it.remainder = bits & bit_wrap<T>();
#endif
        it.head = head;
        it.size = bits = bit_ceil<T>(bits);
        head += bits;
    }
    return arr;
}

}

/**
 * @brief Structure analogous to std::vector<bool> or std::bitset,
 * but with bits stored as tree divided by levels. Every bit on a 
 * level indicates if word with this index on underlying level is 
 * fully set. User can choose word type of bit array, number of 
 * used bits and grow point. Grow point means number of words on 
 * a top level, so it always contains <= G words. This granularity 
 * (and the fact that tree structure is static and calculated at 
 * compile time) gives perfect access time for both small and huge 
 * vectors. If tree will only contain 1 level, it will be plain and 
 * no additional calculations will be performed, as with a regular 
 * integer-based bit flags. For example, with T = uint32_t, N = 128 
 * and G = 4 tree needs 128/32 = 4 words. Since this is <= G the 
 * tree will be totally flat. Often linear access with such a small 
 * number of words is faster than to iterate through 2 tree levels.
 * 
 * @tparam T Word type for storage array
 * @tparam N Number of used bits from array
 * @tparam G Grow point, maximum number of words on the top level (after this tree adds level at compile-time)
 */
template<class T, size_t N, size_t G>
struct bit_vector {
    constexpr bit_vector()              { init_mask();}
    constexpr static size_t capacity()  { return N; }
    constexpr static size_t depth()     { return levels.size(); }
    constexpr static size_t words()     { return levels.back().head + levels.back().size; }
    constexpr void reset()
    {
        std::fill(buf, buf + words(), 0);
        init_mask();
    }
    constexpr bool operator[](size_t pos) const
    { 
        assert(pos < N);
        return get_arr_bit(buf, pos); 
    }
    constexpr void set(size_t pos)
    {
        assert(pos < N);
        if constexpr (depth() == 1) {
            set_arr_bit(buf, pos); 
            return;
        }
        size_t bit;
        for (const auto &it : levels) {
            bit = pos & bit_wrap<T>();
            pos = pos >> bit_shft<T>();
            set_bit(buf[it.head + pos], bit);
            if (buf[it.head + pos] != bit_full<T>())
                break;
        }
    }
    constexpr void clr(size_t pos)
    { 
        assert(pos < N);
        if constexpr (depth() == 1) {
            clr_arr_bit(buf, pos); 
            return;
        }
        size_t bit;
        for (const auto &it : levels) {
            bit = pos & bit_wrap<T>();
            pos = pos >> bit_shft<T>();
            if (buf[it.head + pos] == bit_full<T>()) {
                clr_bit(buf[it.head + pos], bit);
                continue;
            } else {
                clr_bit(buf[it.head + pos], bit);
                break;
            }
        }
    }
    constexpr auto acquire_any()
    {
        const auto &end = levels.back();
        for (size_t i = 0; i < end.size; ++i) {
            if (buf[end.head + i] != bit_full<T>()) {
                if constexpr (depth() == 1) {
                    size_t pos = cnttz(~buf[i]);
                    set_bit(buf[i], pos);
                    return (i << bit_shft<T>()) + pos;
                } else {
                    size_t pos = (i << bit_shft<T>()) + cnttz(~buf[end.head + i]);
                    for (auto it = levels.rbegin() + 1; it != levels.rend(); ++it) {
                        pos = (pos << bit_shft<T>()) + cnttz(~buf[it->head + pos]);
                    }
                    set(pos);
                    return pos;
                }
            }
        }
        return N;
    }
private:
    constexpr void init_mask()
    {
#if (UTL_BIT_VECTOR_REMAINDER_STORED)
        for (const auto& [head, size, remainder] : levels) {
            if (remainder) {
                buf[head + size - 1] = ~(bit(remainder) - 1);
            }
        }
#else
        size_t bits = N;
        for (const auto& [head, size] : levels) {
            auto remainder = bits & bit_wrap<T>();
            if (remainder) {
                buf[head + size - 1] = ~(bit(remainder) - 1);
            }
            bits = bit_ceil<T>(bits);
        }
#endif
    }
    static constexpr auto levels = imp::bit_tree_struct<T, N, G>();
private:
    T buf[words()] = {};
};

}

#endif
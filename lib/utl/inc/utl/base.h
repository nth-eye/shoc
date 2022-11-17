#ifndef UTL_BASE_H
#define UTL_BASE_H

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <type_traits>

namespace utl {

/**
 * @brief Helper to get number of elements in array. 
 * 
 * @tparam T Auto-deduced element type
 * @tparam N Auto-deduced number of elements
 * @return Array size
 */
template<class T, size_t N>
constexpr size_t countof(T(&)[N]) 
{ 
    return N; 
}

template<class T> 
concept arithmetic = std::is_arithmetic_v<T>;

}

#endif
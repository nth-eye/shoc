#ifndef SHOC_BIGINT_H
#define SHOC_BIGINT_H

#include "shoc/util.h"

namespace shoc {

template<std::unsigned_integral T, size_t N> 
struct bigint {
    constexpr bigint() = default;
    constexpr bigint()
    {
        
    }
private:
    static constexpr auto words = utl::words_in_bits(N);
private:
    T buf[words]{};
};

}

#endif
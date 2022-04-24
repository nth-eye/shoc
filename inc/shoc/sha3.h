#ifndef SHOC_SHA3_H
#define SHOC_SHA3_H

#include "shoc/util.h"

namespace shoc {

template <size_t L>
struct Keccak {

    static constexpr size_t L_val = L;
    static constexpr size_t B = 25 * (1 << L);
    static constexpr size_t R = 12 + 2 * L;

private:

};

}

#endif
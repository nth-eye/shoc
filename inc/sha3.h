#ifndef SHA3_H
#define SHA3_H

#include <cstddef>

namespace creep {

template <size_t L>
struct Keccak {

    static constexpr size_t L_val = L;
    static constexpr size_t B = 25 * (1 << L);
    static constexpr size_t R = 12 + 2 * L;

private:

};

}

#endif
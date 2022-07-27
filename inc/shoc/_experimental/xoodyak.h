#ifndef SHOC_XOODYAK_H
#define SHOC_XOODYAK_H

#include "shoc/util.h"

namespace shoc {

struct Xoodyak {

    using word = uint32_t;

    static constexpr size_t SIZE = 32;

    void init();
    void feed(const void *in, size_t len);
    void stop(byte *out, size_t len = SIZE);
    void operator()(const void *in, size_t len, byte *out, size_t out_len = SIZE);
private:

};

void Xoodyak::init()
{
    
}

}

#endif
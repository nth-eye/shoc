#ifndef SHOC_TEST_H
#define SHOC_TEST_H

#include <gtest/gtest.h>
#include "shoc/util.h"

namespace shoc {

inline void compare(span_i<> out, span_i<> exp)
{
    ASSERT_TRUE(out.data());
    ASSERT_TRUE(exp.data());
    ASSERT_TRUE(out.size() == exp.size());
    for (size_t i = 0; i < exp.size(); ++i)
        ASSERT_EQ(out[i], exp[i]) << "at index " << i;
}

}

#endif
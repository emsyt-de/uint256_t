#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(Arithmetic, increment){
    uint256_t value(0);
    EXPECT_EQ(++value, 1);
	EXPECT_EQ(value++, 1);
	EXPECT_EQ(++value, 3);
}

TEST(Arithmetic, decrement){
    uint256_t value(0);
    EXPECT_EQ(--value, uint256_t(0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL));
    EXPECT_EQ(value--, uint256_t(0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL));
    EXPECT_EQ(--value, uint256_t(0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xfffffffffffffffdULL));
}

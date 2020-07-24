#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(Arithmetic, equation){
	const uint256_t big1    (0xfedbca9876543210ULL,0xfedbca9876543210ULL,0xfedbca9876543210ULL,0xfedbca9876543210ULL);
	const uint256_t small1  (0xffffULL,0xffffULL,0xffffULL,0xffffULL);
	const uint256_t big2    (0x8000000000000000LL,0x0ULL,0x0ULL,0x0ULL);
	const uint256_t small2  (0x7ULL);

	auto div1 = big1/small1;
	auto mod1 = big1%small1;
	auto div2 = big2/small2;
	auto mod2 = big2%small2;

	EXPECT_EQ(div1 * small1 + mod1 - big1, 0);
	EXPECT_EQ(div2 * small2 + mod2 - big2, 0);
}

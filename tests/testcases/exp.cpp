#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(Arithmetic, exponential)
{
	uint256_t val(0xfedbca9876543210ULL);
	uint256_t val2(0x4);

	EXPECT_EQ(uint256_t::exp(val, 0), 1);
	EXPECT_EQ(uint256_t::exp(val, 1), val);
	EXPECT_EQ(uint256_t::exp(val, val2), uint256_t(0xfb76f5abec88993e, 0x64992828e4c5380c, 0x338379e572c292e5, 0x210066d1a4810000));
	EXPECT_EQ(uint256_t::exp(0x6543210, 0x5), uint256_t(0x0, 0x27, 0xaa012fe5e9d32976, 0x107e9f8f7a100000));
}

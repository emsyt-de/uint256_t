#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(Logical, and){
	const uint256_t A1(0xffffffff,0x00000000,0x00000000,0x00000000);
	const uint256_t A2(0xffffffff,0xffffffff,0x00000000,0x00000000);
	const uint256_t A3(0x00000000,0x00000000,0xffffffff,0x00000000);
	const uint256_t A4(0x00000000,0x00000000,0x00000000,0xffffffff);
	const uint256_t B(0x00000000,0x00000000,0x00000000,0x00000000);

	EXPECT_EQ(A1 && A1, true);
	EXPECT_EQ(A1 && A2, true);
	EXPECT_EQ(A1 && A3, true);
	EXPECT_EQ(A1 && A4, true);
	EXPECT_EQ(A1 && B, false);
	EXPECT_EQ(A2 && B, false);
	EXPECT_EQ(A3 && B, false);
	EXPECT_EQ(A4 && B, false);
}

TEST(Logical, or){
	const uint256_t A1(0xffffffff,0x00000000,0x00000000,0x00000000);
	const uint256_t A2(0xffffffff,0xffffffff,0x00000000,0x00000000);
	const uint256_t A3(0x00000000,0x00000000,0xffffffff,0x00000000);
	const uint256_t A4(0x00000000,0x00000000,0x00000000,0xffffffff);
	const uint256_t B(0x00000000,0x00000000,0x00000000,0x00000000);

	EXPECT_EQ(A1 || A1, true);
	EXPECT_EQ(A1 || A2, true);
	EXPECT_EQ(A1 || A3, true);
	EXPECT_EQ(A1 || A4, true);
	EXPECT_EQ(A1 || B,  true);
	EXPECT_EQ(A2 || B,  true);
	EXPECT_EQ(A3 || B,  true);
	EXPECT_EQ(A4 || B,  true);
}

TEST(Logical, not){
	const uint256_t A1(0xffffffff,0x00000000,0x00000000,0x00000000);
	const uint256_t A2(0xffffffff,0xffffffff,0x00000000,0x00000000);
	const uint256_t A3(0x00000000,0x00000000,0xffffffff,0x00000000);
	const uint256_t A4(0x00000000,0x00000000,0x00000000,0xffffffff);
	const uint256_t B(0x00000000,0x00000000,0x00000000,0x00000000);
	EXPECT_EQ(!A1, false);
	EXPECT_EQ(!A1, false);
	EXPECT_EQ(!A1, false);
	EXPECT_EQ(!A1, false);
	EXPECT_EQ(!B,  true);
}

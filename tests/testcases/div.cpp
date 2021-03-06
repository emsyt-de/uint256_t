#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(Arithmetic, divide){
	const uint256_t big    (0xfedbca9876543210ULL,0xfedbca9876543210ULL,0xfedbca9876543210ULL,0xfedbca9876543210ULL);
	const uint256_t small  (0xffffULL,0xffffULL,0xffffULL,0xffffULL);
	const uint256_t small2  (0x800ULL,0x0ULL,0x0ULL,0x0ULL);

    EXPECT_EQ(small / small, 1);
	EXPECT_EQ(small2 / small2, 1);
	EXPECT_EQ(small / big, 0);
	EXPECT_EQ(big / small, 280224226164681);
	EXPECT_EQ(big / small2, 8967038409034374);
	EXPECT_EQ(uint256_t(23)/8,2);

    EXPECT_EQ(big   / big,   1);

    EXPECT_THROW(uint256_t(1) / uint256_t(0), std::domain_error);
}

TEST(External, divide){
    bool      t    = true;
    bool      f    = false;
    uint8_t   u8   = 0xaaULL;
    uint16_t  u16  = 0xaaaaULL;
    uint32_t  u32  = 0xaaaaaaaaULL;
    uint64_t  u64  = 0xaaaaaaaaaaaaaaaaULL;
	uint128_t u128 = 0xaaaaaaaaaaaaaaaaULL;
	u128 = u128 << 64;
	u128 |= 0xaaaaaaaaaaaaaaaaULL;

	uint128_t u128_1 = 0x163356b88ac0de0ULL;
	u128_1 = u128_1 << 64;
	u128_1 |= 0x163356b88ac0de01ULL;

    const uint256_t val(0x7bULL);

    EXPECT_EQ(t    /  val, false);
    EXPECT_EQ(f    /  val, false);
    EXPECT_EQ(u8   /  val, uint256_t(0x1ULL));
    EXPECT_EQ(u16  /  val, uint256_t(0x163ULL));
    EXPECT_EQ(u32  /  val, uint256_t(0x163356bULL));
    EXPECT_EQ(u64  /  val, uint256_t(0x163356b88ac0de0ULL));
    EXPECT_EQ(u128 /  val, uint256_t(0x0000000000000000ULL, 0x0000000000000000ULL, 0x163356b88ac0de0ULL, 0x163356b88ac0de01ULL));

    EXPECT_EQ(t    /= val, false);
    EXPECT_EQ(f    /= val, false);
    EXPECT_EQ(u8   /= val, (uint8_t)  0x1ULL);
    EXPECT_EQ(u16  /= val, (uint16_t) 0x163ULL);
    EXPECT_EQ(u32  /= val, (uint32_t) 0x163356bULL);
    EXPECT_EQ(u64  /= val, (uint64_t) 0x163356b88ac0de0ULL);
	EXPECT_EQ(u128 /= val,  u128_1);
}

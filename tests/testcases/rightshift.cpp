#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(BitShift, right){
    // operator>>
	uint256_t val128(0x0,0x0,0xffffffffffffffffULL,0xffffffffffffffffULL);
	uint128_t exp128 = 0xffffffffffffffffULL;
	exp128 <<= 64;
	exp128 |= 0xffffffffffffffffULL;
	for(uint8_t i = 0; i < 128; i++){
		EXPECT_EQ(val128 >> i, exp128 >> i);
	}

    uint256_t val(0xffffffffffffffffULL);
    uint64_t exp = 0xffffffffffffffffULL;
    for(uint8_t i = 0; i < 64; i++){
        EXPECT_EQ(val >> i, exp >> i);
    }

    uint256_t zero(0);
    for(uint8_t i = 0; i < 64; i++){
        EXPECT_EQ(zero >> i, 0);
    }

    // operator>>=
    for(uint8_t i = 0; i < 64; i++){
        EXPECT_EQ(val >>= 1, exp >>= 1);
    }

    for(uint8_t i = 0; i < 64; i++){
        EXPECT_EQ(zero >>= 1, 0);
    }
}

TEST(External, shift_right){
    bool     t   = true;
    bool     f   = false;
    uint8_t  u8  = 0xffULL;
    uint16_t u16 = 0xffffULL;
    uint32_t u32 = 0xffffffffULL;
    uint64_t u64 = 0xffffffffffffffffULL;

    const uint256_t zero(0);
    const uint256_t one(1);

    EXPECT_EQ(t   >> zero, one);
    EXPECT_EQ(f   >> zero, zero);
    EXPECT_EQ(u8  >> zero, u8);
    EXPECT_EQ(u16 >> zero, u16);
    EXPECT_EQ(u32 >> zero, u32);
    EXPECT_EQ(u64 >> zero, u64);

    EXPECT_EQ(t   >>= zero, t);
    EXPECT_EQ(f   >>= zero, f);
    EXPECT_EQ(u8  >>= zero, u8);
    EXPECT_EQ(u16 >>= zero, u16);
    EXPECT_EQ(u32 >>= zero, u32);
    EXPECT_EQ(u64 >>= zero, u64);

    EXPECT_EQ(t   >> one, uint256_t(t)   >> 1);
    EXPECT_EQ(f   >> one, uint256_t(f)   >> 1);
    EXPECT_EQ(u8  >> one, uint256_t(u8)  >> 1);
    EXPECT_EQ(u16 >> one, uint256_t(u16) >> 1);
    EXPECT_EQ(u32 >> one, uint256_t(u32) >> 1);
    EXPECT_EQ(u64 >> one, uint256_t(u64) >> 1);

    EXPECT_EQ(t   >>= one, false);
    EXPECT_EQ(f   >>= one, false);
    EXPECT_EQ(u8  >>= one, (uint8_t)  0x7fULL);
    EXPECT_EQ(u16 >>= one, (uint16_t) 0x7fffULL);
    EXPECT_EQ(u32 >>= one, (uint32_t) 0x7fffffffULL);
    EXPECT_EQ(u64 >>= one, (uint64_t) 0x7fffffffffffffffULL);

    EXPECT_EQ(u8  >> uint256_t(7),  zero);
    EXPECT_EQ(u16 >> uint256_t(15), zero);
    EXPECT_EQ(u32 >> uint256_t(31), zero);
    EXPECT_EQ(u64 >> uint256_t(63), zero);

    EXPECT_EQ(u8  >>= uint256_t(7),  (uint8_t)  0);
    EXPECT_EQ(u16 >>= uint256_t(15), (uint16_t) 0);
    EXPECT_EQ(u32 >>= uint256_t(31), (uint32_t) 0);
    EXPECT_EQ(u64 >>= uint256_t(63), (uint64_t) 0);
}

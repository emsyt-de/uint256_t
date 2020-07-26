#include <map>

#include <gtest/gtest.h>

#include "uint256_t.h"

TEST(External, ostream){
	const uint256_t value1(1000000);
	const uint256_t value2(0x0000000000000002ULL,0x00dcba9876543210ULL,0x0000000000000000ULL,0xfedcba9876543210ULL);

    // write out octal uint256_t
	std::stringstream oct1; oct1 << std::oct << value1;
	std::stringstream oct2; oct2 << std::oct << value2;
	EXPECT_EQ(oct1.str(), "3641100");
	EXPECT_EQ(oct2.str(), "20015627246073124144100000000000000000000001773345651416625031020");

	// write out hexadecimal uint256_t
	std::stringstream hex1; hex1 << std::hex << value1;
	std::stringstream hex2; hex2 << std::hex << value2;
	EXPECT_EQ(hex1.str(), "f4240");
	EXPECT_EQ(hex2.str(), "200dcba98765432100000000000000000fedcba9876543210");


    // write out decimal uint256_t
	std::stringstream dec1; dec1 << std::dec << value1;
	std::stringstream dec2; dec2 << std::dec << value2;
	EXPECT_EQ(dec1.str(), "1000000");
	EXPECT_EQ(dec2.str(), "12575345098146018056657306966606486499429149247268856279568");

    // zero
    std::stringstream zero; zero << uint256_t();
    EXPECT_EQ(zero.str(), "0");
}

TEST(External, istream){
	uint256_t value1,value2,value3, value4, value5;
	const std::string dec_str("10000001000");
	const std::string hex_str("0x00000000000000200dcba98765432100000000000000000fedcba9876543210");
	const std::string oct_str("070000000000002006453437654321000000000000000002346276543210");
	const std::string empty("");
	const std::string zero("0");

	// write out octal uint256_t
	std::stringstream(dec_str) >> value1;
	std::stringstream(hex_str) >> value2;
	std::stringstream(oct_str) >> value3;
	std::stringstream(zero)    >> value4;
	EXPECT_EQ(value1, 10000001000);
	EXPECT_EQ(value2, uint256_t(0x0000000000000002,0x00dcba9876543210,0x0000000000000000,0xfedcba9876543210));
	EXPECT_EQ(value3, uint256_t(0x0000000000000000,0x0001C00000000101,0xa571fd6344000000,0x0000002732faC688));
	// zero
	EXPECT_EQ(value4, uint256_t());
	// empty
	std::stringstream(empty)    >> value1;
	EXPECT_EQ(value1, 10000001000);
	std::stringstream ss(empty);
	ss.exceptions(std::ios::failbit);
	EXPECT_THROW(ss >> value1, std::ios::failure);
}

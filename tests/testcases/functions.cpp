#include <map>

#include <gtest/gtest.h>

#include "uint256_t.h"

static const std::map <uint32_t, std::string> tests = {
    std::make_pair(2,  "10000100000101011000010101101100"),
    std::make_pair(3,  "12201102210121112101"),
    std::make_pair(4,  "2010011120111230"),
    std::make_pair(5,  "14014244043144"),
    std::make_pair(6,  "1003520344444"),
    std::make_pair(7,  "105625466632"),
    std::make_pair(8,  "20405302554"),
    std::make_pair(9,  "5642717471"),
    std::make_pair(10, "2216002924"),
    std::make_pair(11, "a3796a883"),
    std::make_pair(12, "51a175124"),
    std::make_pair(13, "294145645"),
    std::make_pair(14, "170445352"),
    std::make_pair(15, "ce82d6d4"),
    std::make_pair(16, "8415856c"),
    std::make_pair(17, "56dc4e33"),
    std::make_pair(18, "3b2db13a"),
    std::make_pair(19, "291i3b4g"),
    std::make_pair(20, "1eca0764"),
    std::make_pair(21, "14hc96jg"),
    std::make_pair(22, "jblga9e"),
    std::make_pair(23, "em6i5a5"),
    std::make_pair(24, "be75374"),
    std::make_pair(25, "91mo4go"),
    std::make_pair(26, "74d74li"),
    std::make_pair(27, "5jblgea"),
    std::make_pair(28, "4gl7i9g"),
    std::make_pair(29, "3l13lor"),
    std::make_pair(30, "315o5e4"),
    std::make_pair(31, "2fcfub9"),
    std::make_pair(32, "221b1bc"),
    std::make_pair(33, "1nkji2p"),
    std::make_pair(34, "1eq93ik"),
    std::make_pair(35, "176p6y9"),
    std::make_pair(36, "10ncmss")
    // std::make_pair(256, "uint256_t"),
};

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

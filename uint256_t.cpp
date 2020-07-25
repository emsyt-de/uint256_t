#include "uint256_t.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>

const uint256_t uint256_0 = 0;
const uint256_t uint256_1 = 1;

uint256_t uint256_t::multiply(const uint256_t & lhs, const uint256_t & rhs)
{
	auto lhs_msb_bit_number = lhs.bits();
	auto lhs_msb_exponential = uint256_1<<(lhs_msb_bit_number-1);
	auto rhs_msb_bit_number = rhs.bits();
	auto rhs_msb_exponential = uint256_1<<(rhs_msb_bit_number-1);
	if(lhs == uint256_0 || rhs == uint256_0)
	{
		return uint256_0;
	}
	else if(lhs == lhs_msb_exponential) // Save calculations for 2^n numbers
	{
		return rhs << (lhs_msb_bit_number-1);
	}
	else if(rhs == rhs_msb_exponential)
	{
		return lhs << (rhs_msb_bit_number-1);
	}

	// split values into 4 64-bit parts
	uint128_t top[4] = {uint256_t::upper64(lhs.upper128()), uint256_t::lower64(lhs.upper128()), uint256_t::upper64(lhs.lower128()), uint256_t::lower64(lhs.lower128())};
	uint128_t bottom[4] = {uint256_t::upper64(rhs.upper128()), uint256_t::lower64(rhs.upper128()), uint256_t::upper64(rhs.lower128()), uint256_t::lower64(rhs.lower128())};
	uint128_t products[4][4];

	// multiply each component of the values
	for(int y = 3; y > -1; y--){
		for(int x = 3; x > -1; x--){
			products[3 - y][x] = top[x] * bottom[y];
		}
	}

	// first row
	uint128_t fourth64 = uint128_t(uint256_t::lower64(products[0][3]));
	uint128_t third64  = uint128_t(uint256_t::lower64(products[0][2])) + uint128_t(uint256_t::upper64(products[0][3]));
	uint128_t second64 = uint128_t(uint256_t::lower64(products[0][1])) + uint128_t(uint256_t::upper64(products[0][2]));
	uint128_t first64  = uint128_t(uint256_t::lower64(products[0][0])) + uint128_t(uint256_t::upper64(products[0][1]));

	// second row
	third64  += uint128_t(uint256_t::lower64(products[1][3]));
	second64 += uint128_t(uint256_t::lower64(products[1][2])) + uint128_t(uint256_t::upper64(products[1][3]));
	first64  += uint128_t(uint256_t::lower64(products[1][1])) + uint128_t(uint256_t::upper64(products[1][2]));

	// third row
	second64 += uint128_t(uint256_t::lower64(products[2][3]));
	first64  += uint128_t(uint256_t::lower64(products[2][2])) + uint128_t(uint256_t::upper64(products[2][3]));

	// fourth row
	first64  += uint128_t(uint256_t::lower64(products[3][3]));

	// combines the values, taking care of carry over
	return uint256_t(first64 << uint128_64, uint128_0) +
			uint256_t(uint256_t::upper64(third64), third64 << uint128_64) +
			uint256_t(second64, uint128_0) +
			uint256_t(fourth64);
}

std::pair <uint256_t, uint256_t> uint256_t::divmod(const uint256_t & lhs, const uint256_t & rhs)
{
	auto rhs_msb_bit_number = rhs.bits();
	auto rhs_msb_exponential = uint256_1<<(rhs_msb_bit_number-1);
	/// Save some calculations
	if (rhs == uint256_0)
	{
		throw std::domain_error("Error: division or modulus by 0");
	}
	else if (rhs == uint256_1)
	{
		return std::pair <uint256_t, uint256_t> (lhs, uint256_0);
	}
	else if (lhs == rhs)
	{
		return std::pair <uint256_t, uint256_t> (uint256_1, uint256_0);
	}
	else if ((lhs == uint256_0) || (lhs < rhs))
	{
		return std::pair <uint256_t, uint256_t> (uint256_0, lhs);
	}
	else if(rhs == rhs_msb_exponential) // check if only one bit is set (2^n)
	{
		return std::pair(lhs >> (rhs_msb_bit_number-1), lhs & (rhs_msb_exponential-1));
	}

	std::pair <uint256_t, uint256_t> qr(uint256_0, lhs);
	auto msb_order_diff = lhs.bits() - rhs_msb_bit_number;
	uint256_t copyd = rhs << msb_order_diff;
	uint256_t adder = uint256_1 << msb_order_diff;
	while (qr.second >= rhs)
	{
		if (qr.second >= copyd)
		{
			qr.second -= copyd;
			qr.first |= adder;
		}
		copyd >>= uint256_1;
		adder >>= uint256_1;
	}
	return qr;
}

uint256_t uint256_t::exp(const uint256_t & base, const uint256_t & exponent)
{
	auto result = uint256_1;
	auto l_exp = exponent;
	while(l_exp > uint128_0)
	{
		result *= base;
		--l_exp;
	}
	return result;
}

/// Get order of msb bit.
/// Return 0 if value == 0, otherwise [1 ... 256].
uint16_t uint256_t::bits() const{
	uint16_t out = 0;
	if(upper > std::numeric_limits<uint64_t>::max())
	{
		out = 256 - static_cast<uint16_t>(__builtin_clzll(upper >> 64)); // upper has to be > 0
	}
	else if (upper > 0)
	{
		out = 192 - static_cast<uint16_t>(__builtin_clzll(static_cast<uint64_t>(upper))); // upper has to be > 0
	}
	else if(lower > std::numeric_limits<uint64_t>::max())
	{
		out = 128 - static_cast<uint16_t>(__builtin_clzll(lower >> 64)); // lower has to be > 0
	}
	else if(lower > 0)
	{
		out = 64 - static_cast<uint16_t>(__builtin_clzll(static_cast<uint64_t>(lower))); // lower has to be > 0
	}
	return out;
}

/// stream operators

/// out stream supports hex, oct and dec formatting
std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs)
{
	if(rhs == uint256_0)
	{
		// for value 0 at least one zero should be added to output stream
		stream << '0';
	}
	else
	{
		auto gen_out = [](std::ostream & stream, const uint256_t & hs, uint256_t divider, int width) -> void
		{
			std::vector<uint64_t> u64_vec;
			auto u256 = hs;
			while (u256>uint256_0)
			{
				auto pair = uint256_t::divmod(u256,	divider);
				u64_vec.push_back( pair.second );
				u256 = pair.first;
			}
			auto it = u64_vec.rbegin();
			while(it != u64_vec.rend())
			{
				if(it == u64_vec.rbegin())
				{
					stream << *it;
				}
				else
				{
					stream << std::internal << std::setfill('0') << std::setw(width)<< *it;
				}
				++it;
			}
		};
		if(stream.flags() & std::ios::hex)
		{
			// divide with max 16^n dec number for 64-bit size (2^64 == 16^16)
			gen_out(stream,rhs,uint256_1<<64,16);
		}
		else if(stream.flags() & std::ios::oct)
		{
			// divide with max 8^n dec number for 64-bit size (2^63 == 8^21)
			gen_out(stream,rhs,01000000000000000000000,21);
		}
		else if(stream.flags() & std::ios::dec)
		{
			// divide with max 10^n dec number for 64-bit size (10^19)
			gen_out(stream,rhs,10000000000000000000ULL,19);
		}
	}
	return stream;
}

/// In stream operator accepts hex, dec and oct formated strings
std::istream & operator>>(std::istream & stream, uint256_t & rhs)
{
	std::string sv;
	stream >> sv;
	bool is_hex = sv.compare(0, 2, "0x") == 0 && sv.size() > 2 && sv.size() <= 66 && sv.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
	bool is_oct = sv.compare(0, 1, "0") == 0 && sv.size() > 1 && sv.size() <= 86 && sv.find_first_not_of("01234567", 1) == std::string::npos;
	bool is_dec = sv.size() > 0 && sv.size() <= 80 && sv.find_first_not_of("0123456789", 0) == std::string::npos;
	if(is_hex)
	{
		rhs = uint256_0;
		sv = sv.substr(2);
		std::vector<uint64_t> result;
		uint256_t base = 16;
		uint256_t exponent = uint256_0;
		while(sv.size())
		{
			size_t idx;
			auto size = std::max(sv.size(),16UL);
			auto num = std::stoul(sv.substr(0,size),&idx,16);
//			rhs += uint256_t(num) * exp(base,exponent);
		}
		if(result.size() < 4)
		{
			result.resize(4);
		}
		rhs = {result[3],result[2],result[1],result[0]};
	}
	return stream;
}


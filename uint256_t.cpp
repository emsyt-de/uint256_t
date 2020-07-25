#include "uint256_t.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>

const uint256_t uint256_0 = 0;
const uint256_t uint256_1 = 1;

std::pair <uint256_t, uint256_t> uint256_t::divmod(const uint256_t & lhs, const uint256_t & rhs)
{
	auto hrs_msb_bit_number = rhs.bits();
	auto hrs_msb_exponential = uint256_1<<(hrs_msb_bit_number-1);
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
	else if(rhs == hrs_msb_exponential) // check if only one bit is set (2^n)
	{
		return std::pair(lhs >> (hrs_msb_bit_number-1), lhs & (hrs_msb_exponential-1));
	}

	std::pair <uint256_t, uint256_t> qr(uint256_0, lhs);
	auto msb_order_diff = lhs.bits() - hrs_msb_bit_number;
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

/// Accepts hex format string
std::istream & operator>>(std::istream & stream, uint256_t & rhs)
{
	std::string sv;
	stream >> sv;
	bool is_hex = sv.compare(0, 2, "0x") == 0 && sv.size() > 2 && sv.size() <= 66 && sv.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
	if(is_hex)
	{
		sv = sv.substr(2);
		std::vector<uint64_t> result;
		while(sv.size())
		{
			size_t idx;
			auto size = std::max(sv.size(),16UL);
			auto num = std::stoul(sv.substr(0,size),&idx,16);
			result.push_back(num);
			sv.substr(idx);
		}
		if(result.size() < 4)
		{
			result.resize(4);
		}
		rhs = {result[3],result[2],result[1],result[0]};
	}
	return stream;
}


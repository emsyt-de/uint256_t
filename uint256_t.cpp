#include "uint256_t.h"
#include <sstream>
#include <vector>
#include <iomanip>

const uint256_t uint256_0 = 0;
const uint256_t uint256_1 = 1;


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
			// divide with max 16^n hex number for 64-bit size (2^64 == 16^16)
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
	std::string s;
	stream >> s;
	if(!rhs.init(s))
	{
		stream.setstate(std::ios::failbit);
	}
	return stream;
}


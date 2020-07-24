#include "uint256_t.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>

const uint256_t uint256_0 = 0;
const uint256_t uint256_1 = 1;

std::pair <uint256_t, uint256_t> uint256_t::divmod(const uint256_t & lhs, const uint256_t & rhs) {
	// Save some calculations /////////////////////
	if (rhs == uint256_0){
		throw std::domain_error("Error: division or modulus by 0");
	}
	else if (rhs == uint256_1){
		return std::pair <uint256_t, uint256_t> (lhs, uint256_0);
	}
	else if (lhs == rhs){
		return std::pair <uint256_t, uint256_t> (uint256_1, uint256_0);
	}
	else if ((lhs == uint256_0) || (lhs < rhs)){
		return std::pair <uint256_t, uint256_t> (uint256_0, lhs);
	}

	std::pair <uint256_t, uint256_t> qr(uint256_0, lhs);
	uint256_t copyd = rhs << (lhs.bits() - rhs.bits());
	uint256_t adder = uint256_1 << (lhs.bits() - rhs.bits());
	if (copyd > qr.second){
		copyd >>= uint256_1;
		adder >>= uint256_1;
	}
	while (qr.second >= rhs){
		if (qr.second >= copyd){
			qr.second -= copyd;
			qr.first |= adder;
		}
		copyd >>= uint256_1;
		adder >>= uint256_1;
	}
	return qr;
}

const uint128_t & uint256_t::upper128() const {
	return UPPER;
}

const uint128_t & uint256_t::lower128() const {
	return LOWER;
}

/// Get order of msb bit.
/// Return 0 if value == 0, otherwise [1 ... 256].
uint16_t uint256_t::bits() const{
	uint16_t out = 0;
	if(UPPER > std::numeric_limits<uint64_t>::max())
	{
		uint64_t upper = UPPER >> 64;
		out = 256 - static_cast<uint16_t>(__builtin_clzll(upper)); // upper has to be > 0
	}
	else if (UPPER > 0)
	{
		uint64_t upper = static_cast<uint64_t>(UPPER);
		out = 192 - static_cast<uint16_t>(__builtin_clzll(upper)); // upper has to be > 0
	}
	else if(LOWER > std::numeric_limits<uint64_t>::max())
	{
		uint64_t lower = LOWER >> 64;
		out = 128 - static_cast<uint16_t>(__builtin_clzll(lower)); // lower has to be > 0
	}
	else if(LOWER > 0)
	{
		uint64_t lower = static_cast<uint64_t>(LOWER);
		out = 64 - static_cast<uint16_t>(__builtin_clzll(lower)); // lower has to be > 0
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
		std::ostringstream testwidth;
		testwidth.setf(stream.flags());
		testwidth << std::numeric_limits<uint64_t>::max();
		auto width = static_cast<int>(testwidth.str().size());
		auto u64_quarter = { uint256_t::upper64(rhs.upper128()), uint256_t::lower64(rhs.upper128()), uint256_t::upper64(rhs.lower128()), uint256_t::lower64(rhs.lower128())};
		for(auto u64: u64_quarter)
		{
			if(u64)
			{
				stream << u64;
				stream << std::left << std::setfill('0') << std::setw(width);
			}
		}
	}
	return stream;
}

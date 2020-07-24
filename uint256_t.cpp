#include "uint256_t.h"
#include <vector>
#include <cstring>

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

//std::vector<uint8_t> uint256_t::export_bits() const {
//    std::vector<uint8_t> ret;
//    ret.reserve(32);
//    UPPER.export_bits(ret);
//    LOWER.export_bits(ret);
//    return ret;
//}

//std::vector<uint8_t> uint256_t::export_bits_truncate() const {
//    std::vector<uint8_t> ret = export_bits();

//	//prune the zeroes
//	int i = 0;
//	while (ret[i] == 0 && i < 64) i++;
//	ret.erase(ret.begin(), ret.begin() + i);

//	return ret;
//}

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

std::string uint256_t::str(uint8_t base, const unsigned int & len) const{
    if ((base < 2) || (base > 36)){
        throw std::invalid_argument("Base must be in the range 2-36");
    }
    std::string out = "";
    if (!(*this)){
        out = "0";
    }
    else{
        std::pair <uint256_t, uint256_t> qr(*this, uint256_0);
        do{
            qr = divmod(qr.first, base);
            out = "0123456789abcdefghijklmnopqrstuvwxyz"[(uint8_t) qr.second] + out;
        } while (qr.first);
    }
    if (out.size() < len){
        out = std::string(len - out.size(), '0') + out;
    }
    return out;
}

// stream operators
std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs){
    if (stream.flags() & stream.oct){
        stream << rhs.str(8);
    }
    else if (stream.flags() & stream.dec){
        stream << rhs.str(10);
    }
    else if (stream.flags() & stream.hex){
        stream << rhs.str(16);
    }
    return stream;
}

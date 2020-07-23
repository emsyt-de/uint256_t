#include "uint256_t.h"
#include <vector>
#include <cstring>

const uint256_t uint256_0(0);
const uint256_t uint256_1(1);
const uint256_t uint256_max(-1);


uint256_t & uint256_t::operator=(const uint256_t & rhs){
    UPPER = rhs.UPPER;
    LOWER = rhs.LOWER;
    return *this;
}

uint256_t & uint256_t::operator=(uint256_t && rhs){
    if (this != &rhs){
        UPPER = std::move(rhs.UPPER);
        LOWER = std::move(rhs.LOWER);
        rhs.UPPER = uint128_0;
        rhs.LOWER = uint128_0;
    }
    return *this;
}

uint256_t::operator bool() const{
    return (bool) (UPPER | LOWER);
}

uint256_t::operator uint8_t() const{
    return (uint8_t) LOWER;
}

uint256_t::operator uint16_t() const{
    return (uint16_t) LOWER;
}

uint256_t::operator uint32_t() const{
    return (uint32_t) LOWER;
}

uint256_t::operator uint64_t() const{
    return (uint64_t) LOWER;
}

uint256_t::operator uint128_t() const{
    return LOWER;
}


uint256_t uint256_t::operator<<(const uint128_t & rhs) const{
    return *this << uint256_t(rhs);
}

uint256_t uint256_t::operator<<(const uint256_t & rhs) const{
    const uint128_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) || (shift >= uint128_256)){
        return uint256_0;
    }
    else if (shift == uint128_128){
        return uint256_t(LOWER, uint128_0);
    }
    else if (shift == uint128_0){
        return *this;
    }
    else if (shift < uint128_128){
        return uint256_t((UPPER << shift) + (LOWER >> (uint128_128 - shift)), LOWER << shift);
    }
    else if ((uint128_256 > shift) && (shift > uint128_128)){
        return uint256_t(LOWER << (shift - uint128_128), uint128_0);
    }
    else{
        return uint256_0;
    }
}

uint256_t & uint256_t::operator<<=(const uint128_t & shift){
    return *this <<= uint256_t(shift);
}

uint256_t & uint256_t::operator<<=(const uint256_t & shift){
    *this = *this << shift;
    return *this;
}

uint256_t uint256_t::operator>>(const uint128_t & rhs) const{
    return *this >> uint256_t(rhs);
}

uint256_t uint256_t::operator>>(const uint256_t & rhs) const{
    const uint128_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) | (shift >= uint128_256)){
        return uint256_0;
    }
    else if (shift == uint128_128){
        return uint256_t(UPPER);
    }
    else if (shift == uint128_0){
        return *this;
    }
    else if (shift < uint128_128){
        return uint256_t(UPPER >> shift, (UPPER << (uint128_128 - shift)) + (LOWER >> shift));
    }
    else if ((uint128_256 > shift) && (shift > uint128_128)){
        return uint256_t(UPPER >> (shift - uint128_128));
    }
    else{
        return uint256_0;
    }
}

uint256_t & uint256_t::operator>>=(const uint128_t & shift){
    return *this >>= uint256_t(shift);
}

uint256_t & uint256_t::operator>>=(const uint256_t & shift){
    *this = *this >> shift;
    return *this;
}

bool uint256_t::operator!() const{
    return ! (bool) *this;
}

bool uint256_t::operator&&(const uint128_t & rhs) const{
    return (*this && uint256_t(rhs));
}

bool uint256_t::operator&&(const uint256_t & rhs) const{
    return ((bool) *this && (bool) rhs);
}

bool uint256_t::operator||(const uint128_t & rhs) const{
    return (*this || uint256_t(rhs));
}

bool uint256_t::operator||(const uint256_t & rhs) const{
    return ((bool) *this || (bool) rhs);
}

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

uint256_t & uint256_t::operator++(){
    *this += uint256_1;
    return *this;
}

uint256_t uint256_t::operator++(int){
    uint256_t temp(*this);
    ++*this;
    return temp;
}

uint256_t & uint256_t::operator--(){
    *this -= uint256_1;
    return *this;
}

uint256_t uint256_t::operator--(int){
    uint256_t temp(*this);
    --*this;
    return temp;
}

uint256_t uint256_t::operator+() const{
    return *this;
}

uint256_t uint256_t::operator-() const{
    return ~*this + uint256_1;
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
    if (UPPER){
        out = 128;
        uint128_t up = UPPER;
        while (up){
            up >>= uint128_1;
            out++;
        }
    }
    else{
        uint128_t low = LOWER;
        while (low){
            low >>= uint128_1;
            out++;
        }
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

uint256_t operator<<(const bool & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const uint8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const uint16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const uint32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const uint64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

#include <iostream>

uint256_t operator<<(const uint128_t & lhs, const uint256_t & rhs){
	return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const int8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const int16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const int32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint256_t operator<<(const int64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

uint128_t & operator<<=(uint128_t & lhs, const uint256_t & rhs){
	lhs = (uint256_t(lhs) << rhs).lower128();
    return lhs;
}

uint256_t operator>>(const bool & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const uint8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const uint16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const uint32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const uint64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const uint128_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const int8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const int16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const int32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint256_t operator>>(const int64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

uint128_t & operator>>=(uint128_t & lhs, const uint256_t & rhs){
	lhs = (uint256_t(lhs) >> rhs).lower128();
    return lhs;
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

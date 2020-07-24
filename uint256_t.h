	/*
	uint256_t.h
	An unsigned 256 bit integer library for C++

	Copyright (c) 2013 - 2017 Jason Lee @ calccrypto at gmail.com

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

	With much help from Auston Sterling

	Thanks to Fran�ois Dessenne for convincing me
	to do a general rewrite of this class.
	*/

	#ifndef __UINT256_T__
	#define __UINT256_T__

	#include <cstdint>
	#include <ostream>
	#include <vector>
	#include <stdexcept>
	#include <type_traits>
	#include <concepts>
	#include <utility>

	typedef __uint128_t uint128_t;
	class uint256_t;

	// Give uint256_t type traits
	namespace std {  // This is probably not a good idea
	template <> struct is_arithmetic <uint256_t> : std::true_type {};
	template <> struct is_integral   <uint256_t> : std::true_type {};
	template <> struct is_unsigned   <uint256_t> : std::true_type {};
	}

	class uint256_t {

	public:

		/// Constructors
		uint256_t() = default;
		uint256_t(const uint256_t & rhs) = default;
		uint256_t(uint256_t && rhs) = default;

		template <std::integral S, std::integral T>
		uint256_t(const S & upper_rhs, const T & lower_rhs)
			: upper(upper_rhs), lower(lower_rhs)
		{}

		uint256_t(const uint128_t & upper_rhs, const uint128_t & lower_rhs)
			: upper(upper_rhs), lower(lower_rhs)
		{}
		constexpr uint256_t(const uint128_t & lower_rhs)
			: upper(uint128_0), lower(lower_rhs)
		{}

		template <std::integral R, std::integral S, std::integral T, std::integral U>
		uint256_t(const R & upper_lhs, const S & lower_lhs, const T & upper_rhs, const U & lower_rhs)
		{
			upper = upper_lhs;
			upper = upper << 64;
			upper |= lower_lhs;

			lower = upper_rhs;
			lower = lower << 64;
			lower |= lower_rhs;
		}

		/// Assignment Operators
		uint256_t & operator=(const uint256_t & rhs) = default;
		uint256_t & operator=(uint256_t && rhs) = default;

		/// Typecast Operators
		constexpr operator bool() const
		{
			return static_cast<bool>(lower) || static_cast<bool>(upper);
		}

		constexpr operator uint8_t() const
		{
			return static_cast<uint8_t>(lower);
		}

		constexpr operator uint16_t() const
		{
			return static_cast<uint16_t>(lower);
		}

		constexpr operator uint32_t() const
		{
			return static_cast<uint32_t>(lower);
		}

		constexpr operator uint64_t() const
		{
			return static_cast<uint64_t>(lower);
		}

		constexpr operator uint128_t() const
		{
			return lower;
		}

		/// Bitwise Operators

		/// And
		template <std::integral T1, std::integral T2>
		friend inline constexpr uint256_t operator&(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				return uint256_t(lhs.upper & rhs.upper, lhs.lower & rhs.lower);
			}
			else if constexpr (std::is_same_v<T1,uint256_t>)
			{
				return uint256_t(uint128_0, lhs.lower & static_cast<uint128_t>(rhs));
			}
			else if constexpr (std::is_same_v<T2,uint256_t>)
			{
				return uint256_t(uint128_0, static_cast<uint128_t>(lhs) & rhs.lower);
			}
			else
			{
				return rhs & lhs;
			}
		}

		template <std::integral T1, std::integral T2>
		friend inline constexpr T1 operator&=(T1 & lhs, const T2 & rhs)
		{
			return lhs = (lhs & rhs);
		}

		/// Or
		template <std::integral T1, std::integral T2>
		friend inline constexpr uint256_t operator|(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				return uint256_t(lhs.upper | rhs.upper, lhs.lower | rhs.lower);
			}
			else if constexpr (std::is_same_v<T1,uint256_t>)
			{
				return uint256_t(lhs.upper, lhs.lower | static_cast<uint128_t>(rhs));
			}
			else if constexpr (std::is_same_v<T2,uint256_t>)
			{
				return uint256_t(rhs.upper, static_cast<uint128_t>(lhs) | rhs.lower);
			}
			else
			{
				return rhs | lhs;
			}
		}

		template <std::integral T1, std::integral T2>
		friend inline constexpr T1 operator|=(T1 & lhs, const T2 & rhs)
		{
			return lhs = (lhs | rhs);
		}

		/// Xor
		template <std::integral T1, std::integral T2>
		friend inline constexpr uint256_t operator^(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				return uint256_t(lhs.upper ^ rhs.upper, lhs.lower ^ rhs.lower);
			}
			else if constexpr (std::is_same_v<T1,uint256_t>)
			{
				return uint256_t(lhs.upper, lhs.lower ^ static_cast<uint128_t>(rhs));
			}
			else if constexpr (std::is_same_v<T2,uint256_t>)
			{
				return uint256_t(rhs.upper, static_cast<uint128_t>(lhs) ^ rhs.lower);
			}
			else
			{
				return rhs ^ lhs;
			}
		}

		template <std::integral T1, std::integral T2>
		friend inline constexpr T1 operator^=(T1 & lhs, const T2 & rhs)
		{
			return lhs = (lhs ^ rhs);
		}

		/// Invert operator
		inline uint256_t operator~() const {
			return uint256_t(~upper, ~lower);
		}

		/// Bit Shift Operators

		/// Shift left
		template <std::integral T1, std::integral T2>
		friend inline constexpr uint256_t operator<<(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				const uint128_t shift = rhs.lower;
				if (rhs.upper || (shift >= uint128_256)){
					return 0;
				}
				else if (shift == uint128_128){
					return uint256_t(lhs.lower, uint128_0);
				}
				else if (shift == uint128_0){
					return lhs;
				}
				else if (shift < uint128_128){
					return uint256_t((lhs.upper << shift) + (lhs.lower >> (uint128_128 - shift)), lhs.lower << shift);
				}
				else if ((uint128_256 > shift) && (shift > uint128_128)){
					return uint256_t(lhs.lower << (shift - uint128_128), uint128_0);
				}
				else{
					return 0;
				}
			}
			else
			{
				return uint256_t(lhs) << uint256_t(rhs);
			}
		}

		template <std::integral T1, std::integral T2>
		friend inline constexpr T1 operator<<=(T1 & lhs, const T2 & rhs)
		{
			return lhs = (lhs << rhs);
		}

		/// Shift right
		template <std::integral T1, std::integral T2>
		friend inline constexpr uint256_t operator>>(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				const uint128_t shift = rhs.lower;
				if (rhs.upper || (shift >= uint128_256)){
					return 0;
				}
				else if (shift == uint128_128){
					return lhs.upper;
				}
				else if (shift == uint128_0){
					return lhs;
				}
				else if (shift < uint128_128){
					return uint256_t(lhs.upper >> shift, (lhs.upper << (uint128_128 - shift)) + (lhs.lower >> shift));
				}
				else if ((uint128_256 > shift) && (shift > uint128_128)){
					return uint256_t(lhs.upper >> (shift - uint128_128));
				}
				else{
					return 0;
				}
			}
			else
			{
				return uint256_t(lhs) >> uint256_t(rhs);
			}
		}

		template <std::integral T1, std::integral T2>
		friend inline constexpr T1 operator>>=(T1 & lhs, const T2 & rhs)
		{
			return lhs = (lhs >> rhs);
		}

		/// Comparison Operators

		/// Equal
		template <std::integral T1, std::integral T2>
		friend inline bool operator==(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				return lhs.upper == rhs.upper && lhs.lower == rhs.lower;
			}
			else if constexpr (std::is_same_v<T1,uint256_t>)
			{
				return !lhs.upper && lhs.lower == static_cast<uint128_t>(rhs);
			}
			else if constexpr (std::is_same_v<T2,uint256_t>)
			{
				return !rhs.upper && static_cast<uint128_t>(lhs) == rhs.lower;
			}
			else
			{
				return rhs == lhs;
			}

		}

		/// Three way compare
		template <std::integral T1, std::integral T2>
		friend inline constexpr std::strong_ordering operator<=>(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				auto comp_upper = lhs.upper <=> rhs.upper;
				auto comp_lower = lhs.lower <=> rhs.lower;
				return lhs.upper != rhs.upper ? comp_upper : comp_lower;
			}
			else if constexpr (std::is_same_v<T1,uint256_t>)
			{
				return lhs.upper ? std::strong_ordering::greater : lhs.lower <=> static_cast<uint128_t>(rhs);
			}
			else if constexpr (std::is_same_v<T2,uint256_t>)
			{
				return rhs.upper ? std::strong_ordering::less : static_cast<uint128_t>(lhs) <=> rhs.lower;
			}
			else
			{
				return rhs <=> lhs;
			}
		}

		/// Arithmetic Operators

		/// Plus
		template <std::integral T1, std::integral T2>
		friend inline uint256_t operator+(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				const auto lower_sum = lhs.lower128() + rhs.lower128();
				return uint256_t(lhs.upper128() + rhs.upper128() + (lower_sum < lhs.lower128() ? uint128_1:uint128_0), lower_sum);
			}
			else
			{
				return uint256_t(lhs) + uint256_t(rhs);
			}
		}

		template <std::integral T1, std::integral T2>
		friend inline T1 & operator+=(T1 & lhs, const T2 & rhs){
			return lhs = static_cast <T1> (lhs + rhs);
		}

		/// Minus
		template <std::integral T1, std::integral T2>
		friend inline uint256_t operator-(const T1 & lhs, const T2 & rhs)
		{
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				const auto lower_minus = lhs.lower128() - rhs.lower128();
				return uint256_t(lhs.upper128() - rhs.upper128() - (lower_minus > lhs.lower128()), lower_minus);
			}
			else
			{
				return uint256_t(lhs) - uint256_t(rhs);
			}
		}


		template <std::integral T1, std::integral T2>
		friend inline T1 & operator-=(T1 & lhs, const T2 & rhs){
			return lhs = static_cast <T1> (lhs - rhs);
		}

		/// Multiply
		template <std::integral T1, std::integral T2>
		friend inline uint256_t operator*(const T1 & lhs, const T2 & rhs){
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
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
			else
			{
				return uint256_t(lhs) * uint256_t(rhs);
			}
		}

		template <std::integral T1, std::integral T2 >
		friend inline T1 & operator*=(T1 & lhs, const T2 & rhs){
			return lhs = static_cast <T1> (lhs * rhs);
		}

		/// Divide
		template <std::integral T1, std::integral T2>
		friend inline uint256_t operator/(const T1 & lhs, const T2 & rhs){
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				return uint256_t::divmod(lhs,rhs).first;
			}
			else
			{
				return uint256_t(lhs) / uint256_t(rhs);
			}
		}

		template <std::integral T1, std::integral T2 >
		friend inline T1 & operator/=(T1 & lhs, const T2 & rhs){
			return lhs = static_cast <T1> (lhs / rhs);
		}

		/// Modulo
		template <std::integral T1, std::integral T2>
		friend inline uint256_t operator%(const T1 & lhs, const T2 & rhs){
			if constexpr (std::is_same_v<T1,T2> && std::is_same_v<T1,uint256_t>)
			{
				return divmod(lhs,rhs).second;
			}
			else
			{
				return uint256_t(lhs) % uint256_t(rhs);
			}
		}

		template <std::integral T1, std::integral T2 >
		friend inline T1 & operator%=(T1 & lhs, const T2 & rhs){
			return lhs = static_cast <T1> (lhs % rhs);
		}


		/// Increment Operators

		/// pre increment
		friend uint256_t & operator++(uint256_t & hs)
		{
			static constexpr uint256_t uint256_1(1);
			hs += uint256_1;
			return hs;
		}

		/// post increment
		friend uint256_t operator++(uint256_t & hs, int)
		{
			static constexpr uint256_t uint256_1(1);
			auto temp(hs);
			hs += uint256_1;
			return temp;
		}

		/// pre decrement
		friend uint256_t & operator--(uint256_t & hs)
		{
			static constexpr uint256_t uint256_1(1);
			hs -= uint256_1;
			return hs;
		}

		/// post decrement
		friend uint256_t operator--(uint256_t & hs, int)
		{
			static constexpr uint256_t uint256_1(1);
			auto temp(hs);
			hs -= uint256_1;
			return temp;
		}

		/// positive sign operator
		friend uint256_t operator+(const uint256_t & hs)
		{
			return hs;
		}

		/// negative sign operator
		friend uint256_t operator-(const uint256_t & hs)
		{
			static constexpr uint256_t uint256_1(1);
			return ~hs + uint256_1;
		}

		/// IO Operator
		friend std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs);

		/// Get private values
		const uint128_t & upper128() const { return upper; }
		const uint128_t & lower128() const { return lower; }

		static inline uint64_t upper64(uint128_t u128) {
			return u128>>64;
		}

		static inline uint64_t lower64(uint128_t u128) {
			return static_cast<uint64_t>(u128);
		}

		/// Get order of msb bit.
		/// Return 0 if value == 0, otherwise [1 ... 256].
		uint16_t bits() const;
	private:
		static std::pair <uint256_t, uint256_t> divmod(const uint256_t & lhs, const uint256_t & rhs);
		void init(const std::string_view& s);

		inline static constexpr uint128_t uint128_0 = 0;
		inline static constexpr uint128_t uint128_1 = 1;
		inline static constexpr uint128_t uint128_64 = 64;
		inline static constexpr uint128_t uint128_128 = 128;
		inline static constexpr uint128_t uint128_256 = 256;

		uint128_t upper = 0, lower = 0;


	};

	#endif

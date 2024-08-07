/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigint.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:44:50 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/07 16:23:51 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gomoku.hpp>

#ifndef BIGINT_HPP
# define BIGINT_HPP

class BigInt
{
	private:
		mpz_t _value;

	public:
		// Default Constructor
		BigInt();

		// Constructor with string input
		BigInt(const std::string& initial_value);

		BigInt(const size_t& initial_value);

		// Copy Constructor
		BigInt(const BigInt& other);

		// Move Constructor
		BigInt(BigInt&& other) noexcept;

		// Destructor
		~BigInt();

		// Copy Assignment Operator
		BigInt& operator=(const BigInt& other);

		// Move Assignment Operator
		BigInt& operator=(BigInt&& other) noexcept;
		// Arithmetic Operators BigInt
		BigInt operator+(const BigInt& other) const;

		BigInt operator-(const BigInt& other) const;

		BigInt operator*(const BigInt& other) const;

		BigInt operator/(const BigInt& other) const;

		BigInt operator%(const BigInt& other) const;

		// Bitwise Operators
		BigInt operator&(const BigInt& other) const;

		BigInt operator|(const BigInt& other) const;

		BigInt operator^(const BigInt& other) const;

		BigInt operator~() const;

		// Shift Operators
		BigInt operator<<(unsigned int shift) const;

		BigInt operator>>(unsigned int shift) const;

		// Comparison Operators
		bool operator==(const BigInt& other) const;

		bool operator!=(const BigInt& other) const;

		bool operator<(const BigInt& other) const;

		bool operator<=(const BigInt& other) const;

		bool operator>(const BigInt& other) const;

		bool operator>=(const BigInt& other) const;



		// Size_t Operators
		// Arithmetic Operators
		BigInt operator+(size_t& other) const;

		BigInt operator-(size_t& other) const;

		BigInt operator*(size_t& other) const;

		BigInt operator/(size_t& other) const;

		BigInt operator%(size_t& other) const;

		// Bitwise Operators
		BigInt operator&(size_t& other) const;

		BigInt operator|(size_t& other) const;

		BigInt operator^(size_t& other) const;


		// Comparison Operators
		bool operator==(size_t& other) const;

		bool operator!=(size_t& other) const;

		bool operator<(size_t& other) const;

		bool operator<=(size_t& other) const;
		bool operator>(size_t& other) const;
		bool operator>=(size_t& other) const;


		// Increment and Decrement
		BigInt& operator++();

		BigInt operator++(int);
		BigInt& operator--();

		BigInt operator--(int);


		// Output Stream
		friend std::ostream& operator<<(std::ostream& os, const BigInt& big_int);

		// Input Stream
		friend std::istream& operator>>(std::istream& is, BigInt& big_int);
};


#endif

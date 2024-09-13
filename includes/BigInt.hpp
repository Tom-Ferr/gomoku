
# include <gomoku.hpp>

#ifndef BIGINT_HPP
# define BIGINT_HPP


class BigInt
{
	private:
		mpz_t _value;

	public:
		static const BigInt zero;

		/* tempoprary value for all operations*/
		static BigInt _tmp;
		size_t _mask_pos = 0;

		/*
		** time saving operations, reducing BigInt creation
		*/
		static BigInt &bi_and(const BigInt &one, const BigInt &other);
		static bool and_equal(const BigInt &one, const BigInt &other, const BigInt &equals_to);
		static bool and_equal_zero(const BigInt &one, const BigInt &other);

		/*
		** applies the mask to both one and other and then performs the bitwise or
		*/
		static BigInt &masked_bitwise_or(const BigInt &one, const BigInt &other, const BigInt &mask);


		// Default Constructor
		BigInt();

		// Constructor with string input
		BigInt(const std::string& initial_value, int base=10);

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
		BigInt &operator&(const BigInt& other) const;

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
		BigInt operator+(size_t other) const;

		BigInt operator-(size_t other) const;

		BigInt operator*(size_t other) const;

		BigInt operator/(size_t other) const;

		BigInt operator%(size_t other) const;

		// Bitwise Operators
		BigInt &operator&(size_t other) const;

		BigInt operator|(size_t other) const;

		BigInt operator^(size_t other) const;


		// Comparison Operators
		bool operator==(size_t other) const;

		bool operator!=(size_t other) const;

		bool operator<(size_t other) const;

		bool operator<=(size_t other) const;
		bool operator>(size_t other) const;
		bool operator>=(size_t other) const;


		// Increment and Decrement
		BigInt& operator++();

		BigInt operator++(int);
		BigInt& operator--();

		BigInt operator--(int);

		void set_bit(size_t bit);
		bool get_bit(size_t bit);

		size_t bitCount() const;
		void clear_bit(size_t bit);

		size_t size() const;
		size_t pos() const;

		void set_maskpos(size_t pos);
		size_t get_maskpos() const;

		// Output Stream
		friend std::ostream& operator<<(std::ostream& os, const BigInt& big_int);

		// Input Stream
		friend std::istream& operator>>(std::istream& is, BigInt& big_int);
};


#endif

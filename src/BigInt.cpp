#include <BigInt.hpp>

BigInt BigInt::tmp = BigInt();
BigInt const BigInt::zero = 0;


BigInt &BigInt::bi_and(const BigInt &one, const BigInt &other)
{
	mpz_and(tmp._value, one._value, other._value);
	return tmp;
}

bool BigInt::and_equal(const BigInt &one, const BigInt &other, const BigInt &equals_to)
{
	bi_and(one, other);
	return tmp == equals_to;
}

bool BigInt::and_equal_zero(const BigInt &one, const BigInt &other)
{
	return and_equal(one, other, BigInt::zero);
}

/*
** applies the mask to both one and other and then performs the bitwise or
*/
BigInt &BigInt::masked_bitwise_or(const BigInt &one, const BigInt &other, const BigInt &mask)
{

	mpz_ior(tmp._value, one._value, other._value);
	bi_and(tmp, mask);
	return tmp;
}

BigInt::BigInt()
{
	mpz_init(_value);
}

BigInt::BigInt(const std::string& initial__value, int base)
{
	mpz_init(_value);
	mpz_set_str(_value, initial__value.c_str(), base);
}

BigInt::BigInt(const size_t& initial__value)
{
	mpz_init(_value);
	mpz_set_ui(_value, initial__value);
}

BigInt::BigInt(const BigInt& other)
{
	mpz_init(_value);
	mpz_set(_value, other._value);
}

BigInt::BigInt(BigInt&& other) noexcept
{
	mpz_init(_value);
	mpz_swap(_value, other._value);
	// mpz_clear(other._value);
}

BigInt::~BigInt() {
	mpz_clear(_value);
}

BigInt& BigInt::operator=(const BigInt& other)
{
	if (this != &other)
		mpz_set(_value, other._value);
	return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept
{
	if (this != &other)
	{
		mpz_swap(_value, other._value);
		//mpz_clear(other._value);
	}
	return *this;
}

BigInt BigInt::operator+(const BigInt& other) const
{
	mpz_add(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator-(const BigInt& other) const
{
	mpz_sub(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator*(const BigInt& other) const
{
	mpz_mul(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator/(const BigInt& other) const
{
	mpz_tdiv_q(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator%(const BigInt& other) const
{
	mpz_tdiv_r(tmp._value, _value, other._value);
	return tmp;
}

// Bitwise Operators
BigInt &BigInt::operator&(const BigInt& other) const
{
	mpz_and(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator|(const BigInt& other) const
{
	mpz_ior(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator^(const BigInt& other) const
{
	mpz_xor(tmp._value, _value, other._value);
	return tmp;
}

BigInt BigInt::operator~() const
{
	mpz_com(tmp._value, _value);
	return tmp;
}

BigInt BigInt::operator<<(unsigned int shift) const
{
	mpz_mul_2exp(tmp._value, _value, shift);
	return tmp;
}

BigInt BigInt::operator>>(unsigned int shift) const
{
	mpz_fdiv_q_2exp(tmp._value, _value, shift);
	return tmp;
}

// Comparison Operators
bool BigInt::operator==(const BigInt& other) const
{
	return mpz_cmp(_value, other._value) == 0;
}

bool BigInt::operator!=(const BigInt& other) const
{
	return mpz_cmp(_value, other._value) != 0;
}

bool BigInt::operator<(const BigInt& other) const
{
	return mpz_cmp(_value, other._value) < 0;
}

bool BigInt::operator<=(const BigInt& other) const
{
	return mpz_cmp(_value, other._value) <= 0;
}

bool BigInt::operator>(const BigInt& other) const
{
	return mpz_cmp(_value, other._value) > 0;
}

bool BigInt::operator>=(const BigInt& other) const
{
	return mpz_cmp(_value, other._value) >= 0;
}

// Arithmetic Operators
BigInt BigInt::operator+(size_t& other) const
{
	mpz_add_ui(tmp._value, _value, other);
	return tmp;
}

BigInt BigInt::operator-(size_t& other) const
{
	mpz_sub(tmp._value, _value, BigInt(other)._value);
	return tmp;
}

BigInt BigInt::operator*(size_t& other) const
{
	mpz_mul(tmp._value, _value, BigInt(other)._value);
	return tmp;
}

BigInt BigInt::operator/(size_t& other) const
{
	mpz_tdiv_q(tmp._value, _value, BigInt(other)._value);
	return tmp;
}

BigInt BigInt::operator%(size_t& other) const
{
	mpz_tdiv_r(tmp._value, _value, BigInt(other)._value);
	return tmp;
}

// Bitwise Operators
BigInt &BigInt::operator&(size_t& other) const
{
	mpz_and(tmp._value, _value, BigInt(other)._value);
	return tmp;
}

BigInt BigInt::operator|(size_t& other) const
{
	mpz_ior(tmp._value, _value, BigInt(other)._value);
	return tmp;
}

BigInt BigInt::operator^(size_t& other) const
{
	mpz_xor(tmp._value, _value, BigInt(other)._value);
	return tmp;
}


// Comparison Operators
bool BigInt::operator==(size_t& other) const {
	if (other == 0)
		return mpz_cmp(_value, BigInt::zero._value) == 0;
	return mpz_cmp(_value, BigInt(other)._value) == 0;
}

bool BigInt::operator!=(size_t& other) const {
	if (other == 0)
		return mpz_cmp(_value, BigInt::zero._value) != 0;
	return mpz_cmp(_value, BigInt(other)._value) != 0;
}

bool BigInt::operator<(size_t& other) const {
	if (other == 0)
		return mpz_cmp(_value, BigInt::zero._value) < 0;
	return mpz_cmp(_value, BigInt(other)._value) < 0;
}

bool BigInt::operator<=(size_t& other) const {
	if (other == 0)
		return mpz_cmp(_value, BigInt::zero._value) <= 0;
	return mpz_cmp(_value, BigInt(other)._value) <= 0;
}

bool BigInt::operator>(size_t& other) const {
	if (other == 0)
		return mpz_cmp(_value, BigInt::zero._value) > 0;
	return mpz_cmp(_value, BigInt(other)._value) > 0;
}

bool BigInt::operator>=(size_t& other) const {
	if (other == 0)
		return mpz_cmp(_value, BigInt::zero._value) >= 0;
	return mpz_cmp(_value, BigInt(other)._value) >= 0;
}

// Increment and Decrement
BigInt& BigInt::operator++()
{
	mpz_add_ui(_value, _value, 1);
	return *this;
}

BigInt BigInt::operator++(int)
{
	tmp = *this;
	mpz_add_ui(_value, _value, 1);
	return tmp;
}

BigInt& BigInt::operator--()
{
	mpz_sub_ui(_value, _value, 1);
	return *this;
}

BigInt BigInt::operator--(int)
{
	tmp = *this;
	mpz_sub_ui(_value, _value, 1);
	return tmp;
}

void BigInt::set_bit(size_t bit)
{
	mpz_setbit(_value, bit);
}

bool BigInt::get_bit(size_t bit)
{
	return mpz_tstbit(_value, bit);
}

size_t BigInt::bitCount() const
{
    return mpz_popcount(_value);
}

void BigInt::clear_bit(size_t bit)
{
	mpz_clrbit(_value, bit);
}

size_t BigInt::size() const
{
	return mpz_sizeinbase(_value, 2);
}

std::ostream& operator<<(std::ostream& os, const BigInt& big_int) {
	char* str = mpz_get_str(nullptr, 2, big_int._value);
	os << str;
	free(str);
	return os;
}

std::istream& operator>>(std::istream& is, BigInt& big_int) {
	std::string str;
	is >> str;
	mpz_set_str(big_int._value, str.c_str(), 10);
	return is;
}

size_t BigInt::pos() const
{
	return mpz_scan1(_value, 0);
}

#include <iostream>
#include <string>
#include <gmp.h>
#include <gmpxx.h>

class BigInt {
private:
    mpz_t _value;

public:
    // Default Constructor
    BigInt() {
        mpz_init(_value);
    }

    // Constructor with string input
    BigInt(const std::string& initial_value) {
        mpz_init(_value);
        mpz_set_str(_value, initial_value.c_str(), 10);
    }

    // Copy Constructor
    BigInt(const BigInt& other) {
        mpz_init(_value);
        mpz_set(_value, other._value);
    }

    // Move Constructor
    BigInt(BigInt&& other) noexcept {
        mpz_init(_value);
        mpz_swap(_value, other._value);
        mpz_clear(other._value);
    }

    // Destructor
    ~BigInt() {
        mpz_clear(_value);
    }

    // Copy Assignment Operator
    BigInt& operator=(const BigInt& other) {
        if (this != &other) {
            mpz_set(_value, other._value);
        }
        return *this;
    }

    // Move Assignment Operator
    BigInt& operator=(BigInt&& other) noexcept {
        if (this != &other) {
            mpz_swap(_value, other._value);
            mpz_clear(other._value);
        }
        return *this;
    }

    // Arithmetic Operators
    BigInt operator+(const BigInt& other) const {
        BigInt result;
        mpz_add(result._value, _value, other._value);
        return result;
    }

    BigInt operator-(const BigInt& other) const {
        BigInt result;
        mpz_sub(result._value, _value, other._value);
        return result;
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        mpz_mul(result._value, _value, other._value);
        return result;
    }

    BigInt operator/(const BigInt& other) const {
        BigInt result;
        mpz_tdiv_q(result._value, _value, other._value);
        return result;
    }

    BigInt operator%(const BigInt& other) const {
        BigInt result;
        mpz_tdiv_r(result._value, _value, other._value);
        return result;
    }

    // Bitwise Operators
    BigInt operator&(const BigInt& other) const {
        BigInt result;
        mpz_and(result._value, _value, other._value);
        return result;
    }

    BigInt operator|(const BigInt& other) const {
        BigInt result;
        mpz_ior(result._value, _value, other._value);
        return result;
    }

    BigInt operator^(const BigInt& other) const {
        BigInt result;
        mpz_xor(result._value, _value, other._value);
        return result;
    }

    BigInt operator~() const {
        BigInt result;
        mpz_com(result._value, _value);
        return result;
    }

    // Shift Operators
    BigInt operator<<(unsigned int shift) const {
        BigInt result;
        mpz_mul_2exp(result._value, _value, shift);  // Shift left by 'shift' bits
        return result;
    }

    BigInt operator>>(unsigned int shift) const {
        BigInt result;
        mpz_fdiv_q_2exp(result._value, _value, shift);  // Shift right by 'shift' bits
        return result;
    }

    // Comparison Operators
    bool operator==(const BigInt& other) const {
        return mpz_cmp(_value, other._value) == 0;
    }

    bool operator!=(const BigInt& other) const {
        return mpz_cmp(_value, other._value) != 0;
    }

    bool operator<(const BigInt& other) const {
        return mpz_cmp(_value, other._value) < 0;
    }

    bool operator<=(const BigInt& other) const {
        return mpz_cmp(_value, other._value) <= 0;
    }

    bool operator>(const BigInt& other) const {
        return mpz_cmp(_value, other._value) > 0;
    }

    bool operator>=(const BigInt& other) const {
        return mpz_cmp(_value, other._value) >= 0;
    }

    // Output Stream
    friend std::ostream& operator<<(std::ostream& os, const BigInt& big_int) {
        char* str = mpz_get_str(nullptr, 10, big_int._value);
        os << str;
        free(str);
        return os;
    }

    // Input Stream
    friend std::istream& operator>>(std::istream& is, BigInt& big_int) {
        std::string str;
        is >> str;
        mpz_set_str(big_int._value, str.c_str(), 10);
        return is;
    }

    // Increment and Decrement
    BigInt& operator++() {
        mpz_add_ui(_value, _value, 1);
        return *this;
    }

    BigInt operator++(int) {
        BigInt temp = *this;
        mpz_add_ui(_value, _value, 1);
        return temp;
    }

    BigInt& operator--() {
        mpz_sub_ui(_value, _value, 1);
        return *this;
    }

    BigInt operator--(int) {
        BigInt temp = *this;
        mpz_sub_ui(_value, _value, 1);
        return temp;
    }
};
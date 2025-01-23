#ifndef __METANULL_MATH_H__
#define __METANULL_MATH_H__

#include <stdexcept>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>

/*
* References & Resources:
* https://www.geeksforgeeks.org/euclidean-algorithms-basic-and-extended/
* https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
* https://www.geeksforgeeks.org/lcm-of-given-array-elements/?ref=lbp
* https://www.geeksforgeeks.org/c-program-to-check-prime-number/
*/
namespace metanull {
    namespace math {
        /**
         * @brief Returns the number of digits in n.
         * @param n The number.
         * @return The number of digits in n.
         */
        template <typename T> size_t count_digits(T n);

        /**
         * @brief Returns true if the number of digits in n is even.
         * @param n The number.
         * @return True if the number of digits in n is even, false otherwise.
         */
        template <typename T> bool count_digits_is_even(T n);

        // Explicit template instantiations
        template size_t count_digits<unsigned char>(unsigned char);
        template size_t count_digits<unsigned short>(unsigned short);
        template size_t count_digits<unsigned int>(unsigned int);
        template size_t count_digits<unsigned long>(unsigned long);
        template size_t count_digits<unsigned long long>(unsigned long long);
        template size_t count_digits<signed char>(signed char);
        template size_t count_digits<short>(short);
        template size_t count_digits<int>(int);
        template size_t count_digits<long>(long);
        template size_t count_digits<long long>(long long);

        template bool count_digits_is_even<unsigned char>(unsigned char);
        template bool count_digits_is_even<unsigned short>(unsigned short);
        template bool count_digits_is_even<unsigned int>(unsigned int);
        template bool count_digits_is_even<unsigned long>(unsigned long);
        template bool count_digits_is_even<unsigned long long>(unsigned long long);
        template bool count_digits_is_even<signed char>(signed char);
        template bool count_digits_is_even<short>(short);
        template bool count_digits_is_even<int>(int);
        template bool count_digits_is_even<long>(long);
        template bool count_digits_is_even<long long>(long long);

        /**
         * @brief Returns the prime factors of num.
         * @param num The number to factorize.
         * @return The prime factors of num.
         */
        template <typename T> std::vector<T> factorize(T num);

        // Explicit template instantiations
        template std::vector<char> factorize<char>(char);
        template std::vector<short> factorize<short>(short);
        template std::vector<int> factorize<int>(int);
        template std::vector<long> factorize<long>(long);
        template std::vector<long long> factorize<long long>(long long);
        template std::vector<unsigned char> factorize<unsigned char>(unsigned char);
        template std::vector<unsigned short> factorize<unsigned short>(unsigned short);
        template std::vector<unsigned int> factorize<unsigned int>(unsigned int);
        template std::vector<unsigned long> factorize<unsigned long>(unsigned long);
        template std::vector<unsigned long long> factorize<unsigned long long>(unsigned long long);
        
        /**
            * @brief Returns true if n is a prime number.
            * @param n The number to check.
            * @return True if n is a prime number, false otherwise.
            */
        bool is_prime(long long n);
        /**
            * @brief Returns true if a and b are coprime.
            * @param a The first number.
            * @param b The second number.
            * @return True if a and b are coprime, false otherwise.
            */
        bool is_coprime(long long a, long long b);

        /**
            * @brief Returns the greatest common divisor of a and b using the basic Euclidian algorithm.
            * @param a The first number.
            * @param b The second number.
            * @return The greatest common divisor of a and b.
            */
        long long greatest_common_divisor(long long a, long long b);
        /**
            * @brief Returns the greatest common divisor of a and b using the extended Euclidian algorithm.
            * @param a The first number.
            * @param b The second number.
            * @param x The Bézout coefficient x.
            * @param y The Bézout coefficient y.
            * @return The greatest common divisor of a and b.
            */
        long long greatest_common_divisor_extended(long long a, long long b, long long * x, long long * y);

        /**
            * @brief Returns the modular inverse of a modulo m using the basic Euclidian algorithm.
            * The modular inverse of a modulo m is the integer a^-1 such that a * a^-1 ≡ 1 (mod m).
            * Extended Euclidian algorithm is used to find the Bézout coefficients x and y such that a * x + m * y = 1.
            * Extended Euclidian algorithm is applicable only if a and m are coprime; i.e. gcd(a, m) = 1.
            * @param a The number.
            * @param m The modulo.
            * @return The modular inverse of a modulo m.
            * @throws std::invalid_argument if the modular inverse does not exist.
            * @see greatest_common_divisor_extended
            */
        long long modular_inverse(long long a, long long m);
        /**
            * @brief Returns the modular inverse of a modulo m using the modular inverse of a modulo m using the extended Euclidian algorithm.
            * The modular inverse of a modulo m is the integer a^-1 such that a * a^-1 ≡ 1 (mod m).
            * Extended Euclidian algorithm is used to find the Bézout coefficients x and y such that a * x + m * y = 1.
            * Extended Euclidian algorithm is applicable only if a and m are coprime; i.e. gcd(a, m) = 1.
            * @param a The number.
            * @param m The modulo.
            * @return The modular inverse of a modulo m.
            * @throws std::invalid_argument if the modular inverse does not exist.
            * @see is_coprime
            */
        long long modular_inverse_extended(long long a, long long m);
        /**
            * @brief Returns the modular inverse of a modulo m using Fermat's little theorem.
            * Fermat's little theorem states that if p is a prime number, then for any integer a, the number a^p - a is an integer multiple of p.
            * Fermat's little theorem is applicable only if m is a prime number.
            * @param a The number.
            * @param m The modulo.
            * @return The modular inverse of a modulo m.
            * @throws std::invalid_argument if the modulo is not a prime number or the modular inverse does not exist.
            * @see is_prime
            * @see is_coprime
            */
        long long modular_inverse_fermat(long long a, long long m);

        /**
            * @brief Returns the modular power of x raised to the power of y modulo m.
            * @param x The base.
            * @param y The exponent.
            * @param m The modulo.
            * @return The modular power of x raised to the power of y modulo m.
            */
        long long modular_power(long long x, long long y, long long m);

        /**
            * @brief Returns the least common multiple of a and b.
            * @param a The first number.
            * @param b The second number.
            * @return The least common multiple of a and b.
            */ 
        long long least_common_multiple(long long a, long long b);

        /**
            * @brief Returns the least common multiple of a list of numbers.
            * @param numbers The list of numbers.
            * @return The least common multiple of the list of numbers.
            */
        long long least_common_multiple(std::vector<long long> numbers, long long idx = 0);

    }
}

#endif // __METANULL_MATH_H__
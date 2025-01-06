#pragma once
#ifndef ADVENTOFCODE_MATH_H
#define ADVENTOFCODE_MATH_H

#include <stdexcept>
#include <vector>

/*
 * Resources:
 * https://www.geeksforgeeks.org/euclidean-algorithms-basic-and-extended/
 * https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
 * https://www.geeksforgeeks.org/lcm-of-given-array-elements/?ref=lbp
 * https://www.geeksforgeeks.org/c-program-to-check-prime-number/
 */

/*
    * Returns true if n is a prime number.
    * 
    * @param n The number to check.
    * @return True if n is a prime number, false otherwise.
    */
bool isPrime(long long n);
/*
    * Returns true if a and b are coprime.
    * 
    * @param a The first number.
    * @param b The second number.
    * @return True if a and b are coprime, false otherwise.
    */
bool isCoprime(long long a, long long b);

/*
    * Returns the greatest common divisor of a and b using the basic Euclidian algorithm.
    * 
    *
    * @param a The first number.
    * @param b The second number.
    * @return The greatest common divisor of a and b.
    */
long long gcdBasicEuclidian(long long a, long long b);
/*
    * Returns the greatest common divisor of a and b using the extended Euclidian algorithm.
    * 
    * @param a The first number.
    * @param b The second number.
    * @param x The Bézout coefficient x.
    * @param y The Bézout coefficient y.
    * @return The greatest common divisor of a and b.
    */
long long gcdExtendedEuclidian(long long a, long long b, long long * x, long long * y);

/*
    * Returns the modular inverse of a modulo m using the basic Euclidian algorithm.
    * The modular inverse of a modulo m is the integer a^-1 such that a * a^-1 ≡ 1 (mod m).
    * Extended Euclidian algorithm is used to find the Bézout coefficients x and y such that a * x + m * y = 1.
    * Extended Euclidian algorithm is applicable only if a and m are coprime; i.e. gcd(a, m) = 1.
    * 
    * @param a The number.
    * @param m The modulo.
    * @return The modular inverse of a modulo m.
    * @throws std::invalid_argument if the modular inverse does not exist.
    * @see gcdExtendedEuclidian
    */
long long modInverseExtendedEuclidian(long long a, long long m);
/*
    * Returns the modular inverse of a modulo m using the modular inverse of a modulo m using the extended Euclidian algorithm.
    * The modular inverse of a modulo m is the integer a^-1 such that a * a^-1 ≡ 1 (mod m).
    * Extended Euclidian algorithm is used to find the Bézout coefficients x and y such that a * x + m * y = 1.
    * Extended Euclidian algorithm is applicable only if a and m are coprime; i.e. gcd(a, m) = 1.
    * 
    * @param a The number.
    * @param m The modulo.
    * @return The modular inverse of a modulo m.
    * @throws std::invalid_argument if the modular inverse does not exist.
    * @see isCoprime
    */
long long modInverseModularExtendedEuclidian(long long a, long long m);
/*
    * Returns the modular inverse of a modulo m using Fermat's little theorem.
    * Fermat's little theorem states that if p is a prime number, then for any integer a, the number a^p - a is an integer multiple of p.
    * Fermat's little theorem is applicable only if m is a prime number.
    * 
    * @param a The number.
    * @param m The modulo.
    * @return The modular inverse of a modulo m.
    * @throws std::invalid_argument if the modulo is not a prime number or the modular inverse does not exist.
    * @see isPrime
    * @see isCoprime
    */
long long modInverseFermat(long long a, long long m);

/*
    * Returns the modular power of x raised to the power of y modulo m.
    * 
    * @param x The base.
    * @param y The exponent.
    * @param m The modulo.
    * @return The modular power of x raised to the power of y modulo m.
    */
long long modPower(long long x, long long y, long long m);

/*
    * Returns the least common multiple of a and b.
    * 
    * @param a The first number.
    * @param b The second number.
    * @return The least common multiple of a and b.
    */ 
long long lcmBasicEuclidian(long long a, long long b);

/*
    * Returns the least common multiple of a list of numbers.
    * 
    * @param numbers The list of numbers.
    * @return The least common multiple of the list of numbers.
    */
long long lcmBasicEuclidian(std::vector<long long> numbers, long long idx = 0);

#endif // ADVENTOFCODE_MATH_H
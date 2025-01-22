#include "Math.h"

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
bool isPrime(long long n) {
    if(n <= 1) {
        return false;
    }
    if(n <= 3) {
        return true;
    }
    if(n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    for(long long i = 5; i * i <= n; i += 6) {
        if(n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

/*
    * Returns true if a and b are coprime.
    * 
    * @param a The first number.
    * @param b The second number.
    * @return True if a and b are coprime, false otherwise.
    */
bool isCoprime(long long a, long long b) {
    return gcdBasicEuclidian(a, b) == 1;
}

/*
    * Returns the greatest common divisor of a and b using the basic Euclidian algorithm.
    * 
    * @param a The first number.
    * @param b The second number.
    * @return The greatest common divisor of a and b.
    */
long long gcdBasicEuclidian(long long a, long long b) {
    if(a == 0) {
        return b;
    }
    return gcdBasicEuclidian(b % a, a);
}
/*
    * Returns the greatest common divisor of a and b using the extended Euclidian algorithm.
    * Extended Euclidean algorithm also finds integer coefficients x and y such that: ax + by = gcd(a, b) 
    * 
    * @param a The first number.
    * @param b The second number.
    * @param x The Bézout coefficient x.
    * @param y The Bézout coefficient y.
    * @return The greatest common divisor of a and b.
    */
long long gcdExtendedEuclidian(long long a, long long b, long long * x, long long * y) {
    if(a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    long long x1, y1;
    long long gcd = gcdExtendedEuclidian(b % a, a, &x1, &y1);
    // Update x and y using results of recursive call
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

/*
    * Returns the modular inverse of a modulo m using the basic Euclidian algorithm.
    * The modular inverse of a modulo m is the integer a^-1 such that a * a^-1 ≡ 1 (mod m).
    * Extended Euclidian algorithm is used to find the Bézout coefficients x and y such that a * x + m * y = 1.
    * Extended Euclidian algorithm is applicable only if a and m are coprime.
    * 
    * @param a The number.
    * @param m The modulo.
    * @return The modular inverse of a modulo m.
    */
long long modInverseExtendedEuclidian(long long a, long long m) {
    long long x, y;
    long long gcd = gcdExtendedEuclidian(a, m, &x, &y);
    if(gcd != 1) {
        throw std::invalid_argument("The modular inverse does not exist.");
    }
    return (x % m + m) % m;
}

/*
    * Returns the modular inverse of a modulo m using the modular inverse of a modulo m using the extended Euclidian algorithm.
    * The modular inverse of a modulo m is the integer a^-1 such that a * a^-1 ≡ 1 (mod m).
    * Extended Euclidian algorithm is used to find the Bézout coefficients x and y such that a * x + m * y = 1.
    * Extended Euclidian algorithm is applicable only if a and m are coprime; i.e. gcd(a, m) = 1.
    * 
    * @param a The number.
    * @param m The modulo.
    * @return The modular inverse of a modulo m.
    */
long long modInverseModularExtendedEuclidian(long long a, long long m) {
    if(!isCoprime(a, m)) {
        throw std::invalid_argument("The modular inverse does not exist.");
    }
    long long m0 = m;
    long long y = 0, x = 1;

    if(m == 1) {
        return 0;
    }
    while(a > 1) {
        // q is quotient
        long long q = a / m;
        long long t = m;

        // m is remainder now, process same as Euclid's algorithm
        m = a % m, a = t;
        t = y;

        // Update y and x
        y = x - q * y;
        x = t;
    }

    // Make x positive
    if(x < 0) {
        x += m0;
    }
    return x;
}

/*
    * Returns the modular inverse of a modulo m using Fermat's little theorem.
    * Fermat's little theorem states that if p is a prime number, then for any integer a, the number a^p - a is an integer multiple of p.
    * Fermat's little theorem is applicable only if m is a prime number.
    * 
    * @param a The number.
    * @param m The modulo.
    * @return The modular inverse of a modulo m.
    */
long long modInverseFermat(long long a, long long m) {
    if(!isPrime(m)) {
        throw std::invalid_argument("The modulo is not a prime number.");
    }
    if(!isCoprime(a, m)) {
        throw std::invalid_argument("The modular inverse does not exist.");
    }
    return modPower(a, m - 2, m);    
}

/*
    * Returns the modular power of x raised to the power of y modulo m.
    * 
    * @param x The base.
    * @param y The exponent.
    * @param m The modulo.
    * @return The modular power of x raised to the power of y modulo m.
    */
long long modPower(long long x, long long y, long long m) {
    if(y == 0) {
        return 1;
    }
    long long p = modPower(x, y / 2, m) % m;
    p = (p * p) % m;
    return (y % 2 == 0) ? p : (x * p) % m;
}

/*
    * Returns the least common multiple of a and b.
    * lcm(a,b) = (a*b/gcd(a,b))
    * 
    * @param a The first number.
    * @param b The second number.
    * @return The least common multiple of a and b.
    */ 
long long lcmBasicEuclidian(long long a, long long b) {
    return (a * b) / gcdBasicEuclidian(a, b);
}

/*
    * Returns the least common multiple of a list of numbers.
    * lcm(a,b) = (a*b/gcd(a,b))
    * 
    * @param numbers The list of numbers.
    * @return The least common multiple of the list of numbers.
    */
long long lcmBasicEuclidian(std::vector<long long> numbers, long long idx) {
    if(idx == numbers.size() - 1) {
        return numbers[idx];
    }
    return lcmBasicEuclidian(numbers[idx], lcmBasicEuclidian(numbers, idx + 1));
}
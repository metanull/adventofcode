#include "math.h"

namespace metanull {
    namespace math {
        inline uint64_t count_digits(uint64_t n) {
            if(n == 0) {
                return 1;
            }
            return (uint64_t)(std::floor(std::log10(n)) + 1);
        }
        inline bool count_digits_is_even(uint64_t n) {
            return (count_digits(n) % 2 == 0) ? true : false;
        }

        template <typename T>
        std::vector<T> factorize(T num) {   // works great for smooth numbers
            std::vector<T> v;
            if (num < 4) { v.push_back(num); return v; }
            T d {2};
            while (num >= d * d) {
                while (num % d == 0) {  // remove all repeats of this divisor
                    v.push_back(d);
                    num /= d;
                }
                ++d;
                if (d * d > num && num > 1) { v.push_back(num); return v; }
            }
            return v;
        }

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

        bool isCoprime(long long a, long long b) {
            return gcdBasicEuclidian(a, b) == 1;
        }

        long long gcdBasicEuclidian(long long a, long long b) {
            if(a == 0) {
                return b;
            }
            return gcdBasicEuclidian(b % a, a);
        }

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

        long long modInverseExtendedEuclidian(long long a, long long m) {
            long long x, y;
            long long gcd = gcdExtendedEuclidian(a, m, &x, &y);
            if(gcd != 1) {
                throw std::invalid_argument("The modular inverse does not exist.");
            }
            return (x % m + m) % m;
        }

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

        long long modInverseFermat(long long a, long long m) {
            if(!isPrime(m)) {
                throw std::invalid_argument("The modulo is not a prime number.");
            }
            if(!isCoprime(a, m)) {
                throw std::invalid_argument("The modular inverse does not exist.");
            }
            return modPower(a, m - 2, m);    
        }

        long long modPower(long long x, long long y, long long m) {
            if(y == 0) {
                return 1;
            }
            long long p = modPower(x, y / 2, m) % m;
            p = (p * p) % m;
            return (y % 2 == 0) ? p : (x * p) % m;
        }

        long long lcmBasicEuclidian(long long a, long long b) {
            return (a * b) / gcdBasicEuclidian(a, b);
        }

        long long lcmBasicEuclidian(std::vector<long long> numbers, long long idx) {
            if(idx == numbers.size() - 1) {
                return numbers[idx];
            }
            return lcmBasicEuclidian(numbers[idx], lcmBasicEuclidian(numbers, idx + 1));
        }
    } // namespace math
} // namespace metanull
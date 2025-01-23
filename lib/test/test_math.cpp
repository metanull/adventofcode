#include "../math.h"

#ifndef ARGV2_EQUALS
    #define ARGV2_EQUALS(x) (argc == 2 && 0 == strcmp(argv[1], x))
#endif

#include <functional>
#include <map>

int main(int argc, char ** argv, char ** envp) {
    if(argc != 2) {
        std::cerr << "Usage: test_charmap <test>" << std::endl;
        return 1;
    }

    std::string test_name = argv[1];
    std::map<std::string, std::function<bool()>> tests = {
        {"count_digits-zero", []() -> bool {
            std::cerr << "Expected result: 1 - for expression count_digits(0)" << std::endl;
            
            auto n = metanull::math::count_digits(0);
            return n == 1;
        }},
        {"count_digits-3", []() -> bool {
            std::cerr << "Expected result: 3 - for expression count_digits(123)" << std::endl;
            
            auto n = metanull::math::count_digits(123);
            return n == 3;
        }},
        {"count_digits-10", []() -> bool {
            std::cerr << "Expected result: 10 - for expression count_digits(1234567890)" << std::endl;
            
            auto n = metanull::math::count_digits(1234567890);
            return n == 10;
        }},
        {"count_digits-LLONG_MAX", []() -> bool {
            std::cerr << "Expected result: 19 - for expression count_digits(LLONG_MAX)" << std::endl;
            
            auto n = metanull::math::count_digits(LLONG_MAX);
            return n == 19;
        }},
        {"count_digits_is_even-0", []() -> bool {
            std::cerr << "Expected result: false - for expression count_digits_is_even(0)" << std::endl;
            
            auto result = metanull::math::count_digits_is_even(0);
            return result == false;
        }},
        {"count_digits_is_even-123", []() -> bool {
            std::cerr << "Expected result: false - for expression count_digits_is_even(123)" << std::endl;
            
            auto result = metanull::math::count_digits_is_even(123);
            return result == false;
        }},
        {"count_digits_is_even-1234", []() -> bool {
            std::cerr << "Expected result: true - for expression count_digits_is_even(1234)" << std::endl;
            
            auto result = metanull::math::count_digits_is_even(1234);
            return result == true;
        }},
        {"count_digits_is_even-1234567890", []() -> bool {
            std::cerr << "Expected result: true - for expression count_digits_is_even(1234567890)" << std::endl;
            
            auto result = metanull::math::count_digits_is_even(1234567890);
            return result == true;
        }},
        {"count_digits_is_even-LLONG_MAX", []() -> bool {
            std::cerr << "Expected result: true - for expression count_digits_is_even(LLONG_MAX)" << std::endl;
            
            auto result = metanull::math::count_digits_is_even(LLONG_MAX);
            return result == false;
        }},
        {"factorize-1", []() -> bool {
            std::cerr << "Expected result: {1} - for expression factorize(1)" << std::endl;
            
            auto result = metanull::math::factorize(1);
            return result == std::vector<int>{1};
        }},
        {"factorize-2", []() -> bool {
            std::cerr << "Expected result: {2} - for expression factorize(2)" << std::endl;
            
            auto result = metanull::math::factorize(2);
            return result == std::vector<int>{2};
        }},
        {"factorize-3", []() -> bool {
            std::cerr << "Expected result: {3} - for expression factorize(3)" << std::endl;
            
            auto result = metanull::math::factorize(3);
            return result == std::vector<int>{3};
        }},
        {"factorize-4", []() -> bool {
            std::cerr << "Expected result: {2, 2} - for expression factorize(4)" << std::endl;
            
            auto result = metanull::math::factorize(4);
            return result == std::vector<int>{2, 2};
        }},
        {"factorize-6", []() -> bool {
            std::cerr << "Expected result: {2, 3} - for expression factorize(6)" << std::endl;
            
            auto result = metanull::math::factorize(6);
            return result == std::vector<int>{2, 3};
        }},
        {"factorize-12", []() -> bool {
            std::cerr << "Expected result: {2, 2, 3} - for expression factorize(12)" << std::endl;
            
            auto result = metanull::math::factorize(12);
            return result == std::vector<int>{2, 2, 3};
        }},
        {"factorize-100", []() -> bool {
            std::cerr << "Expected result: {2, 2, 5, 5} - for expression factorize(100)" << std::endl;
            
            auto result = metanull::math::factorize(100);
            return result == std::vector<int>{2, 2, 5, 5};
        }},
        {"is_prime-2", []() -> bool {
            std::cerr << "Expected result: true - for expression is_prime(2)" << std::endl;
            
            auto result = metanull::math::is_prime(2);
            return result == true;
        }},
        {"is_prime-4", []() -> bool {
            std::cerr << "Expected result: false - for expression is_prime(4)" << std::endl;
            
            auto result = metanull::math::is_prime(4);
            return result == false;
        }},
        {"is_coprime-5-9", []() -> bool {
            std::cerr << "Expected result: true - for expression is_coprime(5, 9)" << std::endl;
            
            auto result = metanull::math::is_coprime(5, 9);
            return result == true;
        }},
        {"is_coprime-6-9", []() -> bool {
            std::cerr << "Expected result: false - for expression is_coprime(6, 9)" << std::endl;
            
            auto result = metanull::math::is_coprime(6, 9);
            return result == false;
        }},
        {"greatest_common_divisor-48-18", []() -> bool {
            std::cerr << "Expected result: 6 - for expression greatest_common_divisor(48, 18)" << std::endl;
            
            auto result = metanull::math::greatest_common_divisor(48, 18);
            return result == 6;
        }},
        {"greatest_common_divisor-101-103", []() -> bool {
            std::cerr << "Expected result: 1 - for expression greatest_common_divisor(101, 103)" << std::endl;
            
            auto result = metanull::math::greatest_common_divisor(101, 103);
            return result == 1;
        }},
        {"greatest_common_divisor_extended-35-15", []() -> bool {
            std::cerr << "Expected result: gcd=5, x=1, y=-2 - for expression greatest_common_divisor_extended(35, 15, &x, &y)" << std::endl;
            
            long long x, y;
            auto result = metanull::math::greatest_common_divisor_extended(35, 15, &x, &y);
            return result == 5 && x == 1 && y == -2;
        }},
        {"modular_inverse-3-11", []() -> bool {
            std::cerr << "Expected result: 4 - for expression modular_inverse(3, 11)" << std::endl;
            
            auto result = metanull::math::modular_inverse(3, 11);
            return result == 4;
        }},
        {"modular_inverse_extended-3-11", []() -> bool {
            std::cerr << "Expected result: 4 - for expression modular_inverse_extended(3, 11)" << std::endl;
            
            auto result = metanull::math::modular_inverse_extended(3, 11);
            return result == 4;
        }},
        {"modular_inverse_fermat-3-11", []() -> bool {
            std::cerr << "Expected result: 4 - for expression modular_inverse_fermat(3, 11)" << std::endl;
            
            auto result = metanull::math::modular_inverse_fermat(3, 11);
            return result == 4;
        }},
        {"modular_power-2-10-1000", []() -> bool {
            std::cerr << "Expected result: 24 - for expression modular_power(2, 10, 1000)" << std::endl;
            
            auto result = metanull::math::modular_power(2, 10, 1000);
            return result == 24;
        }},
        {"least_common_multiple-12-15", []() -> bool {
            std::cerr << "Expected result: 60 - for expression least_common_multiple(12, 15)" << std::endl;
            
            auto result = metanull::math::least_common_multiple(12, 15);
            return result == 60;
        }},
        {"least_common_multiple-vector", []() -> bool {
            std::cerr << "Expected result: 60 - for expression least_common_multiple({3, 4, 5})" << std::endl;
            
            auto result = metanull::math::least_common_multiple(std::vector<long long>{3, 4, 5});
            return result == 60;
        }}
    };

    auto it = tests.find(test_name);
    if (it != tests.end()) {
        if(!it->second()) {
            std::cerr << "Test failed: " << test_name << std::endl;
            return 1;
        }
        return 0;
    } else {
        std::cerr << "Unknown test: " << test_name << ". Options are: " << std::endl;
        for (const auto& test : tests) {
            std::cout << "- " << test.first << std::endl;
        }
        return 1;
    }
}

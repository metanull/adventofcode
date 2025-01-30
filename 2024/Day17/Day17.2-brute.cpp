#include <string>
#include <iostream>
#include <iomanip>

const char *banner = "AdventOfCode 2024 Day 17!";

size_t pgm(uint64_t a, uint8_t * r, size_t rs) {
    size_t i = 0;
    while(a > 0 && i < rs) {
        r[i++] = (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7);
        a = a>>3;
    }
    return i;
}
uint64_t brute(uint64_t s, uint8_t * r, size_t rs) {
    size_t i = 0;
    uint64_t a = 0;
    uint64_t min = s;
    uint64_t max = pow(2,3*rs);
    for(auto x = min; x < max; x ++) {
        i = 0;
        a = x;
        while(a > 0 && i < rs) {
            if(r[i++] != (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7)) {
                break;
            }
            a = a>>3;
        }
        if(a == 0 && i == rs) {
            return x;
        }
    }
    return 0;
}

int main(int argc, char **argv, char **envp)
{
    size_t banner_width = 80;
    std::cout << "\033[7;34;1m" << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << std::setw((banner_width - strlen(banner)) / 2) << '#'
              << banner
              << std::setw(((banner_width - strlen(banner)) / 2) + ((banner_width - strlen(banner)) % 2)) << '#' << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << "\033[0m" << std::endl
              << std::endl;
    std::cout << "Brute forcing..." << std::endl;

    /*
    uint64_t a = 12;
    uint8_t r[32] = {0};
    size_t s = pgm(a,r,sizeof(r));
    std::cout << a << ": ";
    for(auto i = 0; i < s; i ++) {
        std::cout << (int)r[i];
    }
    std::cout << std::endl;

    // Reverse (brute) operation
    std::cout << "Brute: " << brute(0,r,s);
    */

    uint8_t r[] = {2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0};
    auto b = brute((uint64_t)pow(2,3*(sizeof(r)-1)-1),r,sizeof(r));
    std::cout << "Brute: " << b << std::endl;

    return 0;
}
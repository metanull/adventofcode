#include <string>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <atomic>

const char *banner = "AdventOfCode 2024 Day 17!";

size_t pgm(uint64_t a, uint8_t * r, size_t rs) {
    size_t i = 0;
    while(a > 0 && i < rs) {
        r[i++] = (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7);
        a = a>>3;
    }
    return i;
}
uint64_t brute(uint64_t min, uint64_t max, uint8_t * r, size_t rs, size_t n_thread, size_t thread_index, std::atomic<uint64_t> &found_solution) {
    size_t i = 0;
    uint64_t a = 0;
    for(auto x = min; x < max; x += n_thread) {
        if (found_solution != UINT64_MAX && x > found_solution) {
            return 0;
        }
        i = 0;
        a = x + thread_index;
        while((rs == 1 || a > 0) && i < rs) {
            auto _t = (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7);
            if(r[i++] != (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7)) {
                break;
            }
            a = a>>3;
            if(a == 0 && i == rs) {
                if (x < found_solution) {
                    found_solution = x;
                }
                return x;
            }
        }
    }
    return UINT64_MAX;
}

int main(int argc, char **argv, char **envp)
{
    std::atomic<uint64_t> found_solution(UINT64_MAX);

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
    // Forward operation
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
    // uint8_t r[] = {2,1,7,4,1,5,0};
    uint64_t min = sizeof(r) == 1 ? 0 : (uint64_t)pow(2,3*(sizeof(r)-1));
    uint64_t max = (uint64_t)pow(2,3*(sizeof(r)));

    // std::cout << "Number of threads supported by hardware: " << std::max((size_t)1,(size_t)std::thread::hardware_concurrency()) << std::endl;
    size_t n_threads = 4;
    std::vector<std::thread> threads;
    uint64_t results[4] = {0};

    for(size_t i = 0; i < n_threads; i++) {
        threads.emplace_back([&, i, min, max]() {
            results[i] = brute(min, max, r, sizeof(r), n_threads, i, found_solution);
        });
    }

    for(auto &t : threads) {
        t.join();
    }

    std::cout << "Brute: " << results[0] << std::endl;
    std::cout << "Brute: " << results[1] << std::endl;
    std::cout << "Brute: " << results[2] << std::endl;
    std::cout << "Brute: " << results[3] << std::endl;

    // uint64_t b = brute(min,max,r,sizeof(r),1,0);
    // std::cout << "Brute: " << b << std::endl;

    return 0;
}
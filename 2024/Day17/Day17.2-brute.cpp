#include <string>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <atomic>
#include <sstream>

const char *banner = "AdventOfCode 2024 Day 17!";

#define N_THREADS 32

size_t pgm(uint64_t a, uint8_t * r, size_t rs) {
    size_t i = 0;
    while(a > 0 && i < rs) {
        r[i++] = (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7);
        a = a>>3;
    }
    return i;
}
uint64_t brute(uint64_t min, uint64_t max, uint8_t * r, size_t rs, size_t n_thread, size_t thread_index, std::atomic<uint64_t> &found_solution, std::atomic<uint64_t> &counters) {
    size_t i = 0;
    uint64_t a = 0;
    for(auto x = min; x < max; x += n_thread) {
        counters = x;
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
    std::atomic<uint64_t> found_solution(UINT64_MAX);
    std::atomic<uint64_t> counters[4] = {};
    std::vector<std::thread> threads;
    uint64_t results[N_THREADS] = {0};

    for(size_t i = 0; i < N_THREADS; i++) {
        threads.emplace_back([&, i, min, max]() {
            results[i] = brute(min, max, r, sizeof(r), N_THREADS, i, found_solution, counters[i]);
        });
    }

    std::atomic<bool> done(false);
    threads.emplace_back([&]() {
        std::locale frenchBelgium("fr_BE.UTF-8");
        long long dsec = 60;
        auto start = std::chrono::steady_clock::now();
        while (!done) {
            uint64_t local_counters[N_THREADS] = {0};
            for(auto _i = 0; _i < N_THREADS; _i ++) {
                local_counters[_i] = counters[_i];
            }
            std::this_thread::sleep_for(std::chrono::seconds(dsec));
            std::stringstream ss;
            ss.imbue(frenchBelgium);
            uint64_t total_counters = 0;
            ss << std::setw(6) << std::right << std::setfill(' ') << std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() << " m|";
            for (size_t i = 0; i < N_THREADS; i++) {
                total_counters += counters[i] - local_counters[i];
                // ss << "[" << i << "]" << std::setprecision(5) << ((double)counters[i]/ 1000000000000) << "T" << "|" << std::fixed << std::setprecision(2) << (double)((counters[i] - local_counters[i]) / 1000000000) << "Gpm" << "|";
            }
            size_t i = 0;
            ss << " [" << i << "]" << std::setw(20) << counters[i] << "|" << std::fixed << std::setw(6) << std::setprecision(3) << ((double)(counters[i] - local_counters[i]) / 1000000000) << " Gpm" << "|";
            ss << " (...)|";
            i = N_THREADS - 1;
            ss << " [" << i << "]" << std::setw(20) << counters[i] << "|" << std::fixed << std::setw(6) << std::setprecision(3) << ((double)(counters[i] - local_counters[i]) / 1000000000) << " Gpm" << "|";
            ss << " SPEED:" << std::fixed << std::setprecision(3) << ((double)total_counters / 1000000000) << " G/m";
            std::cout << ss.str() << std::endl;
        }
    });

    for(auto &t : threads) {
        t.join();
    }
    done = true;

    std::cout << "Brute: " << results[0] << std::endl;
    std::cout << "Brute: " << results[1] << std::endl;
    std::cout << "Brute: " << results[2] << std::endl;
    std::cout << "Brute: " << results[3] << std::endl;

    // uint64_t b = brute(min,max,r,sizeof(r),1,0);
    // std::cout << "Brute: " << b << std::endl;

    return 0;
}
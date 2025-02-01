#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>

#define NOMINMAX
#include <windows.h>

const char *banner = "AdventOfCode 2024 Day 17!";

#define N_THREADS 32
#define NEEDLE 2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0
#define START 0ULL

/**
 * @brief The 'Program' from the input I have received for "AdventOfCode 2024 17.1": 
 * @param a The input value (the inital value of the A register)
 * @param r The result "output" array
 * @param rs The size of the result array
 * @return The number of elements in the result array (note: if the result array is smaller than the number of elements, the result will be truncated)
 * // Program: 2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0
 * //        0)  2, 4,    BST: B = B & 7
 * //        1)  1, 2,    BXL: B = B ^ 2
 * //        2)  7, 5,    CDV: C = A >>B
 * //        3)  4, 5,    BXC: B = B ^ C
 * //        4)  1, 3,    BXL: B = B ^ 3
 * //        5)  5, 5,    OUT: B & 7    
 * //        6)  0, 3,    ADV: A = A >>3
 * //        7)  3, 0,    JNZ: A != 0 ? then JUMP to 0. else EXIT
 */
size_t pgm(uint64_t a, uint8_t * r, size_t rs);

/**
 * @brief Inverse of 'Program' (brute force): it takes the result output array and tries to find the input value that would produce this output.
 * @see pgm
 * @param min The minimum value to start the brute force
 * @param r The result array to compare/brute force
 * @param rs The size of the result array
 * @return The input value that would produce the result array, or UINT64_MAX if no solution is found.
 */

uint64_t brute(uint64_t min, uint8_t * r, size_t rs);
/*
 * @brief Inverse of 'Program' (brute force) with multiple threads: it takes the result output array and tries to find the input value that would produce this output.
 * @see pgm
 * @param min The minimum value to start the brute force
 * @param max The maximum value to stop the brute force
 * @param r The result array to compare/brute force
 * @param rs The size of the result array
 * @param n_thread The number of threads in use (to split the work)
 * @param thread_index The index of the current thread (to split the work)
 * @param found_solution The atomic variable to store the found solution, and let the workers stop if they are not able to provide a better solution than the one found.
 * @param counter The atomic variable to store the current counter of the worker. It is used by the feedback thread to report on the progress
 * @return The input value that would produce the result array, or UINT64_MAX if no solution is found.
 */

uint64_t brute_thread(uint64_t min, uint64_t max, uint8_t * r, size_t rs, size_t n_thread, size_t thread_index, std::atomic<uint64_t> &found_solution, std::atomic<uint64_t> &counter);
/**
 * @brief The feedback thread that reports on the progress of the brute force operation.
 * @param done The atomic variable to signal the thread to stop.
 * @param counters The list of atomic variables containing the "current counter" of each worker.
 * @param found_solution The atomic variable containing the found solution (if any)
 * @see brute_thread
 */

void monitor_thread(uint64_t min, uint64_t max, std::atomic<bool> &done, std::vector<std::unique_ptr<std::atomic<uint64_t>>> &counters, std::atomic<uint64_t> &found_solution);

/**
 * A mutex to protect the output stream
 */
std::mutex cout_mutex;

/**
 * @brief The main function
 * @param argc The number of arguments
 * @param argv The list of arguments
 * @param envp The list of environment variables
 * @return The exit code
 */
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
    
    std::cout << "(INFO) Number of threads supported by hardware: " << std::max((size_t)1,(size_t)std::thread::hardware_concurrency()) << std::endl;
    std::cout << "Brute forcing..." << std::endl;

    /*
    // // Forward operation
    // uint64_t a = 12;
    // uint8_t r[32] = {0};
    // size_t s = pgm(a,r,sizeof(r));
    // std::cout << a << ": ";
    // for(auto i = 0; i < s; i ++) {
    //     std::cout << (int)r[i];
    // }
    // std::cout << std::endl;

    // // Reverse (brute) operation (singel thread)
    // auto b = brute(0,r,s);
    // std::cout << "Brute: " << b << std::endl;
    */

    /*// uint8_t r[] = {2,1,7,4,1,5,0};   // This is a fast test with an answer of 0
    // uint64_t min = std::max((uint64_t)1, (uint64_t)pow(2,3*(sizeof(r)-1)));
    // uint64_t min = 4000000000000ULL;
    uint8_t r[] = {1,3,1,6,6,3};
    uint64_t min = 123450ULL;
    uint64_t max = (uint64_t)pow(2,3*(sizeof(r)));
    */

    #ifndef NEEDLE
        uint8_t r[] = {2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0};
    #else
        uint8_t r[] = {NEEDLE};
    #endif
    #ifndef START
        uint64_t min = std::max((uint64_t)1, (uint64_t)pow(2,3*(sizeof(r)-1)));
    #else
        uint64_t min = START;
    #endif
    #ifndef END
        uint64_t max = (uint64_t)pow(2,3*(sizeof(r)));
    #else
        uint64_t max = END;
    #endif
        
    // size_t n_workers = std::max((size_t)1, std::thread::hardware_concurrency());
    static const size_t n_workers = N_THREADS;          // The number of workers
    std::atomic<uint64_t> found_solution(UINT64_MAX);   // Atomic variable to store the found solution, and let the workers stop if they are not able to provide a better solution than the one found.
    std::vector<std::thread> threads;                   // The list of threads.
    std::vector<std::unique_ptr<std::atomic<uint64_t>>> counters;        // List of Atomic variables to store the "current counter" of each worker.
    counters.reserve(n_workers);
    std::vector<std::unique_ptr<std::atomic<uint64_t>>> results;                      // List of results of each worker.    
    results.reserve(n_workers);
    for(size_t i = 0; i < n_workers; i++) {
        // Initialize the counter for the worker
        counters.emplace_back(std::make_unique<std::atomic<uint64_t>>(0));
        // Initialize the results for the worker
        results.emplace_back(std::make_unique<std::atomic<uint64_t>>(UINT64_MAX));
    }

    // Start the Worker threads
    for(size_t i = 0; i < n_workers; i++) {
        threads.emplace_back([&, i, min, max]() {
            auto res = brute_thread(min, max, r, sizeof(r), n_workers, i, found_solution, *counters[i]);
            *results[i] = res;
        });
    }

    // Start the Feedback thread
    std::atomic<bool> done(false);
    threads.emplace_back([&]() {
        monitor_thread(min, max, done, counters, found_solution);
    });
    
    // Wait for the worker threads to finish
    for(auto it = threads.begin(); it != threads.end() - 1; it++) {
        it->join();
    }
    // Signal the feedback thread to stop
    done = true;
    threads.back().join();

    // Print the results
    for(auto it = 0; it < results.size(); it++) {
        if(*results[it] != UINT64_MAX && *results[it] != 0) {
            std::cout << "Thread #" << it << " found: " << *results[it] << " - verification: ";
            pgm(*results[it], r, sizeof(r));
            for(auto c : r) {
                std::cout << (int)c << " ";
            }
            std::cout << std::endl;
        }
    }


    return 1;
}

size_t pgm(uint64_t a, uint8_t * r, size_t rs) {
    size_t i = 0;
    while(a > 0 && i < rs) {
        r[i++] = (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7);
        a = a>>3;
    }
    return i;
}

uint64_t brute(uint64_t min, uint8_t * r, size_t rs) {
    min = std::max((uint64_t)1, min);
    uint64_t max = (uint64_t)pow(2,(3*rs));
    size_t i = 0;
    uint64_t a = 0;
    for(auto x = min; x < UINT64_MAX; x++) {
        // Run the forward operation, interrupting as soon as the result is different from the expected result.
        // Set the result index to 0 (we will increment it as we find the correct values)
        i = 0;
        // Set the A register to the current iteration's value.
        a = x;
        while((rs == 1 || a > 0) && i < rs) {
            if(r[i++] != (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7)) {
                // If the result is different from the expected result, we break the loop; so that we can try with the next value in the seqence.
                break;
            }
            a = a>>3;
            if(a == 0 && i == rs) {
                // If we found a solution, return it. As we are looking for the minimum value, we can return the first solution found.
                return x;
            }
        }
    }
    // If we did not find a solution, we return UINT64_MAX.
    return UINT64_MAX;
}

uint64_t brute_thread(uint64_t min, uint64_t max, uint8_t * r, size_t rs, size_t n_thread, size_t thread_index, std::atomic<uint64_t> &found_solution, std::atomic<uint64_t> &counter) {

    // Set the thread priority and print some startup information
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        /*
        std::cout << "Thread " << std::setw(3) << thread_index << " started. " 
            << " [handles:" << ((max - min)/n_thread) << " tests]"
            << " [range:" << min+thread_index << " - " << max+thread_index << "]"
            << std::endl;
        */

        HANDLE thread_handle = GetCurrentThread();
        if(!SetThreadPriority(thread_handle, THREAD_PRIORITY_BELOW_NORMAL)) {
            std::cerr << "Failed to set thread priority" << std::endl;
        }
    }

    // Brute force the solution
    size_t i = 0;
    uint64_t a = 0;
    for(auto x = min; x < max; x += n_thread) { // We increment by the number of threads to split the work between the threads.
        // Update the counter for the feedback thread
        counter = x;
        // Check if another thread has found solution. If our current solution is greater than the found solution, we can stop.
        if (found_solution != UINT64_MAX && x > found_solution) {
            return 0;
        }
        // Run the forward operation, interrupting as soon as the result is different from the expected result.
        // Set the result index to 0 (we will increment it as we find the correct values)
        i = 0;
        // Set the A register to the current iteration's value.
        a = x + thread_index;   // We add the thread index to the current value to split the work between the threads.
        while((rs == 1 || a > 0) && i < rs) {
            if(r[i++] != (uint8_t)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7)) {
                // If the result is different from the expected result, we break the loop; so that we can try with the next value in the seqence.
                break;
            }
            // Update the A register
            a = a>>3;

            // Test if we have found one solution
            if(a == 0 && i == rs) {

                // DEBUG: Print the bytes of the result for visual verification
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "\033[31;7;1mThread " << std::setw(3) << thread_index 
                        << " found a solution for " << x
                        << " (" << i << " characters matched)\033[0m]" << std::endl;


                    // DEBUG
                    #ifdef DEBUG
                        std::cerr << "DEBUG" << std::endl;
                        std::cerr << "R:";
                        for(auto j = 0; j < rs; j++) {
                            std::cout << (int)r[j] << ",";
                        }
                        std::cerr << " - SIZE: " << rs << std::endl;
                        a = x + thread_index;
                        for(auto t = 0; t < rs; t++) {
                            
                            std::cerr << "A: " << a << ", R[" << t << "]: " << (int)r[t] << " == " << (int)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7) << " ? " << ((int)r[t] == (int)(((((a & 7) ^2) ^ (a >> ((a & 7) ^2))) ^ 3) & 7)) << std::endl;
                            a = a>>3;
                        }
                    #endif
                }

                // Return the Solution
                bool exchange_success = false;
                do {
                    // If our solution is better than a previously found solution, we update the found solution.
                    uint64_t expected = found_solution;
                    uint64_t desired = x;
                    if(desired < expected) {
                        exchange_success = found_solution.compare_exchange_strong(expected, desired);   // We use compare_exchange_strong to avoid race conditions.
                    } else {
                        exchange_success = true;
                    }
                } while(!exchange_success);
                return x + thread_index;
            }
        }
    }
    // If we did not find a solution, we return UINT64_MAX.
    return UINT64_MAX;
}

void monitor_thread(uint64_t min, uint64_t max, std::atomic<bool> &done, std::vector<std::unique_ptr<std::atomic<uint64_t>>> &counters, std::atomic<uint64_t> &found_solution) {
    
    std::vector<uint64_t> prev_counters, next_counters, processed_counters, prev_processed_counters;
    std::vector<long double> rpm_counters;
    std::vector<float> percent_counters;
    prev_counters.resize(counters.size());
    next_counters.resize(counters.size());
    processed_counters.resize(counters.size());
    prev_processed_counters.resize(counters.size());
    percent_counters.resize(counters.size());
    rpm_counters.resize(counters.size());
    for (size_t i = 0; i < counters.size(); ++i) {
        prev_counters[i] = *counters[i];
        next_counters[i] = 0;
        processed_counters[i] = 0;
        prev_processed_counters[i] = 0;
        percent_counters[i] = 0.0f;
        rpm_counters[i] = 0.0;
    }

    std::stringstream ss;
    std::locale frenchBelgium("fr_BE.UTF-8");
    ss.imbue(frenchBelgium);
    ss << std::setfill(' ') << std::right << std::fixed << std::setprecision(3);

    long long dsec = 15;
    auto start = std::chrono::steady_clock::now();
    while (!done) {
        // Get the current counters
        for (size_t i = 0; i < counters.size(); ++i) {
            prev_counters[i] = *counters[i];
        }

        // Sleep for a while
        // std::this_thread::sleep_for(std::chrono::seconds(dsec));
        auto sleep_seconds = dsec;
        for (size_t i = 0; i < sleep_seconds; ++i) {
            if (done) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        // When wakingup, get and display data bout elapsed period
        for (size_t i = 0; i < counters.size(); ++i) {
            next_counters[i] = *counters[i];

            // In the event where the counter was not initialized yet when the monitor started, uses the current value instead.
            if(prev_counters[i] == 0) {
                prev_counters[i] = next_counters[i];
            }

            // Calculate the performances of the thread
            processed_counters[i] = (next_counters[i] - (min + i)) / counters.size();
            percent_counters[i] = (float)((long double)(processed_counters[i]) / (long double)(max - min)) * 100.0f;
            rpm_counters[i] = (long double)(processed_counters[i] - prev_processed_counters[i]) / (long double)(dsec * 1000000);
        }
        // Calculate the global performances
        uint64_t total_prev_processed = std::accumulate(prev_processed_counters.begin(), prev_processed_counters.end(), 0ULL);
        uint64_t total_processed = std::accumulate(processed_counters.begin(), processed_counters.end(), 0ULL);
        float total_percent = (float)((long double)(total_processed) / (long double)(max - min)) * 100.0f;
        long double total_rpm = (long double)(total_processed - total_prev_processed) / (long double)(dsec * 1000000);
        uint64_t least_advanced_counter = *std::min_element(next_counters.begin(), next_counters.end());
        uint64_t most_advanced_counter = *std::max_element(next_counters.begin(), next_counters.end());
        prev_processed_counters = processed_counters;
        
        auto min_max = std::minmax_element(processed_counters.begin(), processed_counters.end());
        size_t min_index = std::distance(processed_counters.begin(), min_max.first);
        size_t max_index = std::distance(processed_counters.begin(), min_max.second);
        
        ss << std::setw(4) << std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() << " min.";
        for (auto i : {min_index, max_index}) {
            ss << "[#"<< std::setw(2) << i << " "
               << (i == min_index ? "(worst):\033[33m" : "(best):\033[32m")
               << std::setw(6) << percent_counters[i] << " %, "
               << std::setw(6) << rpm_counters[i] << " M.rpm"
               << "\033[0m"
               << "]";
        }
        ss << "[Global"
           << ": \033[35m" << std::setw(6) << total_percent << " %\033[0m"
           << ", \033[36m" << std::setw(6) << total_rpm << " M.rpm\033[0m"
           << ", Total: " << total_processed
           << ", Least: " << least_advanced_counter;
        if(found_solution != UINT64_MAX) {
            ss << ", Solution:\033[1;7;32m" << found_solution << "\033[0m";
        } else {
            ss << ", Most: " << most_advanced_counter;
        }
        ss << "]";

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << ss.str() << std::endl;
        }
        ss.str("");
        ss.clear();
    }
}

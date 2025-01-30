#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <chrono>
#include <set>
#include <cstddef>
#include <functional>
#include <cmath>
#include <bitset>

#include "Constant.h"
#include "opcode.h"

// ---------------------------------------------------------
const char *banner = "AdventOfCode 2024 Day 17!";
const char *inputFilePath = INPUT_PATH;

/*
// Summary of 2024 17.1
//
//    // INPUT: 64751475
//
//    mem m;
//    m.a = 64751475;
//    char pgm[] = {
//        1.  2, 4,    BST: B = B & 7
//        2.  1, 2,    BXL: B = B ^ 2
//        3.  7, 5,    CDV: C = A >>B
//        4.  4, 5,    BXC: B = B ^ C
//        5.  1, 3,    BXL: B = B ^ 3
//        6.  5, 5,    OUT: B & 7    
//        7.  0, 3,    ADV: A = A >>3
//        8.  3, 0,    JNZ: A != 0 ? then JUMP 1. else EXIT
//    };
//    while(m.ptr < sizeof(pgm)) {
//        m.operation(pgm[m.ptr], pgm[m.ptr + 1]);
//    }
//    for(auto b : m.r) {
//        std::cout << (int)b << ",";
//    }
//    std::cout << std::endl;
//
//    // OUTPUT: 3,1,4,3,1,7,1,6,3.
*/


int main(int argc, char **argv, char **envp)
{
    std::vector<std::string> args(argv, argv + argc);
    if (args.size() == 1)
    {
        // No input file provided, use the default one
        args.push_back(inputFilePath);
    }
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

    char pgm[] = {
            /* 1 */ 2, 4,   /* BST: B = B & A */
            /* 2 */ 1, 2,   /* BXL: B = B ^ 2 */
            /* 3 */ 7, 5,   /* CDV: C = A >>B */
            /* 4 */ 4, 5,   /* BXC: B = B ^ C */
            /* 5 */ 1, 3,   /* BXL: B = B ^ 3 */
            /* 6 */ 5, 5,   /* OUT: B & 7     */
            /* 7 */ 0, 3,   /* ADV: A = A >>3 */
            /* 8 */ 3, 0,   /* JNZ: JUMP 0 or EXIT*/
        };
    // char need[] = { 2, 1, 7, 4, 1, 5, 0, 3 };
    char need[] = {2, 4, 1, 2, 7, 5, 4, 5, 1, 3, 5, 5, 0, 3, 3, 0};
    uint64_t a_val = pow(2,3*(sizeof(need) - 1) + 1);
    while(a_val < pow(2,3*sizeof(need))) {
        if(0 == (a_val % 10000000)) {
            std::cout << "\033[7m" << std::setw(20) << std::setfill(' ') << std::right << a_val << std::endl << "\033[0m\033[1F";
        }
        mem m;
        m.a = a_val;
        bool test_result = true;
        while(test_result && m.ptr < sizeof(pgm)) {
            bool test = 5 == pgm[m.ptr];
            m.operation(pgm[m.ptr], pgm[m.ptr + 1]);
            if(test) {
                for(auto i = 0; i < m.rptr; i++) {
                    if(i >= sizeof(need)) {
                        test_result = false;
                        break;
                    }
                    if(m.r[i] != need[i]) {
                        test_result = false;
                        break;
                    }
                }
            }
        }
        if(test_result == true) {
            if(m.rptr < sizeof(need)) {
                std::cout << "Not a solution: " << a_val << " - ";
                for(auto ib = 0; ib < sizeof(m.r); ib ++) {
                // for(auto b : m.r) {
                    if(m.r[ib] == need[ib] && ib < sizeof(need)) {
                        std::cout << "\033[7;1;32;47m" << (int)m.r[ib] << " \033[0m";
                    } else {
                        std::cout << "\033[2;37m" << (int)m.r[ib] << " \033[0m";
                        // std::cout << (int)m.r[ib] << " ";
                    }
                }
                std::bitset<64> bs(a_val);
                std::cout << " " << bs << std::endl;
                a_val ++;
                continue;
            }
            std::cout << "Solution: " << a_val << std::endl << std::endl;
            for(auto b : m.r) {
                std::cout << (int)b << ",";
            }
            break;
        } else {
            a_val ++;
        }
    }
    std::cout << std::endl;

    return 0;
}
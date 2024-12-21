#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <fstream>
#include <map>
#include <stdexcept>
// #include <compare>

using namespace std;

template<class Os, class Co>
Os& operator<<(Os& os, const Co& co)
{
    os << '{';
    for (auto const& i : co)
        os << ' ' << i;
    return os << " } ";
}
std::vector<uint64_t> stringUintArray(const std::string & str) {
    std::vector<uint64_t> numbers;
    std::stringstream ss(str);
    uint64_t number;
    while(ss >> number) {
        numbers.push_back(number);
        if(ss.peek() == ' ') {
            ss.ignore();
        }
    }
    return numbers;
}
inline uint64_t count_digits(uint64_t n) {
    if(n == 0) {
        return 1;
    }
    return (uint64_t)(std::floor(std::log10(n)) + 1);
}
inline bool count_digits_is_even(uint64_t n) {
    return (count_digits(n) % 2 == 0) ? true : false;
}
inline vector<uint64_t> blink(uint64_t stone) {
    if(stone == 0) {
        return vector<uint64_t>{1};
    }
    if(count_digits_is_even(stone)) {
        uint64_t div = (uint64_t)std::pow(10, count_digits(stone) / 2);
        return vector<uint64_t>{(uint64_t)(stone / div), (uint64_t)(stone % div)};
    }
    return vector<uint64_t>{(stone * 2024)};
}

vector<uint64_t> & blinkList (vector<uint64_t> &v) {
    vector<uint64_t> w;
    for(auto i : v) {
        vector<uint64_t> u = blink(i);
        for(auto j : u) {
            w.push_back(j);
        }
    }
    v = w;
    return v;
}


struct stone {
    uint64_t val = 0;
    stone * blink_1 = nullptr;
    stone * blink_2 = nullptr;

    stone(uint64_t v = 0) {
        val = v;
        blink_1 = nullptr;
        blink_2 = nullptr;
    }
};

std::ostream & operator<<(std::ostream & os, const stone & s) {
    os << s.val;
    return os;
}

const size_t stoneMemorySize = 4096;

const int N = 75;

stone stoneMemory[stoneMemorySize];
uint64_t stoneMemoryIndex = 0;
std::map<uint64_t,stone *> stones;
stone * getStone(uint64_t value) {
    try {
        return stones.at(value);
    } catch(const std::out_of_range & e) {
        uint64_t newStoneIndex = stoneMemoryIndex++;
        if(newStoneIndex > stoneMemorySize -1) {
            throw std::bad_alloc();
        }
        stoneMemory[newStoneIndex] = stone(value);
        stones[value] = &stoneMemory[newStoneIndex];
        return &stoneMemory[newStoneIndex];
    }
}
stone * getBlinkStone(uint64_t value) {
    stone * ps = getStone(value);
    if(nullptr == ps->blink_1) {
        std::vector<uint64_t> newStoneValues = blink(value);
        ps->blink_1 = getStone(newStoneValues[0]);
        if(newStoneValues.size() == 2) {
            ps->blink_2 = getStone(newStoneValues[1]);
        }
    }
    return ps;
}
std::map<uint64_t,uint64_t>stoneCount[2];
std::map<uint64_t,uint64_t> * pStoneCount = &stoneCount[0];
std::map<uint64_t,uint64_t> * pStoneCountWorking = &stoneCount[1];

int main(int argc, char ** argv, char ** envp) {
    std::cout << "AdventOfCode 2024 Day 11!" << std::endl;

    std::ifstream inputFile("Input/Day11.txt");
    std::string inputString;
    if(inputFile.is_open()) {
        std::string line;
        while(getline(inputFile, line)) {
            inputString = line;
            std::cout << line << std::endl;
            // Get only one line
            break;
        }
        inputFile.close();
    }

    // Read stone values from the input file
    std::vector<uint64_t> inputStones;
    std::stringstream ss(inputString);
    uint64_t number;
    while(ss >> number) {
        inputStones.push_back(number);
        if(ss.peek() == ' ') {
            ss.ignore();
        }
    }

    // Process the first blink iteration
    for(auto stoneVal : inputStones) {
        stone * pCurStone = getBlinkStone(stoneVal);
        (* pStoneCount)[pCurStone->val] ++;
    }
    
    // Process N-1 iterations
    try {
        for(auto i = 1; i <= N; i++) {
            std::cerr << "N: " << (i) << ", Distinct input stones: " << pStoneCount->size();
            pStoneCountWorking->clear();
            for(auto curStoneCount : *pStoneCount) {
                // Value: curStoneCount.first
                // Count: curStoneCount.second
                stone * pCurStone = getBlinkStone(curStoneCount.first);
                // std::cerr << curStoneCount.second << " x Stone(" << pCurStone->val << ") => " << "[";
                (*pStoneCountWorking)[pCurStone->blink_1->val] += curStoneCount.second;
                // std::cerr << "Stone(" << pCurStone->blink_1->val << ")(New count: "<< (*pStoneCountWorking)[pCurStone->blink_1->val] <<")";
                if(nullptr != pCurStone->blink_2) {
                    (*pStoneCountWorking)[pCurStone->blink_2->val] += curStoneCount.second;
                    // std::cerr << ", " << "Stone(" << pCurStone->blink_2->val << ")(New count: "<< (*pStoneCountWorking)[pCurStone->blink_2->val] <<")";
                }
                // std::cerr << "]" << endl;
            }

            // Count the total number of stones
            uint64_t count = 0;
            // std::cerr << "Final: ";
            for(auto sc : *pStoneCountWorking) {
                count += sc.second;
                // std::cerr << "(" << sc.second << " x " << sc.first << "),";
            }
            // std::cerr << std::endl;
            std::cout << " ==> Output " << count << " stone(s). Stone memory: " << stoneMemoryIndex << "/" << stoneMemorySize << std::endl;

            // Make "working list", the new current list
            if(pStoneCount == &stoneCount[0]) {
                pStoneCount = &stoneCount[1];
                pStoneCountWorking = &stoneCount[0];
            } else {
                pStoneCount = &stoneCount[0];
                pStoneCountWorking = &stoneCount[1];
            }
        }
    } catch (std::bad_alloc & e) {
        std::cerr << std::endl << "Not enough room foreseen for the stones; try increasing stoneMemorySize (" << stoneMemorySize << ")" << std::endl;
        return 1;
    }

    /*
    std::cerr << "Final: -----------------------------------";
    for(auto sc : *pStoneCount) {
        std::cerr << sc.second << "\t" << sc.first << std::endl;
    }
    */
    return 0;
}
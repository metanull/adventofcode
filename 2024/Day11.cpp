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

int main(int argc, char ** argv, char ** envp) {
    std::cout << "AdventOfCode 2024 Day 11!" << std::endl;

    std::ifstream inputFile("Input/Day11.txt");
    std::string inputString;
    if(inputFile.is_open()) {
        std::string line;
        while(getline(inputFile, line)) {
            inputString = line;
            std::cout << line << std::endl;
            break;
        }
        inputFile.close();
    }

    vector<uint64_t> _v = stringUintArray(inputString);
    vector<uint64_t> _w;

    vector<uint64_t> * pv = &_v;
    vector<uint64_t> * pw = &_w;
    std::cout << *pv << std::endl;
    for(auto i = 0; i < 25; i++) {
        std::cout << (i+1) << ": " << pv->size() << " -> ";
        pw->clear();
        for(auto currentStone : *pv) {
            vector<uint64_t> blinkedStones = blink(currentStone);
            for(auto j : blinkedStones) {
                pw->push_back(j);
            }
        }
        std::cout << pw->size() << std::endl;

        vector<uint64_t> * pTemp = pv;
        pv = pw;
        pw = pTemp;
    }
   return 0;
}
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

    // List of known stones pointers
    std::map<uint64_t,stone *> heads;

    // Memory to store the unique stones
    stone allStones[1024];
    uint64_t allStonesIndex = 0;

    // Working vectors to store all stones in each iteration
    // We use two vectors to avoid copying the vector at each iteration, by swapping one for the other instead
    vector<uint64_t> _v = stringUintArray(inputString);
    vector<uint64_t> _w;

    // Keep track of all the stones we can get from the pool of known stones for each level
    std::map<int,map<uint64_t,int>> refsByLevel;

    // Perform the blink operation N times
    int N = 25;
    vector<uint64_t> * pv = &_v;
    vector<uint64_t> * pw = &_w;
    std::cout << *pv << std::endl;
    for(auto i = 0; i < N; i++) {
        uint64_t refs = 0;
        map<uint64_t,int> stoneref;
        std::cout << (i+1) << ": " << pv->size() << " -> ";
        pw->clear();
        for(auto currentStone : *pv) {
            stone * ps = nullptr;
            bool isref = false;
            try {
                ps = heads.at(currentStone);
                isref = true;
            } catch (const std::out_of_range & e) {
                allStones[allStonesIndex] = stone(currentStone);
                ps = &allStones[allStonesIndex++];
                heads[currentStone] = ps;
            }
            if(ps->blink_1 == nullptr) {
                vector<uint64_t> blinkedStones = blink(currentStone);
                try {
                    ps->blink_1 = heads.at(blinkedStones[0]);
                } catch (const std::out_of_range & e) {
                    allStones[allStonesIndex] = stone(blinkedStones[0]);
                    ps->blink_1 = &allStones[allStonesIndex++];
                    heads[blinkedStones[0]] = ps->blink_1;
                }
                if(blinkedStones.size() == 2) {
                    try {
                        ps->blink_2 = heads.at(blinkedStones[1]);
                    } catch (const std::out_of_range & e) {
                        allStones[allStonesIndex] = stone(blinkedStones[1]);
                        ps->blink_2 = &allStones[allStonesIndex++];
                        heads[blinkedStones[1]] = ps->blink_2;
                    }
                }
                /*if(!isref) {*/
                    pw->push_back(ps->blink_1->val);
                    if(ps->blink_2 != nullptr) {
                        pw->push_back(ps->blink_2->val);
                    }
               /* } else {
                    if(ps->blink_2 != nullptr) {
                        stoneref[ps->blink_2->val]++;
                        refs = refs + 2;
                    } else {
                        stoneref[ps->blink_1->val]++;
                        refs = refs ++;
                    }
                }*/
            } else {
                if(ps->blink_2 != nullptr) {
                    stoneref[ps->blink_2->val]++;
                    refs = refs + 2;
                } else {
                    stoneref[ps->blink_1->val]++;
                    refs = refs ++;
                }
            }
        }
        
        if(i > 0) {
            // Expand all refs from the level above
            for( const auto & j : refsByLevel[i-1] ) {
                // j.first = stone
                // j.second = count refs

                // Get the next level starting from the level-1's stone
                stone * prs = heads.at(j.first);

                // when it leads to a stone that is also found in teh current level, increase the reference count as appropriate
                if(stoneref.find(prs->blink_1->val) != stoneref.end()) {
                    stoneref[prs->blink_1->val] += j.second;
                } else {
                    // otherwise, add the stone's next level to the current level
                    stoneref[prs->blink_1->val] = j.second;
                }
                refs += j.second;

                if(prs->blink_2 != nullptr) {
                    if( stoneref.find(prs->blink_2->val) != stoneref.end()) {
                        stoneref[prs->blink_2->val] += j.second;
                    } else {
                        // otherwise, add the stone's next level to the current level
                        stoneref[prs->blink_2->val] = j.second;
                    }
                    refs += j.second;
                }
            }
        }
        // Store the references for this level
        refsByLevel[i] = stoneref;

        // Debug print
        std::cout << pw->size() << ", refs: " << refs << ": " << *pw << std::endl;

        // Swap the two vectors before the next iteration
        vector<uint64_t> * pTemp = pv;
        pv = pw;
        pw = pTemp;
    }
    for( const auto & i : refsByLevel ) {
        // i.first = Level
        std::cout << "Level " << i.first << ": ";
        for( const auto & j : i.second ) {
            // j.first = stone
            // j.second = refs
            std::cout << j.first << " -> " << j.second << ", ";
        }
        std::cout << std::endl;
    }
    return 0;
}
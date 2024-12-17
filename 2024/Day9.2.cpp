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

inline uint64_t count_digits(uint64_t n);
inline bool count_digits_is_even(uint64_t n);

struct diskblock {
    uint32_t value = 0;
    uint32_t size = 0;
    uint32_t free = 0;
};
std::map<uint32_t,diskblock> image;
std::map<uint32_t,std::vector<diskblock>> moved;
template<class Os, class Co> Os& operator<<(Os& os, const Co& co);

const char * inputFilePath = "Input/Day9.txt";
int main(int argc, char ** argv, char ** envp) {
    std::cout << "AdventOfCode 2024 Day 9!" << std::endl;

    // Read input file
    std::ifstream inputFile(inputFilePath);
    if(!inputFile || !inputFile.is_open()) {
        std::cerr << "Unable to open" << inputFilePath << std::endl;
        return 1;
    }
    char c;
    bool toggle = true;
    diskblock _t;
    diskblock * pt = nullptr;
    uint32_t val = 0; 
    while(inputFile >> c ) {
        if(toggle) {
            pt = & _t;
            pt->size = (c - '0');
            pt->value = val;
            pt->free = 0;
        } else {
            pt->free = (c - '0');
            image[val++] = *pt;
            pt = nullptr;
        }
        toggle = !toggle;
    }
    inputFile.close();
    if(nullptr != pt) {
        // Add the last element, if requrired
        pt->free = 0;
        image[val] = *pt;
    }

/*
// Print the image of the disk
for(auto b : image) {
    std::cout << "{";
    for (auto i = 0; i < b.second.size; i ++) {
        std::cout << static_cast<char>(b.second.value + '0');
    }
    std::cout << "}{";
    for (auto i = 0; i < b.second.free; i ++) {
        std::cout << '.';
    }
    std::cout << "}" << std::endl;
}
std::cout << std::endl;
*/

    // Defragment
    uint32_t position = std::prev(image.end())->first;
    while(position > 0) {
        for( auto block : image ) {
            if(block.first >= position) {
                break;
            }
            if(block.second.free < image[position].size) {
                continue;
            } else {
                moved[block.first].push_back(image[position]);
                image[block.first].free -= image[position].size;
                
                
                image[position-1].free += image[position].size;
                image[position].size = 0;    

                break;
            }
        }
        position --;
/*
        // Print the image of the disk
        for(auto b : image) {
            std::cout << "{";
            for (auto i = 0; i < b.second.size; i ++) {
                std::cout << static_cast<char>(b.second.value + '0');
            }
            std::cout << "}{";
            try {
                for(auto m : moved.at(b.first)){
                    for (auto i = 0; i < m.size; i ++) {
                        std::cout << static_cast<char>(m.value + '0');
                    }
                }
            } catch(const std::out_of_range & e) {
                // Nothing moved here, just keep on
            }
            for (auto i = 0; i < b.second.free; i ++) {
                std::cout << '.';
            }
            std::cout << "}" << std::endl;
        }
        std::cout << std::endl;
*/
    }

    // Get the checksum
    uint32_t checksumIndex = 0;
    uint32_t checksum = 0;

    for(auto b : image) {
        for (auto i = 0; i < b.second.size; i ++) {
            checksum += (b.second.value) * (checksumIndex ++);
        }
        try {
            for(auto m : moved.at(b.first)){
                for (auto i = 0; i < m.size; i ++) {
                    checksum += (m.value) * (checksumIndex ++);
                }
            }
        } catch(const std::out_of_range & e) {
            // Nothing moved here, just keep on
        }
        for (auto i = 0; i < b.second.free; i ++) {
            checksumIndex ++;
        }
    }
    std::cout << "Checksum: " << checksum << std::endl;
    
    return 0;
}

template<class Os, class Co> Os& operator<<(Os& os, const Co& co)
{
    os << '{';
    for (auto const& i : co)
        os << ' ' << i;
    return os << " } ";
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
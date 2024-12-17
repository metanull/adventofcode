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
    diskblock() = default;
    diskblock(uint32_t v, uint32_t s, uint32_t f) : value(v), size(s), free(f) {}
};
std::vector<diskblock> image;
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
    uint32_t val = 0; 
    while(inputFile >> c ) {
        if(toggle) {
            image.push_back(diskblock(val,(c - '0'),0));
            val++;
        } else {
            image.push_back(diskblock(val,0,(c - '0')));
        }
        toggle = !toggle;
    }
    inputFile.close();

/*    // Print the image of the disk
    for(auto b : image) {
        std::cout << "{";
        for (auto i = 0; i < b.size; i ++) {    std::cout << static_cast<char>(b.value + '0'); }
        for (auto i = 0; i < b.free; i ++) {    std::cout << '.'; }
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;
*/
    int processed = 0;
    auto endcur = image.end() - (1 + processed);
    auto begincur = image.begin();
    while(image.begin() < endcur ) {
        if(endcur->size == 0) {
            processed ++;
            endcur --;
            continue;
        }
        begincur = image.begin();
        while(begincur < endcur) {
            if(begincur->free >= endcur->size) {
                // Prepare a block for insertion
                diskblock b(endcur->value, endcur->size, 0);
                // Update the free space
                begincur->free -= endcur->size;
                endcur->free = endcur->size;
                endcur->size = 0;
                // Tidy up
                auto it = endcur;
                if(it < (image.end()-1) && (it +1)->free != 0) {
                    it->free += (it + 1)->free;
                    (it + 1)->free = 0;
                }
                if(it > (image.begin()) && (it -1 )->free != 0) {
                    it->free += (it - 1)->free;
                    (it - 1)->free = 0;
                }
                // Insert the block
                image.insert(begincur, b);
                break;
            }
            begincur++;
        }
        // endcur --;
        processed ++;
        endcur = image.end() - (1 + processed);
    }
/*
        // Print the image of the disk
        for(auto b : image) {
            std::cout << "{";
            for (auto i = 0; i < b.size; i ++) {    std::cout << static_cast<char>(b.value + '0'); }
            for (auto i = 0; i < b.free; i ++) {    std::cout << '.'; }
            std::cout << "}" << std::endl;
        }
        std::cout << std::endl;
*/
    // Checksum
    uint64_t checksum = 0;
    uint64_t i = 0;
    for(auto b : image) {
        for (auto k = 0; k < b.size; k ++) { 
            checksum += (b.value * (i++));
        }
        i += b.free;
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
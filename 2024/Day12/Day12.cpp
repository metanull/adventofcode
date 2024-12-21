#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <map>
#include <stdexcept>
#include <filesystem>
// #include <compare>

#include "..\include\AdventOfCode.h"
#include "..\include\Point.h"

using namespace std;

const char * banner = "AdventOfCode 2024 Day 12!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day12.txt";

std::vector<Point> mapIt(const Point & first, std::vector<std::vector<char>> & map, const Point & O, const Point & Z);

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;
    std::cout << "CWD: " << std::filesystem::current_path() << std::endl;   // Requires c++ 17 (enabled via CMake configuration)

    std::vector<std::vector<char>> inputMap;
    // Read input file
    std::ifstream inputFile(inputFilePath);
    if(!inputFile || !inputFile.is_open()) {
        std::cerr << "Unable to open " << inputFilePath << "." /* << "CWD: " << std::filesystem::current_path()*/  << std::endl;
        return 1;
    }
    std::string line;
    while(getline(inputFile, line)) {
        std::vector<char> mapLine;
        std::istringstream iss(line);
        char car;
        while(iss >> car) {
            mapLine.push_back(car);
        }
        inputMap.push_back(mapLine);
    }
    inputFile.close();

    // Read the map, looking for regions
    Point O = Point::origin();
    Point Z = Point(inputMap.size(),inputMap[0].size());

    mapIt(O, inputMap, O, Z);

    return 0;
}

std::vector<Point> mapIt(const Point & first, std::vector<std::vector<char>> & map, const Point & O, const Point & Z) {
    if(!first.inBounds(O, Z)) {
        throw std::out_of_range("Point is out of bounds");
    }
    std::vector<Point> region;

    Point cur(first);
    char Ref = map[first.y][first.x];

    // A bold attempt: just browse the full list from O(Origin) to Z(End)... in zigzag
    std::cout << cur.x << " " << cur.y << " " << Ref << std::endl;
    bool inc = true;
    while(cur.inBounds(O,Z)) {
        if(cur == first) {
            cur.TryRight(O,Z);
            continue;
        }
        if(map[cur.y][cur.x] != Ref) {
            std::cout << cur.x << " " << cur.y << " " << map[cur.y][cur.x] << std::endl;
        } else {
            std::cout << cur.x << " " << cur.y << " " << map[cur.y][cur.x] << std::endl;
            // region.push_back(cur);
        }
        if(inc) {
            if(!cur.TryRight(O,Z)) {
                cur.Up();
                inc = !inc;
            }
        } else {
            if(!cur.TryLeft(O,Z)) {
                cur.Up();
                inc = !inc;
            }
        }
    } 
    return region;
}
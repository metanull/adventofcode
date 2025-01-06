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

#include "..\AdventOfCode.h"
#include "..\Point.h"

using namespace std;

const char * banner = "AdventOfCode 2024 Day 12!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day12.txt";

bool PointInList(const std::vector<Point> & v, const Point & t) {
    return std::find(v.begin(), v.end(), t) != v.end();
}

long ProcessRegion(const Point & first, std::vector<std::vector<char>> & map, const Point & O, const Point & Z, std::vector<Point> &Region) {
    char Ref = map[first.y][first.x];
    Point cur(first);
    Region.push_back(first);

    long perimeter = 0;
    
    Point M;
    M = cur;
    if(M.TryUp(O,Z) && Ref == map[M.y][M.x]) {
        if(!PointInList(Region,M) ) {
            perimeter += ProcessRegion(M, map, O, Z, Region);
        }
    } else {
        perimeter++;
    }
    M = cur;
    if(M.TryDown(O,Z) && Ref == map[M.y][M.x]) {
        if(!PointInList(Region,M) ) {
            perimeter += ProcessRegion(M, map, O, Z, Region);
        }
    } else {
        perimeter++;
    }
    M = cur;
    if(M.TryRight(O,Z) && Ref == map[M.y][M.x]) {
        if(!PointInList(Region,M)) {
            perimeter += ProcessRegion(M, map, O, Z, Region);
        }
    } else {
        perimeter++;
    }
    M = cur;
    if(M.TryLeft(O,Z) && Ref == map[M.y][M.x]) {
        if(!PointInList(Region,M)) {
            perimeter += ProcessRegion(M, map, O, Z, Region);
        }
    } else {
        perimeter++;
    }
    return perimeter;
}

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;
    //std::cout << "CWD: " << std::filesystem::current_path() << std::endl;   // Requires c++ 17 (enabled via CMake configuration)

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
    Point Z = Point(inputMap[0].size(),inputMap.size());

    std::vector<Point> processed;
    std::vector<std::vector<Point>> regions;

    Point first = O;
    Point cur(first);
    bool inc = true;

    long totalPrice = 0;
    while(cur.inBounds(O,Z)) {
        if(PointInList(processed, cur)) {
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
            continue;
        }
        std::vector<Point> region;
        long perimeter = ProcessRegion(cur, inputMap, O, Z, region);
        regions.push_back(region);
        long area = region.size();
        char v = inputMap[cur.y][cur.x];

        std::cout << "Point: " << cur.x << " " << cur.y << std::endl;
        std::cout << "Value: " << v << std::endl;
        std::cout << "Area: " << area << std::endl;
        std::cout << "Perimeter: " << perimeter << std::endl << std::endl;

        totalPrice += area * perimeter;
        
        processed.insert( processed.end(), region.begin(), region.end() );
        
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

    std::cout << "Total Price: " << totalPrice << std::endl;

    long totalPriceWithDiscount = 0;
    for(auto r : regions) {
        auto min_xvalue = *std::min_element(r.begin(),r.end(),cmpX);
        auto min_yvalue = *std::min_element(r.begin(),r.end(),cmpY);
        auto max_xvalue = *std::max_element(r.begin(),r.end(),cmpX);
        auto max_yvalue = *std::max_element(r.begin(),r.end(),cmpY);
        Point RegionO(min_xvalue.x, min_yvalue.y);
        Point RegionZ(max_xvalue.x, max_yvalue.y);

        std::cout << "Region: O=" << RegionO << " Z=" << RegionZ << std::endl;

        for(auto p : r) {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Total Price: " << totalPrice << std::endl;
    std::cout << "Total Price With Discount: " << totalPriceWithDiscount << std::endl;

    return 0;
}

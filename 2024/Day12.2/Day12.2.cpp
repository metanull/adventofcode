#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <map>
#include <stdexcept>
// #include <filesystem>
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

long GetDownSides(Point & cur, const Point & O, const Point & Z, std::vector<std::vector<char>> & map, std::vector<Point> &Region) {
    // Test if cur is found in region
    long DownSides = 0;
    try {
        while(cur.inBounds(O,Z)) {
            // Skip all points that are not in the region
            if(!PointInList(Region, cur)) {
                cur.Right();
                continue;
            }
            // Test if the point has a fence on the bottom
            auto curLookup = cur;
            if(!curLookup.TryDown(O,Z) || !PointInList(Region, curLookup)) {
                DownSides++;
                // Skip all adjacent points that are in the region and have a fence on the bottom
                while(true) {
                    cur.Right();
                    if(!PointInList(Region, cur)) {
                        break;
                    }
                    curLookup = cur;
                    if(!curLookup.TryDown(O,Z) || !PointInList(Region, curLookup)) {
                        continue;
                    }
                    break;
                }
            } else {
                // Move to the next point
                cur.Right();
            }
        }
    } catch(std::out_of_range & e) {
        // std::cout << "Out of range: " << cur << std::endl;
    }
    return DownSides;
}

long GetUpSides(Point & cur, const Point & O, const Point & Z, std::vector<std::vector<char>> & map, std::vector<Point> &Region) {
    // Test if cur is found in region
    long UpSides = 0;
    try {
        while(cur.inBounds(O,Z)) {
            // Skip all points that are not in the region
            if(!PointInList(Region, cur)) {
                cur.Right();
                continue;
            }
            // Test if the point has a fence on the top
            auto curLookup = cur;
            if(!curLookup.TryUp(O,Z) || !PointInList(Region, curLookup)) {
                UpSides++;
                // Skip all adjacent points that are in the region and have a fence on the top
                while(true) {
                    cur.Right();
                    if(!PointInList(Region, cur)) {
                        break;
                    }
                    curLookup = cur;
                    if(!curLookup.TryUp(O,Z) || !PointInList(Region, curLookup)) {
                        continue;
                    }
                    break;
                }
            } else {
                // Move to the next point
                cur.Right();
            }
        }
    } catch(std::out_of_range & e) {
        // std::cout << "Out of range: " << cur << std::endl;
    }
    return UpSides;
}

long GetLeftSides(Point & cur, const Point & O, const Point & Z, std::vector<std::vector<char>> & map, std::vector<Point> &Region) {
    // Test if cur is found in region
    long LeftSides = 0;
    try {
        while(cur.inBounds(O,Z)) {
            // Skip all points that are not in the region
            if(!PointInList(Region, cur)) {
                cur.Up();
                continue;
            }
            // Test if the point has a fence on the left
            auto curLookup = cur;
            if(!curLookup.TryLeft(O,Z) || !PointInList(Region, curLookup)) {
                LeftSides++;
                // Skip all adjacent points that are in the region and have a fence on the left
                while(true) {
                    cur.Up();
                    if(!PointInList(Region, cur)) {
                        break;
                    }
                    curLookup = cur;
                    if(!curLookup.TryLeft(O,Z) || !PointInList(Region, curLookup)) {
                        continue;
                    }
                    break;
                }
            } else {
                // Move to the next point
                cur.Up();
            }
        }
    } catch(std::out_of_range & e) {
        // std::cout << "Out of range: " << cur << std::endl;
    }
    return LeftSides;
}

long GetRightSides(Point & cur, const Point & O, const Point & Z, std::vector<std::vector<char>> & map, std::vector<Point> &Region) {
    // Test if cur is found in region
    long RightSides = 0;
    try {
        while(cur.inBounds(O,Z)) {
            // Skip all points that are not in the region
            if(!PointInList(Region, cur)) {
                cur.Up();
                continue;
            }
            // Test if the point has a fence on the right
            auto curLookup = cur;
            if(!curLookup.TryRight(O,Z) || !PointInList(Region, curLookup)) {
                RightSides++;
                // Skip all adjacent points that are in the region and have a fence on the right
                while(true) {
                    cur.Up();
                    if(!PointInList(Region, cur)) {
                        break;
                    }
                    curLookup = cur;
                    if(!curLookup.TryRight(O,Z) || !PointInList(Region, curLookup)) {
                        continue;
                    }
                    break;
                }
            } else {
                // Move to the next point
                cur.Up();
            }
        }
    } catch(std::out_of_range & e) {
        // std::cout << "Out of range: " << cur << std::endl;
    }
    return RightSides;
}

long ProcessRegionApplyDiscount(std::vector<std::vector<char>> & map, std::vector<Point> &Region) {
    long sides = 0;
    auto min_xvalue = *std::min_element(Region.begin(),Region.end(),cmpX);
    auto min_yvalue = *std::min_element(Region.begin(),Region.end(),cmpY);
    auto max_xvalue = *std::max_element(Region.begin(),Region.end(),cmpX);
    auto max_yvalue = *std::max_element(Region.begin(),Region.end(),cmpY);
    Point RegionO(min_xvalue.x, min_yvalue.y);
    Point RegionZ(max_xvalue.x+1, max_yvalue.y+1);

    auto value = map[Region[0].y][Region[0].x];

    std::cout << "Region: O=" << RegionO << " Z=" << RegionZ << std::endl;
/*
    for(auto p : Region) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
*/
    Point cur(RegionO);
    
    do {
        sides += GetDownSides(cur, RegionO, RegionZ, map, Region);
        cur.x = RegionO.x;
    } while(cur.TryUp(RegionO,RegionZ));

    cur = RegionO;
    do {
        sides += GetUpSides(cur, RegionO, RegionZ, map, Region);
        cur.x = RegionO.x;
    } while(cur.TryUp(RegionO,RegionZ));

    cur = RegionO;
    do {
        sides += GetLeftSides(cur, RegionO, RegionZ, map, Region);
        cur.y = RegionO.y;
    } while(cur.TryRight(RegionO,RegionZ));

    cur = RegionO;
    do {
        sides += GetRightSides(cur, RegionO, RegionZ, map, Region);
        cur.y = RegionO.y;
    } while(cur.TryRight(RegionO,RegionZ));

    return sides;
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
    long totalPriceDiscount = 0;
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

        long sides = ProcessRegionApplyDiscount(inputMap, region);
        std::cout << "Sides: " << sides << std::endl;

        totalPrice += area * perimeter;
        totalPriceDiscount += area * sides;
        
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
    std::cout << "Total Price with Discount: " << totalPriceDiscount << std::endl;

    /*long totalPriceWithDiscount = 0;
    for(auto r : regions) {
        ProcessRegionApplyDiscount(inputMap, r);
    }*/

    return 0;
}

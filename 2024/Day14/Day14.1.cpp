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
#include "..\Vector2D.h"

using namespace std;

const char * banner = "AdventOfCode 2024 Day 14!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day14.txt";

/*
bool PointInList(const std::vector<Point> & v, const Point & t) {
    return std::find(v.begin(), v.end(), t) != v.end();
}
*/

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;
    //std::cout << "CWD: " << std::filesystem::current_path() << std::endl;   // Requires c++ 17 (enabled via CMake configuration)

    std::vector<std::pair<long, long>> positions;
    std::vector<std::pair<long, long>> velocities;

    // Read input file
    std::ifstream inputFile(inputFilePath);
    if(!inputFile || !inputFile.is_open()) {
        std::cerr << "Unable to open " << inputFilePath << "." /* << "CWD: " << std::filesystem::current_path()*/  << std::endl;
        return 1;
    }
    std::string line;
    while(getline(inputFile, line)) {
        std::istringstream iss(line);
        long px = 0, py = 0, vx = 0, vy = 0;
        iss.ignore(2);
        iss >> px;
        iss.ignore(1, ',');
        iss >> py;
        iss.ignore(3);
        iss >> vx;
        iss.ignore(1, ',');
        iss >> vy;
        positions.push_back(std::make_pair(px, py));
        velocities.push_back(std::make_pair(vx, vy));
    }
    inputFile.close();

    Point O = Point::origin();
    //Point Z = Point(11,7);
    Point Z = Point(101,103);

    // Dump the input
    for(size_t i = 0; i < positions.size(); i++) {
        std::cout << "pos=<" << positions[i].first << "," << positions[i].second << ">, vel=<" << velocities[i].first << "," << velocities[i].second << ">" << std::endl;
    }

    // Simulate N times
    long N=100;
    long quadrants[4] = {0,0,0,0};
    std::vector<std::pair<long, long>> newPositions;
    for(size_t i = 0; i < positions.size(); i++) {
        auto x = positions[i].first + (N*velocities[i].first);
        auto y = positions[i].second + (N*velocities[i].second);

        if(std::abs(x) > std::abs(Z.x)) {
            x = x % Z.x;
        }
        if(std::abs(y) > std::abs(Z.y)) {
            y = y % Z.y;
        }
        if(x < 0) {
            x += Z.x;
        }
        if(y < 0) {
            y += Z.y;
        }

        newPositions.push_back(std::make_pair(x, y));
        std::cout << "pos=<" << positions[i].first << "," << positions[i].second << ">" << "\t";
        std::cout << "<" << x << "," << y << ">" << "\t";
        if(x != (Z.x -1) / 2 && y != (Z.y -1) / 2) {
            if(x < (Z.x -1) / 2 && y < (Z.y -1) / 2) {
                std::cout << "QUADRANT 1" << std::endl;
                quadrants[0] ++;
            } else if(x > (Z.x -1) / 2 && y < (Z.y -1) / 2) {
                std::cout << "QUADRANT 2" << std::endl;
                quadrants[1] ++;
            } else if(x < (Z.x -1) / 2 && y > (Z.y -1) / 2) {
                std::cout << "QUADRANT 3" << std::endl;
                quadrants[2] ++;
            } else if(x > (Z.x -1) / 2 && y > (Z.y -1) / 2) {
                std::cout << "QUADRANT 4" << std::endl;
                quadrants[3] ++;
            }
        } else {
            std::cout << "IGNORED" << std::endl;
        }
    }
    std::cout << "New positions:" << newPositions.size() << std::endl;
    std::cout << "Quadrants:" << quadrants[0] << " " << quadrants[1] << " " << quadrants[2] << " " << quadrants[3] << std::endl;
    std::cout << "Safety factor: " << (quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]) << std::endl;

    return 0;
}

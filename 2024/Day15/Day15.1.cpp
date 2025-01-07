#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <map>
#include <map>
#include <stdexcept>
// #include <compare>

#include "..\AdventOfCode.h"
#include "..\Point.h"
#include "..\Vector2D.h"
#include "..\Math.h"

using namespace std;

#define ROBOT_CHAR '@'
#define CRATE_CHAR 'O'
#define WALL_CHAR '#'
#define EMPTY_CHAR '.'

const char * banner = "AdventOfCode 2024 Day 15!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day15.txt";

void printCharVector(const std::vector<char> & vec, std::ostream & os = std::cout) {
    for(auto p = 0; p < vec.size(); p++) {
        switch(vec[p]) {
            case ROBOT_CHAR:
                os << "\033[1;32m" << vec[p] << "\033[0m";
                break;
            case CRATE_CHAR:
                os << "\033[1;33m" << vec[p] << "\033[0m";
                break;
            case WALL_CHAR:
                os << "\033[1;31m" << vec[p] << "\033[0m";
                break;
            default:
                os << vec[p];
        }
    }
    os << std::endl;
}
void printCharMap(const std::vector<std::vector<char>> & map, std::ostream & os = std::cout) {
    for(auto p = 0; p < map.size(); p++) {
        printCharVector(map[p], os);
    }
    os << std::endl;
}
std::ostream& operator<<(std::ostream& os, const std::pair<int,int> & p) {
    os << "(" << p.first << "," << p.second << ")";
    return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<char> & vec) {
    for(auto p = 0; p < vec.size(); p++) {
        os << vec[p];
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<char>> & map) {
    for(auto p = 0; p < map.size(); p++) {
        os << map[p] << std::endl;
    }
    return os;
}

void MoveRobot(std::vector<std::vector<char>> & map, std::pair<int,int> & Robot, std::pair<int,int> & direction) {
    std::vector<char> ahead;
    if(direction.first == 0) {
        // Move is vertical
        for(auto p = Robot.second + (direction.second > 0 ? 1 : -1); p >= 0 && p < map.size(); p += (direction.second > 0 ? 1 : -1)) {
            ahead.push_back(map[p][Robot.first]);
        }
    } else {
        // Move is horizontal
        for(auto p = Robot.first + (direction.first > 0 ? 1 : -1); p >= 0 && p < map[Robot.second].size(); p += (direction.first > 0 ? 1 : -1)) {
            ahead.push_back(map[Robot.second][p]);
        }
    }
    // Get the first WALL_CHAR in ahead
    auto it = std::find(ahead.begin(), ahead.end(), WALL_CHAR);
    if(it != ahead.end()) {
        ahead.erase(it, ahead.end());
    }
    if(ahead.size() == 0) {
        // No moves are possible
        direction.first = 0;
        direction.second = 0;
        return;
    }
    // Count the number of CRATE_CHAR in ahead
    int crateCount = std::count(ahead.begin(), ahead.end(), CRATE_CHAR);
    if(crateCount > ahead.size()) {
        // No moves are possible
        direction.first = 0;
        direction.second = 0;
        return;
    }

    // Push all crates ahead to the edge...
    for(auto p = 0; p < ahead.size(); p++) {
        if(direction.first == 0) {
            if(direction.second > 0) {
                if(p < ahead.size() - crateCount) {
                    map[Robot.second + p + 1][Robot.first] = EMPTY_CHAR;
                } else {
                    map[Robot.second + p + 1][Robot.first] = CRATE_CHAR;
                }
            } else {
                if(p < ahead.size() - crateCount) {
                    map[Robot.second - p - 1][Robot.first] = EMPTY_CHAR;
                } else {
                    map[Robot.second - p - 1][Robot.first] = CRATE_CHAR;
                }
            }
        } else {
            if(direction.first > 0) {
                if(p < ahead.size() - crateCount) {
                    map[Robot.second][Robot.first + p + 1] = EMPTY_CHAR;
                } else {
                    map[Robot.second][Robot.first + p + 1] = CRATE_CHAR;
                }
            } else {
                if(p < ahead.size() - crateCount) {
                    map[Robot.second][Robot.first - p - 1] = EMPTY_CHAR;
                } else {
                    map[Robot.second][Robot.first - p - 1] = CRATE_CHAR;
                }
            }
        }    
    }
    // ... then move the robot of ahead.size() - crateCount in the right direction
    map[Robot.second][Robot.first] = EMPTY_CHAR;
    if(direction.first == 0) {
        if(direction.second > 0) {
            Robot.second += (ahead.size() - crateCount);
        } else {
            Robot.second -= (ahead.size() - crateCount);
        }
    } else {
        if(direction.first > 0) {
            Robot.first += (ahead.size() - crateCount);
        } else {
            Robot.first -= (ahead.size() - crateCount);
        }
    }
    map[Robot.second][Robot.first] = ROBOT_CHAR;
}

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;

    std::vector<std::vector<char>> map;
    std::vector<char> moves;

    // Read input file
    {
        // Read input file
        std::ifstream inputFile(inputFilePath);
        if(!inputFile || !inputFile.is_open()) {
            std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
            return 1;
        }
        // Read the map
        std::string line;
        while(getline(inputFile, line)) {
            if(line.length() == 0) {
                break;
            }
            map.push_back(std::vector<char>(line.begin(), line.end()));
        }
        // Read the moves
        while(getline(inputFile, line)) {
            moves.insert(moves.end(), line.begin(), line.end());
        }
        inputFile.close();
    }

    // Initialization
    std::pair<int,int> O = std::make_pair(1,1);
    std::pair<int,int> Z = std::make_pair(map[0].size()-2, map.size()-2);
    std::pair<int,int> Robot = std::make_pair(0,0);
    for(auto p = 0; p < map.size(); p++) {
        for(auto q = 0; q < map[p].size(); q++) {
            if(map[p][q] == ROBOT_CHAR) {
                Robot = std::make_pair(q,p);
                break;
            }
        }
    }

    // Print the map
    printCharMap(map);
    // std::cout << "Map:  " << std::endl << map << std::endl;
    std::cout << "Moves:" << moves << std::endl;
    std::cout << "Robot:" << Robot << std::endl;

    // Move the robot around
    char lastDirection = 0;
    int distance = 1;
    for(auto p = 0; p < moves.size(); p++) {
        char direction = moves[p];
        if(lastDirection == 0) {
            lastDirection = direction;
            continue;
        }
        if( direction == lastDirection) {
            distance++;
            continue;
        }
        std::pair<int,int> v = std::make_pair(
              distance * (lastDirection == '<' ? -1 : (lastDirection == '>' ? 1 : 0))
            , distance * (lastDirection == '^' ? -1 : (lastDirection == 'v' ? 1 : 0))
        );
        
        MoveRobot(map, Robot, v);

        // Print the map
        std::cout << "Moves:" << moves << std::endl;
        std::cout << "Robot:" << Robot << std::endl;
        printCharMap(map);
        std::cout << std::endl;

        // Prepare next iteration
        distance = 1;
        lastDirection = direction;
    }

    return 0;
}

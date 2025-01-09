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
#include <set>
#include <filesystem>

#include "..\AdventOfCode.h"
#include "..\Point.h"
#include "..\Vector2D.h"
#include "..\Math.h"

using namespace std;

#define ROBOT_CHAR '@'
#define CRATE_CHAR 'O'
#define CRATE_CHAR1 '['
#define CRATE_CHAR2 ']'
#define WALL_CHAR '#'
#define EMPTY_CHAR '.'

const char * banner = "AdventOfCode 2024 Day 15!";
const char * inputFilePath = "../../2024/Input/Day15.txt";

/**
 * Color print a vector<char>
 */
void printCharVector(const std::vector<char> & vec, std::ostream & os = std::cout);
/**
 * Color print a vector<vector<char>>
 */
void printCharMap(const std::vector<std::vector<char>> & map, std::ostream & os = std::cout);
/**
 * Print a pair<int,int>
 */
std::ostream& operator<<(std::ostream& os, const std::pair<int,int> & p);
/**
 * Print a vector<char>
 */
std::ostream& operator<<(std::ostream& os, const std::vector<char> & vec);
/**
 * Print a vector<vector<char>>
 */
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<char>> & map);

/**
 * Move the robot on the map, following the received instructions
 */
void MoveRobot(std::vector<std::vector<char>> & map, std::vector<char> & moves);
/**
 * Perform one individual instruction
 */
void MoveRobot(std::vector<std::vector<char>> & map, std::pair<int,int> & Robot, std::pair<int,int> & Direction);

int PushUp(std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];
    char next = map[point.second + 1][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {
        // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {
        // Nothing to push
        return 1;
    }

    // Push "c" ahead
    if(WALL_CHAR == next) {
        return 0;
    }
    if(EMPTY_CHAR == next) {
        map[point.second + 1][point.first] = c;
        map[point.second][point.first] = EMPTY_CHAR;
        return 1;
    }
    if(CRATE_CHAR2 == next) {
        depth ++;
        int r = std::min(
              PushUp(map, std::make_pair(point.first - 1,point.second +1), depth)
            , PushUp(map, std::make_pair(point.first,point.second +1), depth)
        );
        if(r) {
            return PushUp(map, point, depth);
        }
        return 0;
    }
    if(CRATE_CHAR1 == next) {
        depth ++;
        int r = std::min(
              PushUp(map, std::make_pair(point.first,point.second +1), depth)
            , PushUp(map, std::make_pair(point.first + 1,point.second +1), depth)
        );
        if(r) {
            return PushUp(map, point, depth);
        }
        return 0;
    }
    throw std::runtime_error("Invalid state (character ahead is not in [EMPTY_CHAR, WALL_CHAR, CRATE_CHAR1, CRATE_CHAR2])");
}
int TestPushUp(const std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];
    char next = map[point.second + 1][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {
        // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {
        // Nothing to push
        return 1;
    }

    // Test if there is room ahead to move "c"
    if(WALL_CHAR == next) {
        return 0;
    }
    if(EMPTY_CHAR == next) {
        return 1;
    }
    if(CRATE_CHAR2 == next) {
        depth ++;
        return std::min(
              TestPushUp(map, std::make_pair(point.first - 1,point.second +1), depth)
            , TestPushUp(map, std::make_pair(point.first,point.second +1), depth)
        );
    }
    if(CRATE_CHAR1 == next) {
        depth ++;
        return std::min(
              TestPushUp(map, std::make_pair(point.first,point.second +1), depth)
            , TestPushUp(map, std::make_pair(point.first + 1,point.second +1), depth)
        );
    }
    throw std::runtime_error("Invalid state (character ahead is not in [EMPTY_CHAR, WALL_CHAR, CRATE_CHAR1, CRATE_CHAR2])");
}

int PushDown(std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];
    char next = map[point.second - 1][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {
        // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {
        // Nothing to push
        return 1;
    }

    // Push "c" ahead
    if(WALL_CHAR == next) {
        return 0;
    }
    if(EMPTY_CHAR == next) {
        map[point.second - 1][point.first] = c;
        map[point.second][point.first] = EMPTY_CHAR;
        return 1;
    }
    if(CRATE_CHAR2 == next) {
        depth ++;
        int r = std::min(
              PushDown(map, std::make_pair(point.first - 1,point.second -1), depth)
            , PushDown(map, std::make_pair(point.first,point.second -1), depth)
        );
        if(r) {
            return PushDown(map, point, depth);
        }
        return 0;
    }
    if(CRATE_CHAR1 == next) {
        depth ++;
        int r = std::min(
              PushDown(map, std::make_pair(point.first,point.second -1), depth)
            , PushDown(map, std::make_pair(point.first + 1,point.second -1), depth)
        );
        if(r) {
            return PushDown(map, point, depth);
        }
        return 0;
    }
    throw std::runtime_error("Invalid state (character ahead is not in [EMPTY_CHAR, WALL_CHAR, CRATE_CHAR1, CRATE_CHAR2])");
}
int TestPushDown(const std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];
    char next = map[point.second - 1][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {
        // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {
        // Nothing to push
        return 1;
    }

    // Test if there is room ahead to move "c"
    if(WALL_CHAR == next) {
        return 0;
    }
    if(EMPTY_CHAR == next) {
        return 1;
    }
    if(CRATE_CHAR2 == next) {
        depth ++;
        return std::min(
              TestPushDown(map, std::make_pair(point.first - 1,point.second -1), depth)
            , TestPushDown(map, std::make_pair(point.first,point.second -1), depth)
        );
    }
    if(CRATE_CHAR1 == next) {
        depth ++;
        return std::min(
              TestPushDown(map, std::make_pair(point.first,point.second -1), depth)
            , TestPushDown(map, std::make_pair(point.first + 1,point.second -1), depth)
        );
    }
    throw std::runtime_error("Invalid state (character ahead is not in [EMPTY_CHAR, WALL_CHAR, CRATE_CHAR1, CRATE_CHAR2])");
}

int PushRight(std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {
        // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {
        // Nothing to push
        return 1;
    }

    // Push what's ahead of "c"
    char next = map[point.second][point.first + (1)];
    if(WALL_CHAR == next) {
        return 0;
    }
    if(EMPTY_CHAR != next) {
        depth ++;
        PushRight(map, std::make_pair(point.first + 1, point.second), depth);
    }
    map[point.second][point.first + 1] = map[point.second][point.first];
    map[point.second][point.first] = EMPTY_CHAR;
    return 1;
}
int TestPushRight(const std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {    // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {   // Nothing to push
        return 1;
    }
    char next = map[point.second][point.first + (1)];
    if(WALL_CHAR == next) { // No room available
        return 0;
    }
    if(EMPTY_CHAR != next) {    // There is something ahead
        return TestPushRight(map, std::make_pair(point.first + 1, point.second), depth);
    }
    return 1;
}

int PushLeft(std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {
        // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {
        // Nothing to push
        return 1;
    }

    // Push what's ahead of "c"
    char next = map[point.second][point.first - (1)];
    if(WALL_CHAR == next) {
        return 0;
    }
    if(EMPTY_CHAR != next) {
        depth ++;
        PushLeft(map, std::make_pair(point.first - 1, point.second), depth);
    }
    map[point.second][point.first - 1] = map[point.second][point.first];
    map[point.second][point.first] = EMPTY_CHAR;
    return 1;
}
int TestPushLeft(const std::vector<std::vector<char>> & map, const std::pair<int,int> point, int & depth) {
    char c = map[point.second][point.first];

    // Test if "c" is pushable
    if(WALL_CHAR == c) {    // Not pushable
        return 0;
    }
    if(EMPTY_CHAR == c) {   // Nothing to push
        return 1;
    }
    char next = map[point.second][point.first - (1)];
    if(WALL_CHAR == next) { // No room available
        return 0;
    }
    if(EMPTY_CHAR != next) {    // There is something ahead
        return TestPushLeft(map, std::make_pair(point.first - 1, point.second), depth);
    }
    return 1;
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
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
            return 1;
        }
        // Read the map
        std::string line;
        while(getline(inputFile, line)) {
            if(line.length() == 0) {
                break;
            }
            std::vector<char> row;
            for(char c : line) {
                switch(c) {
                    case ROBOT_CHAR:
                        row.push_back(ROBOT_CHAR);
                        row.push_back(EMPTY_CHAR);
                        break;
                    case CRATE_CHAR:
                        row.push_back(CRATE_CHAR1);
                        row.push_back(CRATE_CHAR2);
                        break;
                    case WALL_CHAR:
                        row.push_back(WALL_CHAR);
                        row.push_back(WALL_CHAR);
                        break;
                    case EMPTY_CHAR:
                        row.push_back(EMPTY_CHAR);
                        row.push_back(EMPTY_CHAR);
                        break;
                    default:
                        std::cerr << "Invalid character in the map: " << c << std::endl;
                        return 1;
                }
            }
            map.push_back(row);
        }
        // Read the moves
        while(getline(inputFile, line)) {
            moves.insert(moves.end(), line.begin(), line.end());
        }
        inputFile.close();
    }

    // Print the initial state
    // printCharMap(map);
    // Move the robot, following received instructions
    MoveRobot(map, moves);
    // Print the result
    printCharMap(map);

    // Calculate the Crate's Goods Positioning System coordinates
    long long SumGPS = 0;
    for(auto p = 0; p < map.size(); p++) {
        for(auto q = 0; q < map[p].size(); q++) {
            if(map[p][q] == CRATE_CHAR1) {
                SumGPS += (100 * p) + (1 * q);
            }
        }
    }
    std::cout << "SumGPS: " << SumGPS << std::endl;

    return 0;
}

void printCharVector(const std::vector<char> & vec, std::ostream & os) {
    for(auto p = 0; p < vec.size(); p++) {
        switch(vec[p]) {
            case ROBOT_CHAR:
                os << "\033[1;32m" << vec[p] << "\033[0m";
                break;
            case CRATE_CHAR:
            case CRATE_CHAR1:
            case CRATE_CHAR2:
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
void printCharMap(const std::vector<std::vector<char>> & map, std::ostream & os) {
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

/**
 * Follow all move instructions
 */
void MoveRobot(std::vector<std::vector<char>> & map, std::vector<char> & moves) {
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

    // Print the initial state
    // std::cout << "Moves:" << moves << std::endl;
    // std::cout << "Robot:" << Robot << std::endl;
    // std::cout << "Map:  " << std::endl << map << std::endl;
    
    // Read move instructions and Move the robot around
    char lastDirection = 0;
    int distance = 1;
    for(auto p = 0; p < moves.size(); p++) {
        // Collect move instruction
        char direction = moves[p];
        if(lastDirection == 0) {
            lastDirection = direction;
            continue;
        }
        if( direction == lastDirection) {
            distance++;
            continue;
        }

        // Move the Robot ahead as much as possible
        std::pair<int,int> v = std::make_pair(
              distance * (lastDirection == '<' ? -1 : (lastDirection == '>' ? 1 : 0))
            , distance * (lastDirection == '^' ? -1 : (lastDirection == 'v' ? 1 : 0))
        );
        MoveRobot(map, Robot, v);

        // Prepare next iteration
        distance = 1;
        lastDirection = direction;
    }
    // Do the last move
    // Move the Robot ahead as much as possible
    std::pair<int,int> v = std::make_pair(
          distance * (lastDirection == '<' ? -1 : (lastDirection == '>' ? 1 : 0))
        , distance * (lastDirection == '^' ? -1 : (lastDirection == 'v' ? 1 : 0))
    );
    MoveRobot(map, Robot, v);
}

/**
 * Perform one individual move
 */
void MoveRobot(std::vector<std::vector<char>> & map, std::pair<int,int> & Robot, std::pair<int,int> & direction) {
    bool isVertical = direction.first == 0;
    bool isNegative = direction.first < 0 || direction.second < 0;
    int moveCount = isVertical ? std::abs(direction.second) : std::abs(direction.first);
    int depth = 0;

    // Print the move meta data
    // std::cout << "COMMAND: " << Robot << " " << std::right << std::setfill(isVertical ? (isNegative ? '^' : 'v') : (isNegative ? '<' : '>')) << std::setw(moveCount + 1) << moveCount << std::endl;

    if(isVertical) {
        if(isNegative) {
            for(auto p = 0; p < moveCount && TestPushDown(map, Robot, depth); p++) {
                PushDown(map, Robot, depth);
                Robot.second -= 1;
            }
        } else {
            for(auto p = 0; p < moveCount && TestPushUp(map, Robot, depth); p++) {
                PushUp(map, Robot, depth);
                Robot.second += 1;
            }
        }
    } else {
        if(isNegative) {
            for(auto p = 0; p < moveCount && TestPushLeft(map, Robot, depth); p++) {
                PushLeft(map, Robot, depth);
                Robot.first -= 1;
            }
        } else {
            for(auto p = 0; p < moveCount && TestPushRight(map, Robot, depth); p++) {
                PushRight(map, Robot, depth);
                Robot.first += 1;
            }
        }
    }

    // Print the result
    // std::cout << "RESULT:  " << Robot << std::endl;
    // printCharMap(map);
    // std::cout << std::endl;
}
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
const char * inputFilePath = "../../../Input/Day15.txt";

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

std::pair<int,int> TryMove(std::vector<std::vector<char>> & map, std::pair<int,int> From, std::pair<int,int> Direction) {
    if(Direction.first == 0 && Direction.second == 0) {
        // Null move
        return std::make_pair(0,0);
    }
    if(Direction.first == 0) {
        // Vertical Move
        if(Direction.second > 0) {
            // Move Up
            std::pair<int,int> move = std::make_pair(0,1);
            if(From.second == map.size() -1) {
                // Going out of bounds
                return std::make_pair(0,0);
            }
            switch(map[From.second + 1][From.first]) {
                case EMPTY_CHAR: {
                    // Empty, try further, keeping track of the number of successfull moves
                    std::pair<int,int> nextMove = TryMove(map,std::make_pair(From.first, From.second + 1),std::make_pair(Direction.first, Direction.second - 1));
                    move.second += nextMove.second;
                    return move;
                }
                case CRATE_CHAR1: {
                    // Crate ahead, try further, keeping track of the number of successfull moves (! creates span over two horizontal cells)
                    std::pair<int,int> nextMoveL = TryMove(map,std::make_pair(From.first,     From.second + 1),std::make_pair(Direction.first, Direction.second - 1));
                    std::pair<int,int> nextMoveR = TryMove(map,std::make_pair(From.first + 1, From.second + 1),std::make_pair(Direction.first, Direction.second - 1));
                    move.second += std::min(nextMoveL.second,nextMoveR.second);
                    return move;
                } break;
                case CRATE_CHAR2: {
                    // Crate ahead, try further, keeping track of the number of successfull moves (! creates span over two horizontal cells)
                    std::pair<int,int> nextMoveL = TryMove(map,std::make_pair(From.first - 1, From.second + 1),std::make_pair(Direction.first, Direction.second - 1));
                    std::pair<int,int> nextMoveR = TryMove(map,std::make_pair(From.first,     From.second + 1),std::make_pair(Direction.first, Direction.second - 1));
                    move.second += std::min(nextMoveL.second,nextMoveR.second);
                    return move;
                } break;
                case WALL_CHAR: {
                    // Hitting a wall
                    return std::make_pair(0,0);
                } break;
            }
            throw std::out_of_range("Character was unexpected");
        } else {
            // Move Down
            std::pair<int,int> move = std::make_pair(0,-1);
            if(From.second == 0) {
                // Going out of bounds
                return std::make_pair(0,0);
            }
            switch(map[From.second - 1][From.first]) {
                case EMPTY_CHAR: {
                    // Empty, try further, keeping track of the number of successfull moves
                    std::pair<int,int> nextMove = TryMove(map,std::make_pair(From.first, From.second - 1),std::make_pair(Direction.first, Direction.second - 1));
                    move.second -= nextMove.second;
                    return move;
                }
                case CRATE_CHAR1: {
                    // Crate ahead, try further, keeping track of the number of successfull moves (! creates span over two horizontal cells)
                    std::pair<int,int> nextMoveL = TryMove(map,std::make_pair(From.first,     From.second - 1),std::make_pair(Direction.first, Direction.second - 1));
                    std::pair<int,int> nextMoveR = TryMove(map,std::make_pair(From.first + 1, From.second - 1),std::make_pair(Direction.first, Direction.second - 1));
                    move.second -= std::max(nextMoveL.second,nextMoveR.second);
                    return move;
                } break;
                case CRATE_CHAR2: {
                    // Crate ahead, try further, keeping track of the number of successfull moves (! creates span over two horizontal cells)
                    std::pair<int,int> nextMoveL = TryMove(map,std::make_pair(From.first - 1, From.second - 1),std::make_pair(Direction.first, Direction.second - 1));
                    std::pair<int,int> nextMoveR = TryMove(map,std::make_pair(From.first,     From.second - 1),std::make_pair(Direction.first, Direction.second - 1));
                    move.second -= std::max(nextMoveL.second,nextMoveR.second);
                    return move;
                } break;
                case WALL_CHAR: {
                    // Hitting a wall
                    return std::make_pair(0,0);
                } break;
            }
            throw std::out_of_range("Character was unexpected");
        }
    } else {
        // Horizontal Move
        if(Direction.first > 0) {
            // Move right
            std::pair<int,int> move = std::make_pair(1,0);
            if(From.first == map[From.second].size() - 1) {
                // Going out of bounds
                return std::make_pair(0,0);
            }
            switch(map[From.second][From.first + 1] ) {
                case EMPTY_CHAR: {
                    // Empty, try further, keeping track of the number of successfull moves
                    std::pair<int,int> nextMove = TryMove(map,std::make_pair(From.first + 1, From.second),std::make_pair(Direction.first - 1, Direction.second));
                    move.first += nextMove.first;
                    return move;
                }
                case CRATE_CHAR1: {
                    // Crate ahead, try further, keeping track of the number of successfull moves (! creates span over two horizontal cells)
                    std::pair<int,int> nextMove = TryMove(map,std::make_pair(From.first + 2, From.second),std::make_pair(Direction.first, Direction.second));
                    /*if(nextMove.first > 0) {
                        move.first += (nextMove.first - 1);
                    }*/
                    move.first += nextMove.first;
                    return move;
                } break;
                case WALL_CHAR: {
                    // Hitting a wall
                    return std::make_pair(0,0);
                } break;
            }
            throw std::out_of_range("Character was unexpected");
        } else {
            // Move left
            std::pair<int,int> move = std::make_pair(-1,0);
            if(From.first == 0) {
                // Going out of bounds
                return std::make_pair(0,0);
            }
            switch(map[From.second][From.first - 1] ) {
                case EMPTY_CHAR: {
                    // Empty, try further, keeping track of the number of successfull moves
                    std::pair<int,int> nextMove = TryMove(map,std::make_pair(From.first - 1, From.second),std::make_pair(Direction.first + 1, Direction.second));
                    move.first += nextMove.first;
                    return move;
                }
                case CRATE_CHAR2: {
                    // Crate ahead, try further, keeping track of the number of successfull moves (! creates span over two horizontal cells)
                    std::pair<int,int> nextMove = TryMove(map,std::make_pair(From.first - 2, From.second),std::make_pair(Direction.first, Direction.second));
                    /*if(nextMove.first < 0) {
                        move.first += (nextMove.first + 1);
                    }*/
                    move.first += nextMove.first;
                    return move;
                } break;
                case WALL_CHAR: {
                    // Hitting a wall
                    return std::make_pair(0,0);
                } break;
            }
            throw std::out_of_range("Character was unexpected");
        }
    }
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
    printCharMap(map);

    // Move the robot, following received instructions
    // MoveRobot(map, moves);

    std::pair<int,int> t = {0,0};
    t = TryMove(map, std::make_pair(12,6), std::make_pair( 6,0));
    t = TryMove(map, std::make_pair(12,6), std::make_pair(-6,0));
    t = TryMove(map, std::make_pair(12,6), std::make_pair(0, 6));
    t = TryMove(map, std::make_pair(12,6), std::make_pair(0,-6));


    // Print the result
    printCharMap(map);

    // Calculate the Crate's Goods Positioning System coordinates
    // std::set<std::pair<int,int>> GPS;
    long long SumGPS = 0;
    for(auto p = 0; p < map.size(); p++) {
        for(auto q = 0; q < map[p].size(); q++) {
            if(map[p][q] == CRATE_CHAR) {
                SumGPS += (100 * p) + (1 * q);
                // GPS.emplace(q,p);
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

    // Print the move meta data
    // std::cout << "BEFORE: Robot:" << Robot << std::endl;
    // std::cout << "BEFORE: Velocity:" << direction << std::endl;

    // Collect what is ahead of the robot (until a wall, the end of the map, or the maximum required number of moves is reached)
    std::vector<char> ahead;
    int crateCount = 0;
    if(direction.first == 0) {
        // Move is vertical
        if(direction.second > 0) {
            for(auto p = Robot.second + 1; p <= Robot.second + direction.second + crateCount && p < map.size(); p++) {
                crateCount += (map[p][Robot.first] == CRATE_CHAR);
                ahead.push_back(map[p][Robot.first]);
                if(map[p][Robot.first] == WALL_CHAR) {
                    break;
                }
            }
        } else {
            for(auto p = Robot.second - 1; p >= Robot.second + direction.second - crateCount && p >= 0; p--) {
                crateCount += (map[p][Robot.first] == CRATE_CHAR);
                ahead.push_back(map[p][Robot.first]);
                if(map[p][Robot.first] == WALL_CHAR) {
                    break;
                }
            }
        }
    } else {
        // Move is horizontal
        if(direction.first > 0) {
            for(auto p = Robot.first + 1; p <= Robot.first + direction.first + crateCount && p < map[Robot.second].size(); p++) {
                crateCount += (map[Robot.second][p] == CRATE_CHAR);
                ahead.push_back(map[Robot.second][p]);
                if(map[Robot.second][p] == WALL_CHAR) {
                    break;
                }
            }
        } else {
            for(auto p = Robot.first - 1; p >= Robot.first + direction.first - crateCount && p >= 0; p--) {
                crateCount += (map[Robot.second][p] == CRATE_CHAR);
                ahead.push_back(map[Robot.second][p]);
                if(map[Robot.second][p] == WALL_CHAR) {
                    break;
                }
            }
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
    // int crateCount = std::count(ahead.begin(), ahead.end(), CRATE_CHAR);
    if(crateCount > ahead.size()) {
        // No moves are possible
        direction.first = 0;
        direction.second = 0;
        return;
    }

    // Push all crates ahead to the edge...
    if(direction.first == 0) {
        // Vertical
        int inc = direction.second > 0 ? 1 : -1;
        if(inc > 0) {
            for(auto p = 0; p < ahead.size(); p++) {
                if(crateCount && p < crateCount) {
                    map[Robot.second + (ahead.size() - p)][Robot.first] = CRATE_CHAR;
                } else {
                    map[Robot.second + (ahead.size() - p)][Robot.first] = EMPTY_CHAR;
                }
            }
        } else {
            for(auto p = 0; p < ahead.size(); p++) {
                if(crateCount && p < crateCount) {
                    map[Robot.second - (ahead.size() - p)][Robot.first] = CRATE_CHAR;
                } else {
                    map[Robot.second - (ahead.size() - p)][Robot.first] = EMPTY_CHAR;
                }
            }
        }
    } else {
        // Horizontal
        int inc = direction.first > 0 ? 1 : -1;
        if(inc > 0) {
            for(auto p = 0; p < ahead.size(); p++) {
                if(crateCount && p < crateCount) {
                    map[Robot.second][Robot.first + (ahead.size() - p)] = CRATE_CHAR;
                } else {
                    map[Robot.second][Robot.first + (ahead.size() - p)] = EMPTY_CHAR;
                }
            }
        } else {
            for(auto p = 0; p < ahead.size(); p++) {
                if(crateCount && p < crateCount) {
                    map[Robot.second][Robot.first - (ahead.size() - p)] = CRATE_CHAR;
                } else {
                    map[Robot.second][Robot.first - (ahead.size() - p)] = EMPTY_CHAR;
                }
            }
        }
    }

    // ... then move the robot of ahead.size() - crateCount in the right direction
    // ... and keep a trace of the actual moved performed by the robot, replacing the direction vector
    // 1. Clear the current position of the robot
    map[Robot.second][Robot.first] = EMPTY_CHAR;
    // 2. Move the robot
    if(direction.first == 0) {
        if(direction.second > 0) {
            direction.second = (ahead.size() - crateCount);
            Robot.second += (ahead.size() - crateCount);
        } else {
            direction.second = -(ahead.size() - crateCount);
            Robot.second -= (ahead.size() - crateCount);
        }
    } else {
        if(direction.first > 0) {
            direction.first = (ahead.size() - crateCount);
            Robot.first += (ahead.size() - crateCount);
        } else {
            direction.first = -(ahead.size() - crateCount);
            Robot.first -= (ahead.size() - crateCount);
        }
    }
    // 3. Set the new position of the robot
    map[Robot.second][Robot.first] = ROBOT_CHAR;

    // Print the result
    // printCharMap(map);
    // std::cout << "AFTER: Robot:" << Robot << std::endl;
    // std::cout << "AFTER: Actual move:" << direction << std::endl;
    // std::cout << std::endl;
}
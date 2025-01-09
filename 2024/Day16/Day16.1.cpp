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

#define REINDEER_CHAR 'S'
#define EXIT_CHAR 'E'
#define WALL_CHAR '#'
#define EMPTY_CHAR '.'

const char * banner = "AdventOfCode 2024 Day 16!";
const char * inputFilePath = "../../2024/Input/Day16.txt";

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
 * Move the Reindeer on the map, following the received instructions
 * @param map The map
 * @return The best/lowest score for crossing the maze
*/
long long MoveReindeer(std::vector<std::vector<char>> & map);

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
            map.push_back(std::vector<char>(line.begin(), line.end()));
        }
        inputFile.close();
    }

    // Print the Maze
    printCharMap(map);
    // Move the Reindeer in the Maze
    std::cout << "Score: " << MoveReindeer(map) << std::endl;

    return 0;
}

void printCharVector(const std::vector<char> & vec, std::ostream & os) {
    for(auto p = 0; p < vec.size(); p++) {
        switch(vec[p]) {
            case REINDEER_CHAR:
                os << "\033[1;32m" << vec[p] << "\033[0m";
                break;
            case EXIT_CHAR:
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

inline char Clockwise(char Direction) {
    switch(Direction) {
        case 'N': return 'E';
        case 'E': return 'S';
        case 'S': return 'W';
        case 'W': return 'N';
    }
    throw std::runtime_error("Invalid direction");
}
inline char CounterClockwise(char Direction) {
    switch(Direction) {
        case 'N': return 'W';
        case 'W': return 'S';
        case 'S': return 'E';
        case 'E': return 'N';
    }
    throw std::runtime_error("Invalid direction");
}
inline char Ahead(const std::vector<std::vector<char>> & map, std::pair<int,int> Reindeer, char Direction) {
    switch(Direction) {
        case 'N': return map[Reindeer.second - 1][Reindeer.first];
        case 'E': return map[Reindeer.second][Reindeer.first + 1];
        case 'S': return map[Reindeer.second + 1][Reindeer.first];
        case 'W': return map[Reindeer.second][Reindeer.first - 1];
    }
    throw std::runtime_error("Invalid direction");
}
inline std::pair<int,int> Move(std::pair<int,int> Reindeer, char Direction) {
    switch(Direction) {
        case 'N': return std::make_pair(Reindeer.first, Reindeer.second - 1);
        case 'E': return std::make_pair(Reindeer.first + 1, Reindeer.second);
        case 'S': return std::make_pair(Reindeer.first, Reindeer.second + 1);
        case 'W': return std::make_pair(Reindeer.first - 1, Reindeer.second);
    }
    throw std::runtime_error("Invalid direction");
}

long long veryBestScore = LLONG_MAX;
long long Walk(std::vector<std::vector<char>> & map, std::pair<int,int> Reindeer, char Direction, std::set<std::pair<char,std::pair<int,int>>> & visited, long long scoreRef) {
    char cw = Clockwise(Direction);
    char ccw = CounterClockwise(Direction);

    // Check if the Reindeer is at the exit
    if(map[Reindeer.second][Reindeer.first] == EXIT_CHAR) {
        veryBestScore = std::min(veryBestScore, scoreRef);
        return 0;
    }
    // Interrupt if this path has a higher score than the best score
    if(scoreRef >= 0 && scoreRef >= veryBestScore) {
        return LLONG_MAX;
    }
    // Avoid repeating the same path
    if(visited.find(std::make_pair(Direction, Reindeer)) != visited.end()) {
        return -1;
    }
    visited.insert(std::make_pair(Direction, Reindeer));

    // Save the current state of visited
    auto visitedCopy = visited;

    long long bestScore = LLONG_MAX;
    char directions[] = {Direction, cw, ccw};
    for(auto d = 0; d < 3; d++) {
        long long score = -1;
        switch(Ahead(map, Reindeer, directions[d])) {
            case WALL_CHAR:
                score = -1;
                break;
            default:
                score = Walk(map, Move(Reindeer,directions[d]), directions[d], visited, scoreRef + (d == 0 ? 1 : 1001));
                if(score != -1) {
                    if(d == 0) {
                        score += 1;
                    } else {
                        score += 1001;
                    }
                }
                // Restore the visited state
                visited = visitedCopy;
        }
        if(score >= 0) {
            bestScore = std::min(bestScore, score);
        }
    }
    return bestScore;
}

long long MoveReindeer(std::vector<std::vector<char>> & map) {
    // Initialization
    std::pair<int,int> O = std::make_pair(1,1);
    std::pair<int,int> Z = std::make_pair(map[0].size()-2, map.size()-2);
    std::pair<int,int> Reindeer = std::make_pair(0,0);
    std::pair<int,int> MazeExit = std::make_pair(0,0);
    for(auto p = 0; p < map.size(); p++) {
        for(auto q = 0; q < map[p].size(); q++) {
            if(map[p][q] == REINDEER_CHAR) {
                Reindeer = std::make_pair(q,p);
            }
            if(map[p][q] == EXIT_CHAR) {
                MazeExit = std::make_pair(q,p);
            }
        }
    }
    char Direction = 'E';

    // Print the initial state
    std::cout << "Reindeer:" << Reindeer << std::endl;
    std::cout << "Map:  " << std::endl << map << std::endl;
    std::set<std::pair<char,std::pair<int,int>>> visited;
    long long score = Walk(map, Reindeer, Direction, visited,0);
    // std::cout << "Minimum score = " << score << std::endl;
    return score;
}

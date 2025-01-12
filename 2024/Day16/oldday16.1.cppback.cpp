#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include <filesystem>

#include "Maze.h"
#include "Compass.h"
// #include "Reindeer.h"

using namespace std;

#define REINDEER_CHAR 'S'
#define EXIT_CHAR 'E'
#define WALL_CHAR '#'
#define EMPTY_CHAR '.'

#define CHOICE_FWD 'F'
#define CHOICE_CLK 'C'
#define CHOICE_CCLK 'D'

#define BREADCRUMB_CHAR 'O'
#define DEADEND_CHAR 'X'
#define CHOICE_CHAR '?'

// Defines a limit for the max score to reach (e.g. size of the 1001*((map/2)^2))
// #define LIMIT_MAX_SCORE           LLONG_MAX
#define LIMIT_MAX_SCORE           5000000
// Defines a limit to avoid infinite loops (e.g. stops recursion if the same point is visited more than 10 times)
#define LIMIT_ANTI_INFINITELOOP   10

const char * banner = "AdventOfCode 2024 Day 16!";
const char * inputFilePath = "../../2024/Input/Day16.txt";

long long veryBestScore = LIMIT_MAX_SCORE;

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

struct Reindeer {
    std::pair<int,int> c = {0,0};   // Cursor Position
    char d = 'E';                   // Direction (N,E,S,W)
    long score = 0;                 // Current Score (turn = +100; step = +1)
    char choice = CHOICE_FWD;       // Keep track of the last choice (F,C,D)
};

/*
* Return the next direction when turning clockwise
* @param d The current direction (N,E,S,W)
* @return The next direction (N,E,S,W)
*/
inline char Clockwise(char d);
/*
* Return the next direction when turning counter clockwise
* @param d The current direction (N,E,S,W)
* @return The next direction (N,E,S,W)
*/
inline char CounterClockwise(char d);
/*
* Return the next cursor position when moving forward
* @param p The current cursor position
* @param d The current direction (N,E,S,W)
* @return The next cursor position
*/
inline std::pair<int,int> Move(std::pair<int,int> p, char d);
/*
* Return true if the cursor is on a wall
*/
inline bool IsWall(std::pair<int,int> p);
/*
* Return true if the cursor is on a free tile
*/
inline bool IsFree(std::pair<int,int> p);
/*
* Return true if the cursor is on the exit of the maze
*/
inline bool IsExit(std::pair<int,int> p);

/*
* Move the Reindeer forward
*/
inline void Forward();
/*
* Turn the Reindeer clockwise
*/
inline void TurnClockwise() ;
/*
* Turn the Reindeer counter clockwise
*/
inline void TurnCounterClockwise();

/*
* Get the tile in front of the Reindeer
*/
inline char GetTileForward();
/*
* Get the tile clockwise of the Reindeer
*/
inline char GetTileClockwise();
/*
* Get the tile counter clockwise of the Reindeer
*/
inline char GetTileCounterClockwise();

Reindeer reindeer = {std::make_pair(0,0), 'E', 0};
std::pair<int,int> mazeExit = std::make_pair(0,0);
std::vector<std::vector<char>> maze;
long bestScore = LONG_MAX;
std::vector<Reindeer> crosspoints;  // Keep track of the crosspoints (works like a stack)
std::vector<Reindeer> deadends;     // Keep track of the abandonned path, to avoid going back to them

std::stack<Reindeer> stepRestore;   // Empty, or one single element: the crosspoint where to restore from (when one of the step gets interrupted)
std::stack<Reindeer> mainRestore;   // Empty, or one single element: the crosspoint where to restore from (when the main loop gets interrupted)

bool Done() {
    return IsExit(reindeer.c);
}
int Choices() {
    int choices = 0;
    if(GetTileForward() != WALL_CHAR) {
        choices++;
    }
    if(GetTileClockwise() != WALL_CHAR) {
        choices++;
    }
    if(GetTileCounterClockwise() != WALL_CHAR) {
        choices++;
    }
    return choices;
}
bool Step() {
    int choices = Choices();
    if(Done()) {
        // We have reached the exit!
        return false;
    }
    // If the reindeer was restored from a previously visited crosspoint, we have to turn, and try the next direction
    // In case of success, we remove the crosspoint from the stack
    if(!stepRestore.empty()) {
        auto restored = stepRestore.top();
        stepRestore.pop();

        // Turn the reindeer
        switch(restored.choice) {
            case CHOICE_FWD:
                // We have tried Forward already, go clockwise
                TurnClockwise();
                break;
            case CHOICE_CLK:
                // We have tried Clockwise already, go counter clockwise
                TurnCounterClockwise();
                break;
            case CHOICE_CCLK:
                // We have tried all 3 directions, remove the crosspoint from the stack
                crosspoints.pop_back();

                if(crosspoints.empty()) {
                    // This cross point was the last one in the stack, no solution exists
                    reindeer.score = LONG_MAX;
                    return false;
                } else {
                    // Return to the previous cross point
                    reindeer = crosspoints.back();
                    stepRestore.push(reindeer);
                    return true;
                }
                break;
            default:
                throw std::runtime_error("Invalid choice state");
        }
    }
    // If the reindeer is on a crosspoint we have already tested, then we have to go back to the last crosspoint
    // ... unless if we are in the middle of a restore operation
    if(!stepRestore.empty()) {
        if(std::find_if(crosspoints.begin(), crosspoints.end(), [](const Reindeer& r) { return r.c == reindeer.c; }) != crosspoints.end()) {
            reindeer = crosspoints.back();
            stepRestore.push(reindeer);
            return true;
        }
    }
    // If the reindeer is blocked, then it is a dead end
    if(choices == 0) {
        if(!crosspoints.empty()) {
            // Return to last cross point
            reindeer = crosspoints.back();


            /***
             * ICI le restore à lieu... 
             */
            
            stepRestore.push(reindeer);
            return true;
        } else {
            // No more moves possible
            reindeer.score = LONG_MAX;
            return false;
        }
    }
    // Move the reindeer forward, or turn if it is blocked
    // If the reindeer has multiple choices, we also save the current crosspoint
    if(GetTileForward() != WALL_CHAR) {
        if(choices > 1) {
            auto it = std::find_if(crosspoints.begin(), crosspoints.end(), [](const Reindeer& r) { return r.c == reindeer.c; });
            if (it != crosspoints.end()) {
                it->choice = CHOICE_FWD;
            } else {
                auto temp = reindeer;
                temp.choice = CHOICE_FWD;
                crosspoints.push_back(temp);
            }
        }
        Forward();
        return true;
    } else if(GetTileClockwise() != WALL_CHAR) {
        if(choices > 1) {
            auto it = std::find_if(crosspoints.begin(), crosspoints.end(), [](const Reindeer& r) { return r.c == reindeer.c; });
            if (it != crosspoints.end()) {
                it->choice = CHOICE_CLK;
            } else {
                auto temp = reindeer;
                temp.choice = CHOICE_CLK;
                crosspoints.push_back(temp);
            }
        }
        TurnClockwise();
        return true;
    } else if(GetTileCounterClockwise() != WALL_CHAR) {
        if(choices > 1) {
            auto it = std::find_if(crosspoints.begin(), crosspoints.end(), [](const Reindeer& r) { return r.c == reindeer.c; });
            if (it != crosspoints.end()) {
                it->choice = CHOICE_CCLK;
            } else {
                auto temp = reindeer;
                temp.choice = CHOICE_CCLK;
                crosspoints.push_back(temp);
            }
        }
        TurnCounterClockwise();
        return true;
    } else {
        throw std::runtime_error("Invalid state: this appears to be a dead end, but it was not expected.");
    }
}

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl << std::endl;

    // Read input file
    {
        // Read input file
        std::ifstream inputFile(inputFilePath);
        if(!inputFile || !inputFile.is_open()) {
            std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
            return 1;
        }
        // Read the map of th maze
        std::string line;
        while(getline(inputFile, line)) {
            if(line.length() == 0) {
                break;
            }
            maze.push_back(std::vector<char>(line.begin(), line.end()));
        }
        inputFile.close();
    }
    // Initialization
    {
        reindeer.score = 0;
        reindeer.d = 'E';
        for(auto p = 0; p < maze.size(); p++) {
            for(auto q = 0; q < maze[p].size(); q++) {
                if(maze[p][q] == REINDEER_CHAR) {
                    reindeer.c = std::make_pair(q,p);
                }
                if(maze[p][q] == EXIT_CHAR) {
                    mazeExit = std::make_pair(q,p);
                }
            }
        }
    }
    
    // Print the Maze
    printCharMap(maze);

    // Move the Reindeer in the Maze
    {
        while(true) {
            if(reindeer.c.first == 1 && reindeer.c.second == 13) {
                std::cout << "Debug" << std::endl;
            }
            while(Step()) {
                // If the score is too high, abandon this track
                if(reindeer.score > bestScore) {
                    // Keep track of the dead end no to repeat the same mistake
                    if(!mainRestore.empty()) {
                        auto bck = mainRestore.top();
                        mainRestore.pop();

                        auto it = std::find_if(deadends.begin(), deadends.end(), [bck](const Reindeer& r) { return r.c == bck.c; });
                        if(it == deadends.end()) {
                            deadends.push_back(bck);
                        } else {
                            throw std::runtime_error("Invalid state: this dead end was already visited.");
                        }
                    }
                    break;
                }
            }
            bestScore = std::min(reindeer.score, bestScore);

            // are there any path we didn't explore?
            if(!crosspoints.empty()) {
                // Return to last cross point
                reindeer = crosspoints.back();
                // if the restored reindeer is found in the deadends, then we have to go back to the previous crosspoint
                while(!crosspoints.empty() && std::find_if(deadends.begin(), deadends.end(), [](const Reindeer& r) { return r.c == reindeer.c; }) != deadends.end()) {
                    crosspoints.pop_back();         // Remove the crosspoing from the stack
                    reindeer = crosspoints.back();  // Return to the previous crosspoint
                }
                // if the stack is empty, check if it is because we have explored all the paths or if we are at the very last one
                if(crosspoints.empty() && std::find_if(deadends.begin(), deadends.end(), [](const Reindeer& r) { return r.c == reindeer.c; }) != deadends.end()) {
                    // We have explored all the paths
                    break;
                }
                stepRestore.push(reindeer);
                mainRestore.push(reindeer);
                continue;
            }
            break;
        }
        
    }
    
    // Print the best score
    std::cout << "Best Score: " << bestScore << std::endl;

    return 0;
}


inline void Forward() {
    reindeer.c = Move(reindeer.c, reindeer.d);
    reindeer.score += 1;
}
inline void TurnClockwise() {
    reindeer.d = Clockwise(reindeer.d);
    reindeer.score += 1001;
}
inline void TurnCounterClockwise(){
    reindeer.d = CounterClockwise(reindeer.d);
    reindeer.score += 1001;
}
inline char GetTileForward(){
    std::pair<int,int> x = Move(reindeer.c,reindeer.d);
    return maze[x.second][x.first];
}
inline char GetTileClockwise(){
    std::pair<int,int> x = Move(reindeer.c,Clockwise(reindeer.d));
    return maze[x.second][x.first];
}
inline char GetTileCounterClockwise() {
    std::pair<int,int> x = Move(reindeer.c,CounterClockwise(reindeer.d));
    return maze[x.second][x.first];
}

inline bool IsWall(std::pair<int,int> p) {
    return maze[p.second][p.first] == WALL_CHAR;
}
inline bool IsFree(std::pair<int,int> p) {
    return maze[p.second][p.first] == EMPTY_CHAR;
}
inline bool IsExit(std::pair<int,int> p) {
    return maze[p.second][p.first] == EXIT_CHAR;
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
inline std::pair<int,int> Move(std::pair<int,int> c, char d) {
    switch(d) {
        case 'N': return std::make_pair(c.first, c.second - 1);
        case 'E': return std::make_pair(c.first + 1, c.second);
        case 'S': return std::make_pair(c.first, c.second + 1);
        case 'W': return std::make_pair(c.first - 1, c.second);
    }
    throw std::runtime_error("Invalid direction");
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
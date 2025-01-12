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

// ---------------------------------------------------------
// Color print a vector<char>
void printCharVector(const std::vector<char> & vec, std::ostream & os = std::cout);
// Color print a vector<vector<char>>
void printCharMap(const std::vector<std::vector<char>> & map, std::ostream & os = std::cout);
// Print a pair<int,int>
std::ostream& operator<<(std::ostream& os, const std::pair<int,int> & p);
// Print a vector<char>
std::ostream& operator<<(std::ostream& os, const std::vector<char> & vec);
// Print a vector<vector<char>>
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<char>> & map);

// ---------------------------------------------------------
struct Reindeer {
    std::pair<int,int> c = {0,0};   // Cursor Position
    Compass d = Compass::EAST;      // Direction (N,E,S,W)
    long score = 0;                 // Score
    
    // When storing in the stack, keep track of the number of choices made from there
    int choiceMade = 0;
    int maxChoices = 0;

    Reindeer() = default;
    Reindeer(const Reindeer& other) = default;
    Reindeer& operator=(const Reindeer& other) = default;
    

    operator std::pair<int, int>() const {
        return c;
    }
};

bool DoTheNextStep(const Maze & maze, Reindeer & reindeer, Reindeer & nextStep) {
    int firstChoice = -1;
    int firstChoiceScore = 0;
    int secondChoice = -1;
    int secondChoiceScore = 0;
    int thirdChoice = -1;
    int thirdChoiceScore = 0;
    reindeer.maxChoices = 0;
    if(maze.GetTileForward(reindeer.c, reindeer.d) != Maze::WALL_CHAR) {
        reindeer.maxChoices++;
        firstChoice = 0;
        firstChoiceScore = 1;
    }
    if(maze.GetTileClockwise(reindeer.c, reindeer.d) != Maze::WALL_CHAR) {
        reindeer.maxChoices++;
        if(firstChoice == -1) {
            firstChoice = 1;
            firstChoiceScore = 1001;
        }
        secondChoice = 1;
        secondChoiceScore = 1001;
    }
    if(maze.GetTileCounterClockwise(reindeer.c, reindeer.d) != Maze::WALL_CHAR) {
        reindeer.maxChoices++;
        if(firstChoice == -1) {
            firstChoice = 2;
            firstChoiceScore = 1001;
        }
        if(secondChoice == -1) {
            secondChoice = 2;
            secondChoiceScore = 1001;
        }
        thirdChoice = 2;
        thirdChoiceScore = 1001;
    }

    if(reindeer.maxChoices == 0) {
        // Reindeer is in a dead end
        return false;
    }
    if(reindeer.choiceMade > reindeer.maxChoices) {
        // Reindeer has tested all possible choices
        return false;
    }

    // Increment the counter of choices
    reindeer.choiceMade++;

    // Rotate as appropriate and update score accordingly
    nextStep = reindeer;
    if(reindeer.choiceMade == 3) {
        if(thirdChoice == 2) {
            nextStep.score += thirdChoiceScore;
            nextStep.d.CounterClockwise();
        }
    } else
    if(reindeer.choiceMade == 2) {
        if(secondChoice == 1) {
            nextStep.score += secondChoiceScore;
            nextStep.d.Clockwise();
        }
        if(secondChoice == 2) {
            nextStep.score += secondChoiceScore;
            nextStep.d.CounterClockwise();
        }
    } else {
        if(firstChoice == 0) {
            nextStep.score += firstChoiceScore;
            // No rotation
        }
        if(firstChoice == 1) {
            nextStep.score += firstChoiceScore;
            nextStep.d.Clockwise();
        }
        if(firstChoice == 2) {
            nextStep.score += firstChoiceScore;
            nextStep.d.CounterClockwise();
        }
    }
    if(reindeer.d == Compass::NORTH) {
        std::cout << "^";
    }
    if(reindeer.d == Compass::EAST) {
        std::cout << ">";
    }
    if(reindeer.d == Compass::SOUTH) {
        std::cout << "v";
    }
    if(reindeer.d == Compass::WEST) {
        std::cout << "<";
    }

    // Update the position of the nextStep accordingly (but not choiceMage and maxChoices!)
    nextStep.c = maze.Move(nextStep.c, nextStep.d);
    nextStep.choiceMade = 0;
    nextStep.maxChoices = 1;
    return true;
}

int CrosspointBranches(const Maze & maze, const Reindeer & reindeer) {
    int c = 0;
    if(maze.GetTileForward(reindeer.c, reindeer.d) != Maze::WALL_CHAR) {
        c++;
    }
    if(maze.GetTileClockwise(reindeer.c, reindeer.d) != Maze::WALL_CHAR) {
        c++;
    }
    if(maze.GetTileCounterClockwise(reindeer.c, reindeer.d) != Maze::WALL_CHAR) {
        c++;
    }
    return c;
}

std::stack<Reindeer> restoreInner;
std::stack<Reindeer> restoreOuter;
std::vector<Reindeer> crosspoints;
long MazeRunner(const Maze & maze) {
    static long bestScore = LONG_MAX;
    Reindeer reindeer = {maze.Start(), Compass::EAST, 0, 0, 0};

    // Let the reindeer run in the maze from start to the end
    // Exploring all path
    while(true) {
        // Explore one individual path
        while(true) {
            // Exit reached ?

            if(reindeer.c.first == 1 && reindeer.c.second == 13) {
                std::cout << "Reindeer " << reindeer.c << " -> " << reindeer.d << " (" << reindeer.choiceMade << ")" << std::endl;
            }
            if(maze.IsExit(reindeer)) {
                std::cout << std::endl << "EXIT: " << reindeer.score << std::endl;
                if(reindeer.score < bestScore) {
                    bestScore = reindeer.score;
                    std::cout << "BEST SCORE: " << bestScore << std::endl;
                }
                break;
            }

            // Handle Crosspoints
            if(CrosspointBranches(maze, reindeer) > 1) {
/*                if(reindeer.score > bestScore) {
                    // No need to explore further
                    std::cout << " !!! TOO Expensive, skip" << std::endl;
                    if(crosspoints.empty()) {
                        std::cout << "EXHAUSTED" << std::endl;
                        break;
                    }
                    reindeer = crosspoints.back();
                    crosspoints.pop_back();
                    std::cout << "POP " << reindeer.c << " " << reindeer.d << " score " << reindeer.score << std::endl;
                    continue;
                }
*/
                if(reindeer.choiceMade > reindeer.maxChoices) {
                    // All options exhausted
                }
                if(!crosspoints.empty()) {
                    auto it = std::find_if(crosspoints.begin(), crosspoints.end(), [&reindeer](const Reindeer& r) {
                        return r.c.first == reindeer.c.first && r.c.second == reindeer.c.second;
                    });
                    if (it != crosspoints.end()) {
                        // The cross point was already visited
                        Reindeer nextStep;
                        bool stepSuccess = DoTheNextStep(maze, *it, nextStep);
                        if(false == stepSuccess) {
                            reindeer = crosspoints.back();
                            crosspoints.pop_back();
                            std::cout << std::endl << "POP " << reindeer.c << " " << reindeer.d << " score " << reindeer.score << std::endl;
                            continue;
                        } else {
                            reindeer = nextStep;
                        }
                        // Next iteration
                        continue;
                    } else {
                        // The cross point is new, see below
                    }
                }
                // The cross point is new
                Reindeer nextStep;
                bool stepSuccess = DoTheNextStep(maze, reindeer, nextStep);
                if(false == stepSuccess) {
                    break;      // Dead end
                } else {
                    crosspoints.push_back(reindeer);    // Add to the stack of crosspoints
                    reindeer = nextStep;
                }
            } else {
                // Not a cross point
                Reindeer nextStep;
                bool stepSuccess = DoTheNextStep(maze, reindeer, nextStep);
                if(false == stepSuccess) {
                    break;      // Dead end
                } else {
                    reindeer = nextStep;
                }
            }
        }
        // We have reached the end (or a dead end), are there other path to explore?
        if(crosspoints.empty()) {
            // No, let's leave
            break;
        }
        // Yes, restore position at the previous cross point
        reindeer = crosspoints.back();
        // And go on
        continue;
    }

    return bestScore;
}

// ---------------------------------------------------------
const char * banner = "AdventOfCode 2024 Day 16!";
const char * inputFilePath = "../../2024/Input/Day16.txt";
int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl << std::endl;

    // Read the input file, and initialize the maze
    {
        std::vector<std::vector<char>> inputMap;
        // Read input file
        std::ifstream inputFile(inputFilePath);
        if(!inputFile || !inputFile.is_open()) {
            std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
            return 1;
        }
        // Read the map of the maze from the input file
        std::string line;
        while(getline(inputFile, line)) {
            if(line.length() == 0) {
                break;
            }
            inputMap.push_back(std::vector<char>(line.begin(), line.end()));
        }
        inputFile.close();

        // Load the maze from the input map
        Maze maze(inputMap);
        std::cout << MazeRunner(maze);
    }
}

// ---------------------------------------------------------

void printCharVector(const std::vector<char> & vec, std::ostream & os) {
    for(auto p = 0; p < vec.size(); p++) {
        switch(vec[p]) {
            case Maze::START_CHAR:
                os << "\033[1;32m" << vec[p] << "\033[0m";
                break;
            case Maze::EXIT_CHAR:
                os << "\033[1;33m" << vec[p] << "\033[0m";
                break;
            case Maze::WALL_CHAR:
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
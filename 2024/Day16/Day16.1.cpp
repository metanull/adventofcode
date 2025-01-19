#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include <filesystem>
#include <thread>

#include "Constant.h"

#include "Maze.h"
#include "Compass.h"
#include "MazeRunner.h"

#if !defined(NEW_METHOD)
    #define NEW_METHOD 0
#endif

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
const char * banner = "AdventOfCode 2024 Day 16!";
const char * inputFilePath = "../../2024/Input/Day16.txt";

int main(int argc, char ** argv, char ** envp) {
    std::cout << "\033[41;30;1m";
    std::cout << "################################################################################" << std::endl;
    std::cout << banner << std::endl;
    std::cout << "################################################################################" << "\033[0m" << std::endl << std::endl;

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
        MazeRunner runner(maze);

/*
        std::mutex mtx;
        std::condition_variable cv;
        bool turn = true; // true for thread1, false for thread2
        std::vector<MazeSegment> __store;
        auto customFunction1 = [&mtx, &cv, &turn, &__store](long long param1, const std::vector<MazeSegment> & param2) -> bool {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&turn] { return turn; });
            turn = false;
            cv.notify_all();

            __store = param2;

            // Implement your custom logic here
            return true; // or false based on your logic
        };
        auto customFunction2 = [&mtx, &cv, &turn, &__store](long long param1, const std::vector<MazeSegment> & param2) -> bool {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&turn] { return !turn; });
            turn = true;
            cv.notify_all();

            if(
                __store.size() != param2.size()
                || __store.empty() != param2.empty()
                || (!__store.empty() && __store.back().origin.position != param2.back().origin.position)
                || (!__store.empty() && __store.back().end.position != param2.back().end.position)
                || (!__store.empty() && __store.back().origin.direction != param2.back().origin.direction)
                || (!__store.empty() && __store.back().end.direction != param2.back().end.direction)
                || (!__store.empty() && __store.back().origin.score != param2.back().origin.score)
                || (!__store.empty() && __store.back().end.score != param2.back().end.score)
                || (!__store.empty() && __store.back().moves.size() != param2.back().moves.size())
                || (!__store.empty() && !__store.front().moves.empty() && __store.front().moves.front().position != param2.front().moves.front().position)
                || (!__store.empty() && !__store.front().moves.empty() && __store.front().moves.front().direction != param2.front().moves.front().direction)
                || (!__store.empty() && !__store.front().moves.empty() && __store.front().moves.front().score != param2.front().moves.front().score)
                || (!__store.empty() && !__store.back().moves.empty() && __store.back().moves.back().position != param2.back().moves.back().position)
                || (!__store.empty() && !__store.back().moves.empty() && __store.back().moves.back().direction != param2.back().moves.back().direction)
                || (!__store.empty() && !__store.back().moves.empty() && __store.back().moves.back().score != param2.back().moves.back().score)
            ) {
                std::cout << "\033[22;31;7;1m" << "Mismatch" << "\033[0m" << std::endl;
                auto x = param2;

                if(!x.empty()) {
                    std::cout 
                        << "\033[22;32;27;1m"
                        << std::setw(12) << std::setfill(' ') << param1 << " - Thread 2 (old) - "
                        << "(" << std::setw(3) << x.back().origin.position.first << ","
                        << std::setw(3) << x.back().origin.position.second << " - "
                        << x.back().origin.direction << " - "
                        << std::setw(9) << x.back().origin.score << ")"
                        << " ->"
                        << "(" << std::setw(3) << x.back().end.position.first << ","
                        << std::setw(3) << x.back().end.position.second << " - "
                        << x.back().end.direction << " - "
                        << std::setw(9) << x.back().end.score << ") - ";
                    if(!x.back().moves.empty()) {
                        for(auto m : x.back().moves) {
                            std::cout << m.position.first << "," << m.position.second << ";" << m.direction << ";" << m.score << " / ";
                        }
                    }
                    std::cout << "\033[0m" << std::endl;
                }
                x = __store;
                if(!x.empty()) {
                    std::cout 
                        << "\033[22;32;7;1m"
                        << std::setw(12) << std::setfill(' ') << param1 << " - Thread 1 (new) - "
                        << "(" << std::setw(3) << x.back().origin.position.first << ","
                        << std::setw(3) << x.back().origin.position.second << " - "
                        << x.back().origin.direction << " - "
                        << std::setw(9) << x.back().origin.score << ")"
                        << " ->"
                        << "(" << std::setw(3) << x.back().end.position.first << ","
                        << std::setw(3) << x.back().end.position.second << " - "
                        << x.back().end.direction << " - "
                        << std::setw(9) << x.back().end.score << ") - ";
                    if(!x.back().moves.empty()) {
                        for(auto m : x.back().moves) {
                            std::cout << m.position.first << "," << m.position.second << ";" << m.direction << ";" << m.score << " / ";
                        }
                    }
                    std::cout << "\033[0m" << std::endl;
                }
            }

            // Implement your custom logic here
            return true; // or false based on your logic
        };

        std::thread thread1([&runner, &customFunction1]() {
            std::cout << runner.Run(customFunction1) << std::endl;
        });

        std::thread thread2([&runner, &customFunction2]() {
            std::cout << runner.Run3(customFunction2) << std::endl;
        });

        thread1.join();
        thread2.join();
*/

        #if defined(NEW_METHOD) && NEW_METHOD != 0
            std::cout << runner.Run() << std::endl;
        #else
            std::cout << runner.Run3() << std::endl;
        #endif
        
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
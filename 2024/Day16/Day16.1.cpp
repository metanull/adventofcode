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
#include "MazeRunner.h"
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
        MazeRunner runner(maze);

        runner.Run([](std::stack<MazeSegment> segments, long score) {
            std::cout << "\033[42;30;1m";
            std::cout << "== EXIT REACHED ==" << std::endl;
            std::cout << "== Score: " << score << std::endl;
            std::cout << "== Path: " << std::endl;

            Compass prev = Compass::EAST;   // Tracks the previous direction

            // transform the segment stack into a vector, in reverse order
            std::vector<MazeRunnerPosition> path;
            while(!segments.empty()) {
                if(segments.top().origin.position == segments.top().end.position && segments.top().moves.empty()) {
                    // The very first segment is empty (containing the initial direction)
                    prev = segments.top().origin.direction;
                    segments.pop();
                    continue;
                }
                while(!segments.top().moves.empty()) {
                    path.push_back(segments.top().moves.back());
                    segments.top().moves.pop_back();
                }
                segments.pop();
            }
            std::reverse(path.begin(), path.end());
            // print the path
            long turns = 0;
            long steps = 0;
            
            for(auto move : path) {
                if(move.direction != prev) {
                    turns ++;
                    prev = move.direction;
                    std::cout << "TURN ";
                }
                steps ++;
                std::cout << move.direction << " ";
            }
            std::cout << std::endl;
            std::cout << "CALCULATED SCORE: " << ((turns * 1001) + steps - turns) << std::endl;
            std::cout << "==================\033[0m" << std::endl;
        });

        
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
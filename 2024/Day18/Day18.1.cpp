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

#include "charmap.h"

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
const char * banner = "AdventOfCode 2024 Day 18!";
const char * inputFilePath = INPUT_PATH;

int main(int argc, char ** argv, char ** envp) {
    
    std::vector<std::string> args(argv, argv+argc);
    if(args.size() == 1) {
        args.push_back(inputFilePath);
    }

    std::cout << "\033[7;34;1m";
    std::cout << "################################################################################" << std::endl;
    std::cout << std::setw(80) << banner << std::endl;
    std::cout << "################################################################################" << "\033[0m" << std::endl << std::endl;

    // Read the input file, and initialize the maze
    {
        std::vector<std::vector<char>> inputMap;
        // Read input file
        std::ifstream inputFile(args[1]);
        if(!inputFile || !inputFile.is_open()) {
            std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
            return 1;
        }
        // Read the list of corrupted blocks
        std::vector<std::pair<int,int>> blocks;
        std::string line;
        while(getline(inputFile, line)) {
            std::istringstream iss(line);
            long x = 0, y = 0;
            iss >> x;
            iss.ignore(1, ',');
            iss >> y;
            blocks.push_back(std::make_pair(x,y));
        }
        inputFile.close();

        // Load the maze from the input map
        Maze maze;
        std::vector<std::pair<int,int>> selection(blocks);
        selection.resize(1024);
        MazeRunner runner(maze.NewMazeFromBlocks(selection,71,71));

        auto m1 = maze.GetMap();
        auto m2 = maze.RemoveDeadEnds();

        for(int y = 0; y < m1.size(); y ++) {
            std::stringstream ss;
            for(int x = 0; x < m1[0].size(); x ++) {
                if(m1[y][x] == m2[y][x]) {
                    ss << "\033[27m";
                } else {
                    ss << "\033[7m";
                }
                switch(m2[y][x]) {
                    case Maze::WALL_CHAR:   ss << "\033[33m";   break;
                    case Maze::START_CHAR:  ss << "\033[32m";   break;
                    case Maze::EXIT_CHAR:   ss << "\033[35m";   break;
                    default:                ss << "\033[22m";
                }
                ss << m2[y][x];
            }
            std::cout << ss.str() << std::endl;
        }
        

        std::function<std::vector<Compass>(const std::vector<Compass>&)> regular = [](const std::vector<Compass> & options) {
            return options;
        };
        std::function<std::vector<Compass>(const std::vector<Compass>&)> reverse = [](const std::vector<Compass> & options) {
            std::vector<Compass> result;
            for(auto p = options.rbegin(); p != options.rend(); p++) {
                result.push_back(*p);
            }
            return result;
        };
        std::function<std::vector<Compass>(const std::vector<Compass>&)> customOrder = [](const std::vector<Compass> & options) {
            std::vector<Compass> result = options;
            if (options.size() == 3) {
                std::swap(result[0], result[1]);
            }
            return result;
        };
        std::function<std::vector<Compass>(const std::vector<Compass>&)> customOrderReverse = [](const std::vector<Compass> & options) {
            std::vector<Compass> result;
            for(auto p = options.rbegin(); p != options.rend(); p++) {
                result.push_back(*p);
            }
            if (options.size() == 3) {
                std::swap(result[0], result[1]);
            }
            return result;
        };
        
        #if defined(MULTITHREAD) && MULTITHREAD == 1
            std::cout << "\033[7;31;1m*** Multi-threaded execution ***" << std::endl;

            // Run the logic twice in parallel; each processing the crosspoint in a different order of preference (regular (FWD,CW,CCW) and reverse (CCW,CW,FWD))
            long resultRegular = 0;
            std::thread thread1([&runner, &regular, &resultRegular]() {
                resultRegular = runner.Run("NORMAL", regular);
            });

            long resultReverse = 0;
            std::thread thread2([&runner, &reverse, &resultReverse]() {
                resultReverse = runner.Run("REVERSE", reverse);
            });
            
            long resultCustomOrder = 0;
            // std::this_thread::sleep_for(std::chrono::seconds(3));   // Wait a bit before launching the third thread
            std::thread thread3([&runner, &customOrder, &resultCustomOrder]() {
                resultCustomOrder = runner.Run("CUSTOM", customOrder);
            });

            long resultCustomOrderReverse = 0;
            // std::this_thread::sleep_for(std::chrono::seconds(3));   // Wait a bit before launching the fourth thread
            std::thread thread4([&runner, &customOrderReverse, &resultCustomOrderReverse]() {
                resultCustomOrderReverse = runner.Run("CUSTOM-R", customOrderReverse);
            });

            /*std::cout
                << "\033[32mRegular:               " << std::setfill(' ') << std::setw(9) << thread1.get_id() << std::endl
                << "\033[33mReverse:               " << std::setfill(' ') << std::setw(9) << thread2.get_id() << std::endl
                << "\033[34mCustom Order:          " << std::setfill(' ') << std::setw(9) << thread3.get_id() << std::endl
                << "\033[35mCustom Order Reverse:  " << std::setfill(' ') << std::setw(9) << thread4.get_id() << std::endl
                << "\033[0m";
            */
            thread1.join();
            thread2.join();
            thread3.join();
            thread4.join();

            std::cout 
                << "\033[32mResult (regular order):        " << std::setw(9) << resultRegular << std::endl
                << "\033[32mResult (reverse order):        " << std::setw(9) << resultReverse << std::endl
                << "\033[32mResult (custom order):         " << std::setw(9) << resultCustomOrder << std::endl
                << "\033[32mResult (custom order reverse): " << std::setw(9) << resultCustomOrderReverse << std::endl
                << "\033[0m";
        #else
            std::cout << "Single-threaded execution" << std::endl;
            long result = runner.Run("SINGLE-THREAD", regular);
            std::cout << "Result: " << result << std::endl;
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
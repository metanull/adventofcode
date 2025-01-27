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
#include <chrono>
#include <set>

#include "Constant.h"

#include "charmap.h"
#include "math.h"

// ---------------------------------------------------------
const char *banner = "AdventOfCode 2024 Day 18!";
const char *inputFilePath = INPUT_PATH;

int main(int argc, char **argv, char **envp)
{

    std::vector<std::string> args(argv, argv + argc);
    if (args.size() == 1)
    {
        // No input file provided, use the default one
        args.push_back(inputFilePath);
    }
    size_t banner_width = 80;
    std::cout << "\033[7;34;1m" << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << std::setw((banner_width - strlen(banner)) / 2) << '#'
              << banner
              << std::setw(((banner_width - strlen(banner)) / 2) + ((banner_width - strlen(banner)) % 2)) << '#' << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << "\033[0m" << std::endl
              << std::endl;

    metanull::charmap::position start = {0,0};
    metanull::charmap::position end = {70,70};
    metanull::charmap::map inputMap(end.second +1, std::vector<char>(end.first +1, '.'));
    // Read the input file
    {
        std::vector<std::pair<int,int>> blocks;
        std::ifstream inputFile(args[1]);
        if (!inputFile || !inputFile.is_open())
        {
            std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
            return 1;
        }
        
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
        for(size_t b = 0; b < blocks.size() && b < 1024; b++) {
            inputMap[blocks[b].second][blocks[b].first] = '#';
        }
    }
    
    std::cout << "Input map:" << inputMap << std::endl;
    std::cout << "Start: " << start.first << "," << start.second << std::endl;
    std::cout << "End: " << end.first << "," << end.second << std::endl;

    for(auto b : metanull::charmap::maze_find_first_best_path(inputMap, start, end, metanull::charmap::EAST, metanull::charmap::basic_score_move, metanull::charmap::is_tile_free)) {
        std::cout << "Best path: " << b << std::endl;
        metanull::charmap::maze_print_path(std::cout, inputMap, start, end, b, metanull::charmap::is_tile_free);
    }
    return 0;
}

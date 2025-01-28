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
#include <map>

#include "Constant.h"

#include "charmap.h"
#include "math.h"

// ---------------------------------------------------------
const char *banner = "AdventOfCode 2024 Day 20!";
const char *inputFilePath = INPUT_PATH;

int main(int argc, char **argv, char **envp)
{
    size_t diamond_size = 41;
    metanull::charmap::map diamond_map;;
    std::vector<metanull::charmap::position> diamond;

    diamond_map.emplace_back(diamond_size, '#');  
    for(size_t drow = (diamond_size - 1) /2 - 1; drow != SIZE_MAX; drow --) {
        size_t empty = (diamond_size - (drow * 2));
        size_t full = diamond_size - empty + 1;
        metanull::charmap::row row(diamond_size, '.');
        std::fill(row.begin() + empty / 2, row.begin() + empty / 2 + full, '#');
        if(full == diamond_size) {
            diamond_map.push_back(row);    
        } else {
            diamond_map.insert(diamond_map.begin(), row);
            diamond_map.push_back(row);
        }
    }
    diamond = metanull::charmap::char_find(diamond_map, '#');

//    std::cout << "Diamond map:" << diamond_map << std::endl;
//    return 0;
    


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

    metanull::charmap::map inputMap;
    // Read the input file, and initialize the maze
    // Read input file
    std::ifstream inputFile(args[1]);
    if (!inputFile || !inputFile.is_open())
    {
        std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return 1;
    }
    inputFile >> inputMap;
    inputFile.close();

    // std::cout << "Input map:" << inputMap << std::endl;
    auto start = metanull::charmap::char_find(inputMap, 'S').front();
    auto end = metanull::charmap::char_find(inputMap, 'E').front();
    std::cout << "Start: " << start.first << "," << start.second << std::endl;
    std::cout << "End: " << end.first << "," << end.second << std::endl;

    auto b = metanull::charmap::maze_find_first_best_path(inputMap, start, end, metanull::charmap::EAST, metanull::charmap::basic_score_move, metanull::charmap::is_tile_free);
    if(b.empty()) {
        std::cerr << "No path found" << std::endl;
        return 1;
    } else {
        std::cout << b.front() << std::endl;
    }
    // metanull::charmap::maze_print_path(std::cout, inputMap, start, end, b, metanull::charmap::is_tile_free);
/*
      *
     ***
    *****
   *******
  *********  
 ***********
******O******
 ***********
  *********
   ******* 
    *****
     ***
      *
*/

   
    static const size_t MIN_GAIN = 100 + 2;
    for(auto iv = b.front().visited.begin(); iv < b.front().visited.end() - (MIN_GAIN); iv++) {
        // Diamond shape, 4 directions, up to 20 steps from the center, the center is the current position
        std::vector<std::pair<metanull::charmap::position,metanull::charmap::position>> s;
        std::vector<metanull::charmap::direction> dir = {
            metanull::charmap::NORTH,
            metanull::charmap::SOUTH,
            metanull::charmap::EAST,
            metanull::charmap::WEST
        };

    }

    return 0;
}

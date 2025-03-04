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

/**
 * @brief Create a charmap filled with a diamond shape.
 * @param radius The number of cells, from the center of the diamond.
 * @param empty The character to use for 'empty' characters
 * @param full The character to use for the body of the diamond
 * @return A charmap.
 * @example 
 *  auto d = make_diamond(2,'.','#');
 * 
 *  // Result:
 *  //
 *  // ..#..
 *  // .###.
 *  // #####
 *  // .###.
 *  // ..#..
 */
metanull::charmap::map make_diamond(size_t radius = 0, char empty = '.', char full = '#');

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

    metanull::charmap::map inputMap;
    
    // Read the input file, and initialize the maze
    std::ifstream inputFile(args[1]);
    if (!inputFile || !inputFile.is_open())
    {
        std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return 1;
    }
    inputFile >> inputMap;
    inputFile.close();

    // Find Start and End positions
    auto start = metanull::charmap::char_find(inputMap, 'S').front();
    auto end = metanull::charmap::char_find(inputMap, 'E').front();
    std::cout << "Start: " << start.first << "," << start.second << std::endl;
    std::cout << "End: " << end.first << "," << end.second << std::endl;

    // Solve the maze - look for the best path (there is only one for this exercise)
    auto b = metanull::charmap::maze_find_first_best_path(inputMap, start, end, metanull::charmap::EAST, metanull::charmap::basic_score_move, metanull::charmap::is_tile_free);
    if(b.empty()) {
        std::cerr << "No path found" << std::endl;
        return 1;
    } else {
        std::cout << b.front() << std::endl;
    }

    // Print the solved maze
    // metanull::charmap::maze_print_path(std::cout, inputMap, start, end, b, metanull::charmap::is_tile_free);


    // Look for cheats
    const size_t CHEAT_SIZE = 20;
    const size_t CHEAT_LEAST_ACCEPTABLE_GAIN = 100;
    const char CHEAT_EMPTY_CHAR = '#';
    const char CHEAT_FULL_CHAR = '.';
    static const size_t MIN_GAIN = 0;

    // Define a Diamond shape of the size of our cheat; the diamond represents the positions our cheat is able to reach
    size_t diamond_size = 1 + ((CHEAT_SIZE) * 2);
    metanull::charmap::position diamond_center = {diamond_size/2,diamond_size/2};
    metanull::charmap::map diamond_map = make_diamond(CHEAT_SIZE, CHEAT_EMPTY_CHAR, CHEAT_FULL_CHAR);
    // Retain only the positions of the diamonds cells (those = to CHEAT_FULL_CHAR)
    std::vector<metanull::charmap::position> diamond = metanull::charmap::char_find(diamond_map, CHEAT_FULL_CHAR);
    /*
    // Print the Diamond
    std::cout << "Diamond map:" << std::endl << diamond_map << std::endl;
    */
    
    std::set<std::pair<metanull::charmap::position,metanull::charmap::position>> cheats;  // A list of the unique cheats identified {[start;end]}
    std::map<size_t,size_t> cheats_score;  // A list of the score + count occurrences for each cheat
    // Browse the Solution step by step
    for(auto iv = b.front().visited.begin(); iv < b.front().visited.end() /*- (MIN_GAIN + 2)*/; iv++) {
        // Fore each step, add the coordinates in the diamond
        // > test if the resulting coordinates are on an empty cell
        // > test if the resulting coordinates are AFTER the current step (as otherwise it is a handicap rather than a cheat)
        // > count how many steps we have actually skipped using that cheat
        // > save the cheat (score), and count cheats with the same score
        for(auto id = diamond.begin(); id < diamond.end(); id ++) {
            metanull::charmap::position cheat_start = *iv;
            metanull::charmap::position cheat_end = {id->first + iv->first - diamond_center.first, id->second + iv->second - diamond_center.second};
            try {
                // Cheat valid if end is a free tile (including the exit point of the maze)
                if(!metanull::charmap::is_tile_free(inputMap,cheat_end,end)) {
                    // Not a cheat (tile is not free)
                    continue;
                }
            } catch(std::out_of_range) {
                // Not a cheat (out of range)
                continue;
            }
            
            // Cheat valid if it actually is a shortcut compared to the original path
            auto where_was_it = std::find(b.front().visited.begin(), b.front().visited.end(), cheat_end);
            if(where_was_it == b.front().visited.end()) {
                // This case shall NEVER occure
                throw std::out_of_range("Couldn't find the visited point");
            }
            
            auto original_distance = std::distance(iv, where_was_it);
            if(original_distance < 0) {
                // distance is <0 => this is not a cheat as we ended up before the current position
                continue;
            }
            
            auto cheat_distance = (std::abs((long long)(diamond_center.first - id->first)) + std::abs((long long)(diamond_center.second - id->second)));
            if(cheat_distance < 0) {
                // distance is <0 => this is not a cheat as we ended up before the current position
                continue;
            } 
            
            if(cheat_distance >= original_distance) {
                // distance is greater or equal to the original distance => this is nto a cheat
                continue;
            }

            // Save the cheat, and count occurrences of cheats by score
            auto score = original_distance - cheat_distance;
            auto cheat_pair = std::make_pair(cheat_start,cheat_end);
            // Avoid counting the same cheat (same start, same end) twice; we add it to a "set"; if the set doesn't grow, it means that the position was already present in it
            auto cheat_size = cheats.size();
            cheats.emplace(cheat_pair);
            if(cheats.size()>cheat_size) {
                cheats_score[score] ++;
            }
        }
    }

    // Sort the list of cheats by their score
    std::vector<std::pair<size_t, size_t>> vec(cheats_score.begin(), cheats_score.end());
    std::sort(vec.begin(), vec.end(), [](const auto &lhs, const auto &rhs){ return lhs.first < rhs.first;});
    
    // Print the cheats that provide enough value (see CHEAT_LEAST_ACCEPTABLE_GAIN)
    // Calculate the total number of cheats that provide enough value
    size_t count_better_than_least = 0;
    for(const auto & score : vec) {
        if(score.first >= CHEAT_LEAST_ACCEPTABLE_GAIN) {
            std::cout << "Score: " << score.first << " - " << score.second << " occurrence(s)." << std::endl;
            count_better_than_least += score.second;
        }
    }
    // Print the total number of cheats providing enough value
    std::cout << count_better_than_least << std::endl;

    return 0;
}

metanull::charmap::map make_diamond(size_t radius, char empty_char, char full_char) {
    size_t diamond_size = 1 + (radius * 2);
    metanull::charmap::map diamond_map;;
    
    diamond_map.emplace_back(diamond_size, full_char);  
    for(size_t drow = (diamond_size - 1) /2 - 1; drow != SIZE_MAX; drow --) {
        size_t empty = (diamond_size - (drow * 2));
        size_t full = diamond_size - empty + 1;
        metanull::charmap::row row(diamond_size, empty_char);
        std::fill(row.begin() + empty / 2, row.begin() + empty / 2 + full, full_char);
        if(full == diamond_size) {
            diamond_map.push_back(row);    
        } else {
            diamond_map.insert(diamond_map.begin(), row);
            diamond_map.push_back(row);
        }
    }
    return diamond_map;
}

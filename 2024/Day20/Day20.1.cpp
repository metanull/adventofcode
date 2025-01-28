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
    
    std::vector<std::pair<size_t,std::pair<metanull::charmap::position,metanull::charmap::position>>> cheats;
    std::vector<metanull::charmap::direction> dir = {metanull::charmap::NORTH, metanull::charmap::EAST, metanull::charmap::SOUTH, metanull::charmap::WEST};
    static const size_t MIN_GAIN = 100 + 2;
    for(auto iv = b.front().visited.begin(); iv < b.front().visited.end() - (MIN_GAIN); iv++) {
        std::vector<std::pair<metanull::charmap::position,metanull::charmap::position>> s;

        for(auto d : dir) {
            auto p = metanull::charmap::translate(*iv, d);
            auto q = metanull::charmap::translate(p, d);
            s.push_back(std::make_pair(p,q));
        }

        for (auto item : s) {
            auto ifrom = b.front().visited.begin();
            ifrom += std::distance(b.front().visited.begin(), iv);
            ifrom += MIN_GAIN;
            auto ito = b.front().visited.end();

            auto ishortcut = std::find(ifrom, ito, item.second);
            if(ishortcut != ito) {
                bool valid_shortcut = metanull::charmap::is_tile_free(inputMap, item.second, end) &&  !metanull::charmap::is_tile_free(inputMap, item.first, end);
                /*std::cout << "iv             = " << metanull::charmap::position_as_string(*iv) << " = " << metanull::charmap::access(inputMap, *iv) << std::endl;
                std::cout << "item.first     = " << metanull::charmap::position_as_string(item.first) << " = " << metanull::charmap::access(inputMap, item.first) << std::endl;
                std::cout << "item.second    = " << metanull::charmap::position_as_string(item.second) << " = " << metanull::charmap::access(inputMap, item.second) << std::endl;
                std::cout << "distance       = " << std::distance(iv, ishortcut) << std::endl;
                std::cout << "distance >0    = " << ishortcut - iv << std::endl;
                std::cout << "valid_shortcut = " << valid_shortcut << std::endl;
                */
                if(valid_shortcut) {
                    cheats.push_back(std::make_pair(std::distance(iv, ishortcut) - 2, std::make_pair(item.first, item.second)));
                    /*std::cout << "Found shortcut between " 
                        << metanull::charmap::position_as_string(*iv) 
                        << " and " 
                        << metanull::charmap::position_as_string(item.second)
                        << " at "
                        << metanull::charmap::position_as_string(item.first)
                        << " gain "
                        << std::distance(iv, ishortcut) - 2
                        << std::endl;
                        */
                    /*
                    auto verif = inputMap;
                    metanull::charmap::access(verif, item.first) = '.';
                    auto bverif = metanull::charmap::maze_find_first_best_path(verif, start, end, metanull::charmap::EAST, metanull::charmap::basic_score_move, metanull::charmap::is_tile_free);
                    std::cout << b.front().score << " - " << bverif.front().score << " = " << (std::distance(iv, ishortcut) - 2)<< std::endl;
                    metanull::charmap::maze_print_path(std::cout, verif, start, end, bverif, metanull::charmap::is_tile_free);
                    */
                }
            }
        }
/*
        for(auto c : s) {
            if (std::distance(iv, b.front().visited.end()) > 100) {
                auto f = std::find(iv + 100, b.front().visited.end(), c);
                if (f != b.front().visited.end()) {
                    auto mid = *f;
                    mid.first = iv->first + (f->first - iv->first)/2;
                    mid.second = iv->second + (f->second - iv->second)/2;
                    std::cout << "Found a shortcut (" << std::distance(iv, f) << ") in: " << metanull::charmap::position_as_string(*iv) << " & " << metanull::charmap::position_as_string(mid) << " & " << metanull::charmap::position_as_string(*f) << std::endl;
                }
            }
        }
        */
    }

    std::sort(cheats.begin(), cheats.end(), [](const auto &a, const auto &b) {
        return a.first > b.first;
    });
    std::map<size_t, size_t> cheat_counts;
    for (const auto &cheat : cheats) {
        cheat_counts[cheat.first]++;
    }
    /*
    for (const auto &entry : cheat_counts) {
        std::cout << "Gain: " << entry.first << " has " << entry.second << " occurrences." << std::endl;
    }
    */
    /*size_t total_gain = 0;
    for (const auto &entry : cheat_counts) {
        total_gain += entry.first * entry.second;
    }
    std::cout << "Total gain: " << total_gain << std::endl;
    */
   size_t total_cheats = 0;
    for (const auto &entry : cheat_counts) {
        total_cheats += entry.second;
    }
    std::cout << "Total cheats: " << total_cheats << std::endl;
    
    return 0;
}

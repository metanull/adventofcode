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

#include "charmap.h"
#include "math.h"

// ---------------------------------------------------------
const char * banner = "AdventOfCode 2024 Day 16!";
const char * inputFilePath = INPUT_PATH;

struct node {
    size_t score;
    std::pair<metanull::charmap::index,metanull::charmap::direction> start;
    std::pair<metanull::charmap::index,metanull::charmap::direction> end;
    std::vector<metanull::charmap::index> path;
    bool closed = false;
};

std::vector<node> next_nodes(const metanull::charmap::map & map, const node & prev_node) {
    std::vector<node> nodes;
    node cur_node = prev_node;

    while(true) {
        auto n = metanull::charmap::neighbours_if(map,cur_node.end.first,[cur_node](char c, metanull::charmap::direction d) { 
            return (c == '.' || c == 'E') && (d != metanull::charmap::inverse(cur_node.end.second)) && (d == metanull::charmap::NORTH || d == metanull::charmap::SOUTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST );
        });
        if(n.empty()) {
            // Dead end, forget this node
            return {};
        }
        if(n.size() == 1) {
            if(metanull::charmap::access(map, cur_node.end.first) == 'E') {
                // That's the exit, return the current node
                cur_node.path.emplace_back(cur_node.end.first);
                cur_node.closed = true;
                return {cur_node};
            } else {
                // Continue with the only option
                cur_node.path.push_back(cur_node.end.first);
                // Update the score
                if(cur_node.end.second != n[0].first) {
                    cur_node.score += SCORE_TURN;
                }
                cur_node.score += SCORE_STEP;

                cur_node.end.first = metanull::charmap::translate(cur_node.end.first, n[0].first);
                cur_node.end.second = n[0].first;

                continue;
            }
        } else {
            // Multiple options close the node and return
            for(auto opt : n) {
                node new_node = cur_node;

                new_node.path.push_back(new_node.end.first);
                // Update the score
                if(new_node.end.second != opt.first) {
                    new_node.score += SCORE_TURN;
                }
                new_node.score += SCORE_STEP;

                new_node.end.first = metanull::charmap::translate(new_node.end.first, opt.first);
                new_node.end.second = opt.first;
                new_node.closed = false;
                nodes.push_back(new_node);
            }
            return nodes;
        }
    }
}
int main(int argc, char ** argv, char ** envp) {

    std::vector<std::string> args(argv, argv+argc);
    if(args.size() == 1) {
        // No input file provided, use the default one
        args.push_back(inputFilePath);
    }
    size_t banner_width = 80;
    std::cout << "\033[7;34;1m" << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << std::setw((banner_width - strlen(banner)) / 2) << '#'
              << banner
              << std::setw(((banner_width - strlen(banner)) / 2)+((banner_width - strlen(banner)) % 2)) << '#' << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << "\033[0m" << std::endl << std::endl;

    // Read the input file, and initialize the maze
    {
        metanull::charmap::map inputMap;
        // Read input file
        std::ifstream inputFile(args[1]);
        if(!inputFile || !inputFile.is_open()) {
            std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
            return 1;
        }
        inputFile >> inputMap;
        inputFile.close();

        std::cout << "Input map:" << inputMap << std::endl;
        auto start = metanull::charmap::char_find(inputMap, 'S').front();
        auto end = metanull::charmap::char_find(inputMap, 'E').front();
        std::cout << "Start: " << start.first << "," << start.second << std::endl;
        std::cout << "End: " << end.first << "," << end.second << std::endl;

        node start_node = {0, {start, metanull::charmap::EAST}, {start, metanull::charmap::EAST}, {}};
        auto options = metanull::charmap::neighbours_if(inputMap, start, [](char c, metanull::charmap::direction d) { return c != '#'; });
        std::cout << "Options: " << options.size() << std::endl;
        for(auto p = 0; p < options.size(); p++) {
            std::cout << options[p].first.first << "," << options[p].first.second << " = " << options[p].second << std::endl;
        }
        if(options.empty()) {
            std::cerr << "No valid path found in the map" << std::endl;
            return 1;
        }
        std::vector<node> open_nodes;
        std::vector<node> closed_nodes;
        for(auto o : options) {
            node c = start_node;
            if(o.first != c.start.second) {
                c.score += SCORE_TURN;
                c.end.second = o.first;
            }
            c.score += SCORE_STEP;
            c.end.first = metanull::charmap::translate(c.end.first, c.end.second);
            open_nodes.push_back(c);
        }

        for(auto o : open_nodes) {
            auto next = next_nodes(inputMap, o);
            std::cout << "Next nodes: " << next.size() << std::endl;
            for(auto p : next) {
                std::cout << "Node (" << p.start.first.first << ',' <<  p.start.first.second << " -> " << p.end.first.first << ',' <<  p.end.first.second << ") = " << p.score << " " << (p.closed ? "CLOSED" : "OPEN") << std::endl;
                auto kp = p.path.front();
                for(auto k : p.path) {
                    if(k.second != kp.second ) {
                        if(k.second > 0) {
                            std::cout << " vvv ";
                        } else {
                            std::cout << " ^^^ ";
                        }
                    } else {
                        if(k.first != kp.first ) {
                            if(k.first > 0) {
                                std::cout << " >>> ";
                            } else {
                                std::cout << " <<< ";
                            }
                        } else {
                            std::cout << " ??? ";
                        }
                    }
                    std::cout << k.first << ',' << k.second << std::endl;
                    kp = k;
                }
                std::cout << std::endl;
            }
        }
    }
}

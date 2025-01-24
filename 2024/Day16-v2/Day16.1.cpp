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

#include "maze.h"

// ---------------------------------------------------------
const char * banner = "AdventOfCode 2024 Day 16!";
const char * inputFilePath = INPUT_PATH;

int main(int argc, char ** argv, char ** envp) {

maze m = maze(metanull::charmap::map{
    {'S','.','.','.','E'},
    {'#','#','.','#','#'},
    {'#','#','.','#','#'},
}, 'S', 'E', metanull::charmap::EAST);
maze_node current_node;
current_node.start = metanull::charmap::position{0,0};
current_node.start_direction = metanull::charmap::EAST;
current_node.end = metanull::charmap::position{1,0};
current_node.end_direction = metanull::charmap::EAST;
current_node.score = 1;
current_node.visited.push_back(metanull::charmap::position{0,0});
current_node.closed = false;

auto next_nodes = m.next_nodes_from(current_node);
std::cerr << "Expected result: 2 open node {0,0}->{3,0},score=3; {0,0}->{2,1},score=1003" << std::endl;
std::cerr << "next_nodes.size(): " << next_nodes.size() << std::endl;
if (!next_nodes.empty()) {
    for(auto nn : next_nodes) {
        static int counter = 0;
        std::cerr << "Node " << counter++ << ":" << std::endl;
        std::cerr << nn << std::endl;
    }
}
bool b = false;
b =  next_nodes.size() == 2 ;
b =  next_nodes[0].start == metanull::charmap::position{0,0} ;
b =  next_nodes[0].start_direction == metanull::charmap::EAST ;
b =  next_nodes[0].end == metanull::charmap::position{3,0} ;
b =  next_nodes[0].end_direction == metanull::charmap::EAST ;
b =  next_nodes[0].score == 3;
b =  next_nodes[0].visited.size() == 4 ;
b =  next_nodes[0].visited.front() == metanull::charmap::position{0,0} ;
b =  next_nodes[0].visited.back() == metanull::charmap::position{3,0} ;
b =  next_nodes[0].turns.size() == 0 ;
b =  !next_nodes[0].closed;
b =  next_nodes[1].start == metanull::charmap::position{0,0} ;
b =  next_nodes[1].start_direction == metanull::charmap::EAST ;
b =  next_nodes[1].end == metanull::charmap::position{2,1} ;
b =  next_nodes[1].end_direction == metanull::charmap::SOUTH;
b =  next_nodes[1].score == 1003;
b =  next_nodes[1].visited.size() == 4 ;
b =  next_nodes[1].visited.front() == metanull::charmap::position{0,0} ;
b =  next_nodes[1].visited.back() == metanull::charmap::position{2,1} ;
b =  next_nodes[1].turns.size() == 1 ;
b =  next_nodes[1].visited.front() == metanull::charmap::position{2,0} ;
b =  !next_nodes[1].closed;

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

        maze m(inputMap, start, end);
        m.open_nodes = m.init();
        for(auto n : m.open_nodes) {
            auto nn = m.next_nodes_from(n);
        }
    }
}

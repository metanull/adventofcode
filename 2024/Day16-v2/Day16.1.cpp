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

#include "maze.h"

// ---------------------------------------------------------
const char *banner = "AdventOfCode 2024 Day 16!";
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

    std::cout << "Input map:" << inputMap << std::endl;
    auto start = metanull::charmap::char_find(inputMap, 'S').front();
    auto end = metanull::charmap::char_find(inputMap, 'E').front();
    std::cout << "Start: " << start.first << "," << start.second << std::endl;
    std::cout << "End: " << end.first << "," << end.second << std::endl;

    maze m(inputMap, start, end);

    std::vector<maze_node> log;

    static size_t cur_best_score = 0;
    auto clock_start = std::chrono::high_resolution_clock::now();
    auto open_nodes = m.init();
    std::vector<maze_node> closed_nodes;
    while (!open_nodes.empty())
    {

        // Store closed nodes, and remove them from open_nodes
        std::copy_if(open_nodes.begin(), open_nodes.end(), std::back_inserter(closed_nodes), [](const maze_node &n)
                     { return n.closed; });
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [](const maze_node &n)
                                        { return n.closed; }),
                         open_nodes.end());
        // Break on the first closed node
        if (!closed_nodes.empty())
        {
            std::sort(closed_nodes.begin(), closed_nodes.end(), [](const maze_node &lhs, const maze_node &rhs)
                      { return lhs.score < rhs.score; });
            auto best_node = closed_nodes.front();
            std::cout << "Best node: " << best_node << std::endl;
            break;
        }

        // Sort open_nodes by score, to process the best nodes first
        std::sort(open_nodes.begin(), open_nodes.end(), [](const auto &lhs, const auto &rhs)
                  { return lhs.score < rhs.score; });

        // Discover the next nodes starting from the current best one
        auto next_nodes = m.next_nodes_from(open_nodes.front());
        open_nodes.erase(open_nodes.begin());

        // Process the discovered new nodes:
        // Remove nodes that are already found in the log with the same end, end_drection, and a score that is lower or equal
        next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [&log](const maze_node &n)
                                        { return std::find_if(log.begin(), log.end(), [&n](const maze_node &ln)
                                                              { return ln.end == n.end && ln.end_direction == n.end_direction && ln.score <= n.score; }) != log.end(); }),
                         next_nodes.end());
        // In the contrary remove from open_nodes the nodes that are already in the log with the same end, end_drection, and a score that is strictly lower
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [&log](const maze_node &n)
                                        { return std::find_if(log.begin(), log.end(), [&n](const maze_node &ln)
                                                              { return ln.end == n.end && ln.end_direction == n.end_direction && ln.score < n.score; }) != log.end(); }),
                         open_nodes.end());

        // Add remaining discovered nodes to the list of open_nodes pending processing
        if (!next_nodes.empty())
        {
            // Add remaining discovered nodes to the log
            std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(log));

            // Add the remaining next nodes to the list
            std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(open_nodes));
            continue;
        }
    }

    // Process solutions
    if (closed_nodes.empty())
    {
        std::cerr << "No solution found." << std::endl;
        return 1;
    }
    // List all (one) items in closed_nodes
    for (const auto &node : closed_nodes)
    {
        std::cout << node << std::endl;
    }
}

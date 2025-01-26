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

#define BREAK_ON_FIRST_BEST_SCORE 0

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

    auto clock_start = std::chrono::high_resolution_clock::now();
    auto open_nodes = m.init();
    std::vector<maze_node> closed_nodes;

    size_t lowest_score = SIZE_MAX;

    std::time_t start_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char start_time_buffer[26];
    ctime_s(start_time_buffer, sizeof(start_time_buffer), &start_time);
    std::cout << "Start time: " << start_time_buffer << std::endl;

    while (!open_nodes.empty())
    {
        // Print all nodes in open_nodes that are having closed = true

        for (const auto &node : open_nodes)
        {
            if (node.closed)
            {
                std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - std::chrono::system_clock::from_time_t(start_time)).count();
                auto hours = elapsed_seconds / 3600;
                elapsed_seconds %= 3600;
                auto minutes = elapsed_seconds / 60;
                auto seconds = elapsed_seconds % 60;
                char time_buffer[26];
                ctime_s(time_buffer, sizeof(time_buffer), &now_time);
                std::cout << "Current time: " << time_buffer;
                std::cout << "Elapsed time: " << hours << " hours, " << minutes << " minutes, " << seconds << " seconds." << std::endl;
                std::cout << "Closed: " << node << std::endl;
            }
        }

        // Capture the best score as soon as available
        if(lowest_score == SIZE_MAX && !closed_nodes.empty()) {
            lowest_score = closed_nodes.front().score;
            std::cout << "\033[7;32mLowest score: " << lowest_score << "\033[0m" << std::endl;
        }

        // Some debug output
        if (std::any_of(open_nodes.begin(), open_nodes.end(), [](const maze_node &n) { return n.closed; }))
        {
            std::cout << "\033[7;32mClosed node found.\033[0m" << std::endl;
        }

        // Store closed nodes...
        std::copy_if(open_nodes.begin(), open_nodes.end(), std::back_inserter(closed_nodes), [](const maze_node &n)
                     { return n.closed; });
        // ... and remove them from open_nodes
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [](const maze_node &n)
                                        { return n.closed; }),
                         open_nodes.end());

        #if (!defined(BREAK_ON_FIRST_BEST_SCORE) || BREAK_ON_FIRST_BEST_SCORE != 1)
            // Break only when we find a node that has a score greater than the best one
            bool found = false;
            for (auto &node : closed_nodes)
            {
                if(lowest_score != SIZE_MAX && node.score != lowest_score) {
                    found = true;
                    break;
                }
            }
            
            if(found) {
                // make sure that only closed nodes with the actual best_Score are kept
                closed_nodes.erase(std::remove_if(closed_nodes.begin(), closed_nodes.end(), [lowest_score](const maze_node &n)
                                                { return n.score != lowest_score; }),
                                closed_nodes.end());
                // Leave the loop
                break;
            }
        #else
            // Break on the very first closed node
            if(!closed_nodes.empty()) {
                std::sort(closed_nodes.begin(), closed_nodes.end(), [](const maze_node& lhs, const maze_node& rhs) {
                    return lhs.score < rhs.score;
                });
                auto best_node = closed_nodes.front();
                std::cout << "Best node: " << best_node << std::endl;
                break;
            }
        #endif

        // Sort open_nodes by score, to process the best nodes first
        std::sort(open_nodes.begin(), open_nodes.end(), [](const auto &lhs, const auto &rhs)
                  { return lhs.score < rhs.score; });

        // Discover the next nodes starting from the current best one
        auto next_nodes = m.next_nodes_from(open_nodes.front());
        open_nodes.erase(open_nodes.begin());

        // Trim all nodes with a score higher than lowest_score
        if(lowest_score != SIZE_MAX) {
            next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [lowest_score](const maze_node& n) {
                return n.score > lowest_score;
            }), next_nodes.end());
        }

        // Trim all nodes that are already found in the log with the same end, end_drection, and a score that is (1, if only the first best path is needed) lower or equal or (2, if all "best path" are needed) strictly lower
        next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [&log](const maze_node &n)
                                        { return std::find_if(log.begin(), log.end(), [&n](const maze_node &ln)
                                                            { 
                                                                #if (!defined(BREAK_ON_FIRST_BEST_SCORE) || BREAK_ON_FIRST_BEST_SCORE != 1)
                                                                    // <  if we are interrested in all path having the best score: 
                                                                    return ln.score < n.score && ln.end == n.end && ln.end_direction == n.end_direction; 
                                                                #else
                                                                    // <= if we are only interrested in the very first best score: 
                                                                    return ln.score <= n.score && ln.end == n.end && ln.end_direction == n.end_direction;
                                                                #endif
                                                            }) != log.end(); }),
                         next_nodes.end());

        // Trim open_nodes when they contain a node similar to one in next_nodes but with a greater score
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [&next_nodes](const maze_node &n)
                                        { return std::find_if(next_nodes.begin(), next_nodes.end(), [&n](const maze_node &nn)
                                                            { return nn.score < n.score && nn.end == n.end && nn.end_direction == n.end_direction; }) != next_nodes.end(); }),
                         open_nodes.end());

        // Add remaining discovered nodes to the list of open_nodes pending processing
        if (!next_nodes.empty())
        {
            // Update the log by adding or replacing entries from next_nodes
            log.erase(std::remove_if(log.begin(), log.end(), [&next_nodes](const maze_node &n)
                                     { return std::find_if(next_nodes.begin(), next_nodes.end(), [&n](const maze_node &nn)
                                                           { return nn.end == n.end && nn.end_direction == n.end_direction; }) != next_nodes.end(); }),
                      log.end());
            std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(log));

            // Add the remaining next nodes to the list
            std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(open_nodes));

            continue;
        }
    }

    std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char end_time_buffer[26];
    ctime_s(end_time_buffer, sizeof(end_time_buffer), &end_time);
    std::cout << "\033[34mEnd time: " << end_time_buffer << "\033[0m" << std::endl;

    // Process solutions
    if (closed_nodes.empty())
    {
        std::cerr << "\033[31mNo solution found.\033[0m]" << std::endl;
        return 1;
    }
    // Sort closed_nodes by score
    std::sort(closed_nodes.begin(), closed_nodes.end(), [](const maze_node &lhs, const maze_node &rhs)
              { return lhs.score < rhs.score; });

    std::set<metanull::charmap::position> unique_ends;
    if (!closed_nodes.empty())
    {
        size_t smallest_score = closed_nodes.front().score;

        // List all items in closed_nodes that have the smallest score
        std::cout << "\033[33mNodes with the smallest score (" << smallest_score << ")\033[0m]:" << std::endl;
        for (const auto &node : closed_nodes)
        {
            static int solution_number = 0;
            std::cout << "\033[34mSolution " << (++solution_number) << "\033[0m: " << node << std::endl;
            for (const auto &visited_node : node.visited) {
                unique_ends.insert(visited_node);
            }
        }
    }

    std::cout << "\033[34mEnd time: " << end_time_buffer << "\033[0m" << std::endl;

    // Print the number of items in unique_ends
    std::cout << "\033[34mNumber of unique ends: " << unique_ends.size() << "\033[0m" << std::endl;
}

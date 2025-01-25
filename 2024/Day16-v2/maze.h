#ifndef __DAY_16_V2_MAZE_H__
#define __DAY_16_V2_MAZE_H__

#include "charmap.h"
#include "math.h"

#include "Constant.h"

/**
 * @brief A node in the maze (e.g. a start position (the maze's start point), a score, and an end position, where the end position is: a crosspoint, a dead end, a loop or the end of the maze). A node is "closed" when it reaches the end of the maze.
 * 
 */
struct maze_node {
    size_t score = 0;
    bool closed = false;
    metanull::charmap::position start = {0,0};
    metanull::charmap::direction start_direction = metanull::charmap::EAST;
    metanull::charmap::position end = {0,0};
    metanull::charmap::direction end_direction = metanull::charmap::EAST;
    std::vector<metanull::charmap::position> visited = {};
    std::vector<metanull::charmap::position> turns = {};

    maze_node() = default;
    maze_node(const maze_node & other) = default;
    maze_node(metanull::charmap::position pos, metanull::charmap::direction dir, size_t score = 0);

    friend std::ostream & operator<<(std::ostream & os, const maze_node & n);

    /*
    static std::vector<maze_node> init(const metanull::charmap::map & m, const metanull::charmap::position & start, const metanull::charmap::direction & start_direction);
    static std::vector<maze_node> next_nodes_from(const metanull::charmap::map & m, maze_node & origin, const metanull::charmap::position & end);
    */
};

std::ostream & operator<<(std::ostream & os, const maze_node & n);

/**
 * @brief A maze is a map with a start and an end position
 */
struct maze {
    metanull::charmap::map m;
    metanull::charmap::position start = {0,0};
    metanull::charmap::direction start_direction = metanull::charmap::EAST;
    metanull::charmap::position end = {0,0};
    std::vector<maze_node> open_nodes;

    bool is_free(const metanull::charmap::position & p) const;
    bool is_free(char c) const;

    maze() = default;
    maze(const maze & other) = default;
    maze & operator=(const maze & other) = default;
    maze(const metanull::charmap::map & map, const metanull::charmap::position & start, const metanull::charmap::position & end, const metanull::charmap::direction & start_dir = metanull::charmap::EAST);
    maze(const metanull::charmap::map & map, char char_start = 'S', char char_end = 'E', const metanull::charmap::direction & start_dir = metanull::charmap::EAST);

    /**
     * @brief Get the very first node(s)
     */
    std::vector<maze_node> init() const;

    /**
     * @brief Get the next nodes from a given node (by browsing the maze until a dead end, a crosspoint, a loop or the end of the maze)
     */
    std::vector<maze_node> next_nodes_from(maze_node & origin) const;
};

#endif // __DAY_16_V2_MAZE_H__
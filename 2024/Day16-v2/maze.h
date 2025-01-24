#ifndef __DAY_16_V2_MAZE_H__
#define __DAY_16_V2_MAZE_H__

#include "charmap.h"
#include "math.h"

#include "Constant.h"

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
};

std::ostream & operator<<(std::ostream & os, const maze_node & n);

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

    std::vector<maze_node> init() const;
    std::vector<maze_node> next_nodes_from(maze_node & origin) const;
};

#endif // __DAY_16_V2_MAZE_H__
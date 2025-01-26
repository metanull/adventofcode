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

    
    typedef std::function<bool(const metanull::charmap::map & m, const metanull::charmap::position & pos, const metanull::charmap::position & end)> tile_test_function;
    const static tile_test_function is_tile_free;
    static const size_t score_turn = 1000;
    static const size_t score_step = 1;

    static std::vector<maze_node> init(const metanull::charmap::map & m, const metanull::charmap::position & start, const metanull::charmap::position & end, const metanull::charmap::direction & start_direction, const tile_test_function & is_tile_free);
    static std::vector<maze_node> next_nodes_from(const metanull::charmap::map & m, maze_node & cur, const metanull::charmap::position & end, const tile_test_function & is_tile_free);
    static std::vector<maze_node> find_first_best_path(const metanull::charmap::map & m, const metanull::charmap::position & start, const metanull::charmap::position & end, const metanull::charmap::direction & start_direction, const tile_test_function & is_tile_free);
    static std::vector<maze_node> find_all_best_path(const metanull::charmap::map & m, const metanull::charmap::position & start, const metanull::charmap::position & end, const metanull::charmap::direction & start_direction, const tile_test_function & is_tile_free);
};

std::ostream & operator<<(std::ostream & os, const maze_node & n);

#endif // __DAY_16_V2_MAZE_H__
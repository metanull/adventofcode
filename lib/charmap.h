#ifndef __METANULL_CHARMAP_H__
#define __METANULL_CHARMAP_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <functional>

namespace metanull
{
    namespace charmap
    {
        /** A charmap row (an array of characters) */
        using row = std::vector<char>;
        /** A two-dimentionnal character map */
        using map = std::vector<row>;
        /** A pair of unsigned integers, defining a position in a charmap, where first is the abscissa and second is the ordinate */
        using position = std::pair<size_t, size_t>;
        /** A pair of integers, defining a direction, where first is the horizontal direction (-1,0,1) and second the vertical direction (-1,0,1) */
        using direction = std::pair<int, int>;

        size_t horizontal_distance(const position &a, const position &b);
        size_t vertical_distance(const position &a, const position &b);
        const size_t &ordinate(const position &p);
        size_t &ordinate(position &p);
        const size_t &abscissa(const position &p);
        size_t &abscissa(position &p);

        /** No flag is set */
        static const int SUBSET_MATCH_NONE = 0;
        /** when set, match, find and replace treats '\0' as a wild character. */
        static const int SUBSET_MATCH_NULL_AS_WILDCHARACTER = 1;
        /** when set, match and subset, returns a map even when the search expression goes past the boundaries of the map */
        static const int SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS = 2;

        static const direction UNKNOWN = {0, 0};
        static const direction SOUTH = {0, 1};
        static const direction NORTHEAST = {1, 1};
        static const direction EAST = {1, 0};
        static const direction SOUTHEAST = {1, -1};
        static const direction NORTH = {0, -1};
        static const direction SOUTHWEST = {-1, -1};
        static const direction WEST = {-1, 0};
        static const direction NORTHWEST = {-1, 1};

        /**
         * Get a string representation of a direction
         */
        std::string direction_as_string(const direction &d);

        /**
         * Get a string representation of a position
         */
        std::string position_as_string(const position &d);

        /**
         * Rotate a direction clockwise
         * @param d The direction to rotate
         * @return The rotated direction
         */
        direction rotate_clockwise(const direction &d);

        /**
         * Rotate a direction counter-clockwise
         * @param d The direction to rotate
         * @return The rotated direction
         */
        direction rotate_counter_clockwise(const direction &d);

        /**
         * Inverse a direction
         * @param d The direction to inverse
         * @return The inversed direction
         */
        direction inverse(const direction &d);

        /**
         * Get a subset of a character map.
         * It returns a map starting at the point origin, and spanning to the point end
         * @param m The map
         * @param origin The origin point of the subset
         * @param end The end point
         * @param flags The function returns an empty map if end is past the boundaries of m. If flag SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS is set, it returns instead, a truncated subset
         * @return The resulting charmap
         */
        map subset(const map &m, position origin, position end, int flags = 0);
        /**
         * Get a subset of a character map.
         * It returns a map starting at the point origin, and spanning over exactly n rows and columns
         * @param m The map
         * @param origin The origin point of the subset
         * @param n The number of rows and columns to retrieve
         * @param flags The function returns an empty map if end is past the boundaries of m. If flag SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS is set, it returns instead, a truncated subset
         * @return The resulting charmap
         */
        map subset(const map &m, position origin, size_t n, int flags = 0);
        /**
         * Test if a subset of a charmap matches a specific shape
         * It returns a map starting at the point origin, and spanning over exactly n rows and columns
         * @param m The map
         * @param origin The origin point for the match
         * @param needle The pattern to compare the subset to
         * @throws std::out_of_range
         * @return True if the subset matches with needle
         */
        bool subset_quick_matches(const map &m, position origin, const map &needle);

        /**
         * Test if a subset of a charmap matches a specific shape
         * It returns a map starting at the point origin, and spanning over exactly n rows and columns
         * @param m The map
         * @param origin The origin point for the match
         * @param needle The pattern to compare the subset to
         * @param flags The function returns false if the needle spans past the boundaries of m.
         *              If flag SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS is set, it returns instead, a truncated subset
         *              If flag SUBSET_MATCH_NULL_AS_WILDCHARACTER, the function ignore null character ('\0', (char)0, '0x00') from the needle, effectively using the null character as a match-any wildcard
         * @return True if the subset matches with needle
         */
        bool subset_matches(const map &m, position origin, const map &needle, int flags = 0);

        /**
         * Finds all occurrences of a specific shape in a charmap, returning their origin coordinates
         * @param m The map
         * @param needle the pattern to look for in the charmap
         * @param flags If flag SUBSET_MATCH_NULL_AS_WILDCHARACTER, the function ignore null character ('\0', (char)0, '0x00') from the needle, effectively using the null character as a match-any wildcard
         * @return An array of positions of all matches
         */
        std::vector<position> subset_find(const map &m, const map &needle, int flags = 0);

        /**
         * Overwrite a portion of a charmap with another
         * @param m The target map
         * @param origin The origin point for the replacement
         * @param replace The pattern to put in the charmap
         * @throws std::out_of_range
         * @return The number of characters effectively modified in the charmap
         */
        void subset_quick_replace(map &m, position origin, const map &replace);

        /**
         * Overwrite a portion of a charmap with another
         * @param m The target map
         * @param origin The origin point for the replacement
         * @param replace The pattern to put in the charmap
         * @param flags If flag SUBSET_MATCH_NULL_AS_WILDCHARACTER, the function ignore null character ('\0', (char)0, '0x00') from the replace, effectively allowing to leave some characters untouched by the replacement
         * @return The number of characters effectively modified in the charmap
         */
        size_t subset_replace(map &m, position origin, const map &replace, int flags = 0);

        /**
         * Find all differences between two charmaps
         * @param a The first map
         * @param b The second map
         * @param flags If flag SUBSET_MATCH_NULL_AS_WILDCHARACTER, the function ignore null character ('\0', (char)0, '0x00') from the b map, effectively using the null character as a match-any wildcard
         * @return The number of differences between the two maps
         */
        size_t diff_count(const map &a, const map &b, int flags = 0);

        /**
         * Finds all occurrences of a specific characterin a charmap, returning their origin coordinates
         * @param m The map
         * @param needle the character to look for in the charmap
         * @return An array of positions of all matches
         */
        std::vector<position> char_find(const map &m, char needle);

        /**
         * Translates the position of a point in map by a given vector or direction
         * @param o The position
         * @param d The direction vector
         * @return The translated position
         */
        position translate(const position &o, direction d);
        /**
         * Test that an position  is within the boundaries of a map
         * @param m The map
         * @param p The position to test
         * @throws std::out_of_range
         */
        void test_in_bounds(const map &m, const position &p);

        /**
         * Access an item in the charmap by its position
         * @param m The map
         * @param p The position of the items
         * @return A reference to the character
         * @throws std::out_of_range
         */
        char &access(map &m, const position &p);
        /**
         * Access an item in the charmap by its position
         * @param m The map
         * @param p The position of the items
         * @return A reference to the character
         * @throws std::out_of_range
         */
        const char &access(const map &m, const position &p);

        /**
         * Lists all neighbours of a cell that passes a userdefined test
         * @param m The map
         * @param p The position of the point around which to check
         * @param t The test function in the form bool(position,char,direction), it receives: the position of the neighbour, the character at that position, as well as the relative direction to that neighbour starting from p)
         * @return A array of all pairs of character/direction satisfying the test
         */
        std::vector<std::pair<direction, char>> neighbours_if(const map &m, const position &p, std::function<bool(position, char, direction)> t);

        /**
         * Get the column of a charmap at a specific position
         * @param m The map
         * @param p The position of the column
         * @return The column as a string
         */
        std::string column_as_string(const map &m, const position &p);
        /**
         * Get the row of a charmap at a specific position
         * @param m The map
         * @param p The position of the row
         * @return The row as a string
         */
        std::string row_as_string(const map &m, const position &p);
        /**
         * Swap rows and columns of a charmap
         * @param m The map
         * @return The transposed map
         */
        map transpose(const map &m);

        /**
         * Flag to permit reverting order of the rows when printing
         */
        const std::ios_base::iostate invert_rows = std::ios_base::iostate(1 << 16);

        /**
         * A manipulator to invert the order of the rows when printing
         */
        std::ostream &invert(std::ostream &os);

        /**
         * @struct maze_node
         * @brief Represents a node in a maze with various attributes.
         *
         * This structure holds information about a node in a maze, including its score,
         * whether it is closed, its start and end positions and directions, and the
         * positions it has visited and turned.
         */
        struct maze_node
        {
            /**
             * @var maze_node::score
             * @brief The score associated with the node.
             */
            size_t score = 0;
            /**
             * @var maze_node::closed
             * @brief Indicates whether the node is closed (In other words does it cover a full path from the beginning to the end of the maze).
             */
            bool closed = false;
            /**
             * @var maze_node::start
             * @brief The starting position of the node.
             */
            position start = {0, 0};
            /**
             * @var maze_node::start_direction
             * @brief The starting direction of the node.
             */
            direction start_direction = EAST;
            /**
             * @var maze_node::end
             * @brief The ending position of the node.
             */
            position end = {0, 0};
            /**
             * @var maze_node::end_direction
             * @brief The ending direction of the node.
             */
            direction end_direction = EAST;
            /**
             * @var maze_node::visited
             * @brief A vector of positions that have been visited by the node.
             */
            std::vector<position> visited = {};
            /**
             * @var maze_node::turns
             * @brief A vector of positions where the path has turned.
             */
            std::vector<position> turns = {};
            /**
             * @brief Default constructor for maze_node.
             */
            maze_node() = default;
            /**
             * @brief Copy constructor for maze_node.
             *
             * @param other The other maze_node to copy from.
             */
            maze_node(const maze_node &other) = default;
            /**
             * @brief Parameterized constructor for maze_node.
             *
             * @param pos The position of the node.
             * @param dir The direction of the node.
             * @param score The score of the node (default is 0).
             */
            maze_node(position pos, direction dir, size_t score = 0);

            /**
             * @brief Assignment operator for maze_node.
             * @param other The other maze_node to assign from.
             */
            maze_node & operator=(const maze_node &other) = default;
        };

        /**
         * @brief Defines the signature of a function that tests if a tile of the maze is free.
         */
        using tile_test_function = std::function<bool(const map &m, const position &pos, const position &end)>;
        /**
         * @brief Defines the signature of a function that calculates the score of a move in the maze.
         */
        using score_move_function = std::function<size_t(const map &m, const position &pos, const direction &pos_direction, const position &next, const direction &next_direction)>;

        /**
         * @brief A function that tests if a tile of the maze is free.
         *
         * @param m The map.
         * @param p The position of the tile.
         * @param end The ending position.
         * @return True if the tile is free, false otherwise.
         */
        extern tile_test_function is_tile_free;
        /*
         * @brief A function that calculates the score of a move in the maze.
         *
         * @param m The map.
         * @param cur The current position.
         * @param cur_direction The current direction.
         * @param next The next position.
         * @param next_direction The next direction.
         * @return The score of the move. This function returns 1 if the current position is different from the next position, and 0 otherwise.
         *
         */
        extern score_move_function basic_score_move;
        /**
         * @brief A function that calculates the score of a move in the maze.
         *
         * @param m The map.
         * @param cur The current position.
         * @param cur_direction The current direction.
         * @param next The next position.
         * @param next_direction The next direction.
         * @return The score of the move. This function returns 0 if the current position is the same as the next position, otherwise it if the move induced a change in direction: 1 + 1000, otehrwise 1.
         */
        extern score_move_function weighted_turn_score_move;

        /**
         * @brief Get the first nodes in a maze.
         *
         * @param m The map.
         * @param start The starting position.
         * @param end The ending position.
         * @param start_direction The starting direction.
         * @param get_score_for_move The function to calculate the score for a move (default is basic_score_move).
         * @param test_tile_is_free The function to test if a tile is free (default is is_tile_free).
         * @return A vector of the first nodes in the maze.
         */
        std::vector<maze_node> maze_get_first_nodes(const map &m, const position &start, const position &end, const direction &start_direction, const score_move_function &get_score_for_move = basic_score_move, const tile_test_function &test_tile_is_free = is_tile_free);

        /**
         * @brief Get the next nodes in a maze.
         *
         * @param m The map.
         * @param current The current node.
         * @param end The ending position.
         * @param get_score_for_move The function to calculate the score for a move (default is basic_score_move).
         * @param test_tile_is_free The function to test if a tile is free (default is is_tile_free).
         * @return A vector of the next nodes in the maze.
         */
        std::vector<maze_node> maze_get_next_nodes(const map &m, maze_node &current, const position &end, const score_move_function &get_score_for_move = basic_score_move, const tile_test_function &test_tile_is_free = is_tile_free);

        /**
         * @brief Find the first best path in a maze.
         *
         * @param m The map.
         * @param start The starting position.
         * @param end The ending position.
         * @param start_direction The starting direction.
         * @param get_score_for_move The function to calculate the score for a move (default is basic_score_move).
         * @param test_tile_is_free The function to test if a tile is free (default is is_tile_free).
         * @return A vector of the nodes in the first best path.
         */
        std::vector<maze_node> maze_find_first_best_path(const map &m, const position &start, const position &end, const direction &start_direction, const score_move_function &get_score_for_move = basic_score_move, const tile_test_function &test_tile_is_free = is_tile_free);

        /**
         * @brief Find all best paths in a maze.
         *
         * @param m The map.
         * @param start The starting position.
         * @param end The ending position.
         * @param start_direction The starting direction.
         * @param get_score_for_move The function to calculate the score for a move (default is basic_score_move).
         * @param test_tile_is_free The function to test if a tile is free (default is is_tile_free).
         * @return A vector of the nodes in all best paths.
         */
        std::vector<maze_node> maze_find_all_best_path(const map &m, const position &start, const position &end, const direction &start_direction, const score_move_function &get_score_for_move = basic_score_move, const tile_test_function &test_tile_is_free = is_tile_free);

        /**
         * @brief Print a path in a maze.
         * @param os The output stream.
         * @param m The map.
         * @param path The path to print.
         */
        std::ostream &maze_print_path(std::ostream &os, const map &m, const position &start, const position &end, const maze_node &path, const tile_test_function &test_tile_is_free = is_tile_free);

        /**
         * @brief Print multiple path in a maze.
         * @param os The output stream.
         * @param m The map.
         * @param path The path to print.
         */
        std::ostream &maze_print_path(std::ostream &os, const map &m, const position &start, const position &end, const std::vector<maze_node> &path, const tile_test_function &test_tile_is_free = is_tile_free);
    }
}

/**
 * Read a character map from an input stream
 * @param is The input stream
 * @param m The map to read into
 * @return The input stream
 */
std::istream &operator>>(std::istream &is, metanull::charmap::map &m);

/**
 * Write a character map to an output stream
 * @param os The output stream
 * @param m The map to write
 * @return The output stream
 */
std::ostream &operator<<(std::ostream &os, const metanull::charmap::map &m);

/**
 * Write a maze_node to an output stream
 * @param os The output stream
 * @param p The maze_node to write
 * @return The output stream
 */
std::ostream &operator<<(std::ostream &os, const metanull::charmap::maze_node &n);

#endif // __METANULL_CHARMAP_H__
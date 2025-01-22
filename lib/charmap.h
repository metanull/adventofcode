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

namespace metanull {
    namespace charmap {
        /** A charmap row (an array of characters) */
        using row = std::vector<char>;
        /** A two-dimentionnal character map */
        using map = std::vector<row>;
        /** A pair of unsigned integers, defining a position in a charmap */
        using index = std::pair<size_t,size_t>;
        /** A pair of integers, defining a direction, where first is the horizontal direction (-1,0,1) and second the vertical direction (-1,0,1) */
        using direction = std::pair<int,int>;

        /** No flag is set */
        static const int SUBSET_MATCH_NONE = 0;
        /** when set, match, find and replace treats '\0' as a wild character. */
        static const int SUBSET_MATCH_NULL_AS_WILDCHARACTER = 1;
        /** when set, match and subset, returns a map even when the search expression goes past the boundaries of the map */
        static const int SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS = 2;

        static const direction UNKNOWN      = { 0, 0};
        static const direction NORTH        = { 0, 1};
        static const direction NORTHEAST    = { 1, 1};
        static const direction EAST         = { 1, 0};
        static const direction SOUTHEAST    = { 1,-1};
        static const direction SOUTH        = { 0,-1};
        static const direction SOUTHWEST    = {-1,-1};
        static const direction WEST         = {-1, 0};
        static const direction NORTHWEST    = {-1, 1};

        /**
         * Get a subset of a character map.
         * It returns a map starting at the point origin, and spanning to the point end
         * @param m The map
         * @param origin The origin point of the subset
         * @param end The end point
         * @param flags The function returns an empty map if end is past the boundaries of m. If flag SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS is set, it returns instead, a truncated subset
         * @return The resulting charmap
         */
        map subset(const map & m, index origin, index end, int flags = 0);
        /**
         * Get a subset of a character map.
         * It returns a map starting at the point origin, and spanning over exactly n rows and columns
         * @param m The map
         * @param origin The origin point of the subset
         * @param n The number of rows and columns to retrieve
         * @param flags The function returns an empty map if end is past the boundaries of m. If flag SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS is set, it returns instead, a truncated subset
         * @return The resulting charmap
         */
        map subset(const map & m, index origin, size_t n, int flags = 0);
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
        bool subset_matches(const map & m, index origin, const map & needle, int flags = 0);
        /**
         * Finds all occurrences of a specific shape in a charmap, returning their origin coordinates
         * @param m The map
         * @param needle the pattern to look for in the charmap
         * @param flags If flag SUBSET_MATCH_NULL_AS_WILDCHARACTER, the function ignore null character ('\0', (char)0, '0x00') from the needle, effectively using the null character as a match-any wildcard
         * @return An array of positions of all matches
         */
        std::vector<index> subset_find(const map & m, const map & needle, int flags = 0);
        /**
         * Overwrite a portion of a charmap with another
         * @param m The target map
         * @param origin The origin point for the replacement
         * @param replace The pattern to put in the charmap
         * @param flags If flag SUBSET_MATCH_NULL_AS_WILDCHARACTER, the function ignore null character ('\0', (char)0, '0x00') from the replace, effectively allowing to leave some characters untouched by the replacement
         * @return The number of characters effectively modified in the charmap
         */
        size_t subset_replace(map & m, index origin, const map & replace, int flags = 0);

        /**
         * Finds all occurrences of a specific characterin a charmap, returning their origin coordinates
         * @param m The map
         * @param needle the character to look for in the charmap
         * @return An array of positions of all matches
         */
        std::vector<index> char_find(const map & m, char needle);

        /**
         * Translates the index of a point in map by a given vector or direction
         * @param o The index
         * @param d The direction vector
         * @return The translated index
         */
        index translate(const index & o, direction d);
        /**
         * Test that an index  is within the boundaries of a map
         * @param m The map
         * @param p The index to test
         * @throws std::out_of_range
         */
        void test_in_bounds(const map & m, const index & p);

        /**
         * Access an item in the charmap by its index
         * @param m The map
         * @param p The index of the items
         * @return A reference to the character
         * @throws std::out_of_range
         */
        char & access(map & m, const index & p);
        /**
         * Access an item in the charmap by its index
         * @param m The map
         * @param p The index of the items
         * @return A reference to the character
         * @throws std::out_of_range
         */
        const char & access(const map & m, const index & p);

        /**
         * Lists all neighbours of a cell that passes a userdefined test
         * @param m The map
         * @param p The index of the point around which to check
         * @param t The test function in the form bool(char,direction), it receives the character to test, and well as the relative direction to reach it from the reference)
         * @return A array of all pairs of character/direction satisfying the test
         */
        std::vector<std::pair<direction,char>> neighbours_if(const map & m, const index & p, std::function<bool(char,direction)> t);

        /**
         * Get the column of a charmap at a specific index
         * @param m The map
         * @param p The index of the column
         * @return The column as a string
         */
        std::string column_as_string(const map & m, const index & p);
        /**
         * Get the row of a charmap at a specific index
         * @param m The map
         * @param p The index of the row
         * @return The row as a string
         */
        std::string row_as_string(const map & m, const index & p);
        /**
         * Swap rows and columns of a charmap
         * @param m The map
         * @return The transposed map
         */
        map transpose(const map & m);
    }
}

#endif // __METANULL_CHARMAP_H__
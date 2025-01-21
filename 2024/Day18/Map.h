#ifndef __DAY_18_MAP_H__
#define __DAY_18_MAP_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace metanull {
    namespace charmap {
        /** A charmap row (an array of characters) */
        using row = std::vector<char>;
        /** A two-dimentionnal character map */
        using map = std::vector<row>;
        /** A pair of unisgned integers, defining a position in a charmap */
        using index = std::pair<size_t,size_t>;

        /** No flag is set */
        static const int SUBSET_MATCH_NONE = 0;
        /** when set, match, find and replace treats '\0' as a wild character. */
        static const int SUBSET_MATCH_NULL_AS_WILDCHARACTER = 1;
        /** when set, match and subset, returns a map even when the search expression goes past the boundaries of the map */
        static const int SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS = 2;

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
    }
}

#endif // __DAY_18_MAP_H__
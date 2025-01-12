#ifndef ADVENTOFCODE_H
#define ADVENTOFCODE_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <cstddef>

inline uint64_t count_digits(uint64_t n);
inline bool count_digits_is_even(uint64_t n);
template<class Os, class Co> Os& operator<<(Os& os, const Co& co);
template <typename T> std::vector<T> factorize(T num);

/**
 * Print a pair<int,int>
 */
std::ostream& operator<<(std::ostream& os, const std::pair<int,int> & p);
/**
 * Print a vector<char>
 */
std::ostream& operator<<(std::ostream& os, const std::vector<char> & vec);
/**
 * Print a vector<vector<char>>
 */
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<char>> & map);


#endif // ADVENTOFCODE_H
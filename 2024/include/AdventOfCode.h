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

#endif // ADVENTOFCODE_H
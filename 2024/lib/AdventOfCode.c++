#include "..\include\AdventOfCode.h"

template<class Os, class Co> Os& operator<<(Os& os, const Co& co)
{
    os << '{';
    for (auto const& i : co)
        os << ' ' << i;
    return os << " } ";
}
inline uint64_t count_digits(uint64_t n) {
    if(n == 0) {
        return 1;
    }
    return (uint64_t)(std::floor(std::log10(n)) + 1);
}
inline bool count_digits_is_even(uint64_t n) {
    return (count_digits(n) % 2 == 0) ? true : false;
}
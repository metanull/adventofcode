#include "MazeRunner.h"

std::ostream & operator<<(std::ostream & os, const MazeRunnerPosition & p) {
    os << "\033[7;1mPOSITION\033[27;22m" << std::endl;
    os << "\033[1m(" << std::setw(5) << p.position.first << ", " << std::setw(5) << p.position.second << ") \033[22m";
    os << "[" << p.direction << "]";
    os << "[" << std::setw(10) << p.score << "]";
    return os;
}

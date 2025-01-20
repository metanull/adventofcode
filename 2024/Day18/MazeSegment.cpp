#include "MazeRunner.h"

std::ostream & operator<<(std::ostream & os, const MazeSegment & s){
    os << "\033[7;1mSEGMENT\033[27;22m" << std::endl;
    os << "\033[7mFROM  : ";
    os << "\033[1m(" << std::setw(5) << s.origin.position.first << ", " << std::setw(5) << s.origin.position.second << ") \033[22m";
    os << "[" << s.origin.direction << "]";
    os << "[" << std::setw(10) << s.origin.score << "]";
    os << "\033[27m" << std::endl;
    if(!s.moves.empty()) {
        for(auto m: s.moves) {
            os << "\t";
            os << "\033[1m(" << std::setw(5) << m.position.first << ", " << std::setw(5) << m.position.second << ") \033[22m";
            os << "[" << m.direction << "]";
            os << "[" << std::setw(10) << m.score << "]";
            os << std::endl;
        }
    }
    os << "\033[7m";
    os << "TO    : ";
    os << "\033[1m(" << std::setw(5) << s.end.position.first << ", " << std::setw(5) << s.end.position.second << ") \033[22m";
    os << "[" << s.end.direction << "]";
    os << "[" << std::setw(10) << s.end.score << "]";
    os << "\033[27m";
    return os;
}

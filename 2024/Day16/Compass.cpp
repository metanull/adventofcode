#include "Compass.h"

Compass::Compass(char c) {
    if(c != NORTH && c != EAST && c != SOUTH && c != WEST) {
        throw std::runtime_error("Invalid compass direction");
    }
    d = c;
}
Compass::Compass(const Compass & other) : d(other.d) {

}

Compass & Compass::operator=(const Compass & other) {
    if(other.d != NORTH && other.d != EAST && other.d != SOUTH && other.d != WEST) {
        throw std::runtime_error("Invalid compass direction");
    }
    d = other.d;
    return *this;
}
Compass::operator char() const { 
    return d; 
}
bool Compass::operator==(const Compass & other) const { 
    return *this == other.d;
}
bool Compass::operator!=(const Compass & other) const { 
    return !(*this == other.d);
}
bool Compass::operator==(const char & other) const { 
    return d == other;
}
bool Compass::operator!=(const char & other) const { 
    return !(*this == other);
}

Compass & Compass::Clockwise() {
    d = Compass::Clockwise(d);
    return *this;
}
Compass & Compass::CounterClockwise() {
    d = Compass::CounterClockwise(d);
    return *this;
}
Compass & Compass::Reverse() {
    d = Compass::Reverse(d);
    return *this;
}

inline char Compass::Clockwise(char Compass) {
    switch(Compass) {
        case NORTH: return EAST;
        case EAST: return SOUTH;
        case SOUTH: return WEST;
        case WEST: return NORTH;
    }
    throw std::runtime_error("Invalid direction");
}
inline char Compass::CounterClockwise(char Compass) {
    switch(Compass) {
        case NORTH: return WEST;
        case WEST: return SOUTH;
        case SOUTH: return EAST;
        case EAST: return NORTH;
    }
    throw std::runtime_error("Invalid direction");
}
inline char Compass::Reverse(char Compass) {
    switch(Compass) {
        case NORTH: return SOUTH;
        case WEST: return EAST;
        case SOUTH: return NORTH;
        case EAST: return WEST;
    }
    throw std::runtime_error("Invalid direction");
}

std::ostream& Compass::operator<<(std::ostream& os) const {
    switch(d) {
        case NORTH: os << "North"; break;
        case EAST: os << "East"; break;
        case SOUTH: os << "South"; break;
        case WEST: os << "West"; break;
    }
    return os;
}

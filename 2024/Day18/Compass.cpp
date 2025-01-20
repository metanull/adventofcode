#include "Compass.h"

Compass::Compass(char c) {
    if(c != UNKNOWN && c != NORTH && c != EAST && c != SOUTH && c != WEST) {
        throw std::runtime_error("Invalid compass direction");
    }
    d = c;
}
Compass::Compass(const Compass & other) : d(other.d) {

}

Compass & Compass::operator=(const Compass & other) {
    if(other.d != UNKNOWN && other.d != NORTH && other.d != EAST && other.d != SOUTH && other.d != WEST) {
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

char Compass::Clockwise(char Compass) {
    switch(Compass) {
        case NORTH: return EAST;
        case EAST: return SOUTH;
        case SOUTH: return WEST;
        case WEST: return NORTH;
        case UNKNOWN: return UNKNOWN;
    }
    throw std::runtime_error("Invalid direction");
}
char Compass::CounterClockwise(char Compass) {
    switch(Compass) {
        case NORTH: return WEST;
        case WEST: return SOUTH;
        case SOUTH: return EAST;
        case EAST: return NORTH;
        case UNKNOWN: return UNKNOWN;
    }
    throw std::runtime_error("Invalid direction");
}
char Compass::Reverse(char Compass) {
    switch(Compass) {
        case NORTH: return SOUTH;
        case WEST: return EAST;
        case SOUTH: return NORTH;
        case EAST: return WEST;
        case UNKNOWN: return UNKNOWN;
    }
    throw std::runtime_error("Invalid direction");
}

std::ostream& operator<<(std::ostream& os, const Compass & c) {
    switch(c) {
        case Compass::NORTH: os << "^"; break;
        case Compass::EAST: os << ">"; break;
        case Compass::SOUTH: os << "v"; break;
        case Compass::WEST: os << "<"; break;
        case Compass::UNKNOWN: os << "{?}"; break;
    }
    return os;
}

bool Compass::valid() const {
    return d == UNKNOWN || d == NORTH || d == EAST || d == SOUTH || d == WEST;
}
bool Compass::initialized() const {
    return valid() && d != UNKNOWN;
}
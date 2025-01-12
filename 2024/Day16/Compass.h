#ifndef __DAY16_DIRECTION_H__
#define __DAY16_DIRECTION_H__

#include <stdexcept>
#include <iostream>

struct Compass {
    static const char NORTH = 'N';
    static const char EAST = 'E';
    static const char SOUTH = 'S';
    static const char WEST = 'W';

    char d = EAST;                   // Compass (N,E,S,W)

    Compass() = default;
    Compass(char d);
    Compass(const Compass & other);
    Compass & operator=(const Compass & other);
    
    operator char() const;

    bool operator==(const Compass & other) const;
    bool operator!=(const Compass & other) const;
    bool operator==(const char & other) const;
    bool operator!=(const char & other) const;
    
    Compass & Clockwise();
    Compass & CounterClockwise();
    Compass & Reverse();

    /*
    * Return the next direction when turning clockwise
    * @param d The current direction (N,E,S,W)
    * @return The next direction (N,E,S,W)
    */
    static inline char Clockwise(char d);
    /*
    * Return the next direction when turning counter clockwise
    * @param d The current direction (N,E,S,W)
    * @return The next direction (N,E,S,W)
    */
    static inline char CounterClockwise(char d);
    /*
    * Return the next direction when turning 180°
    * @param d The current direction (N,E,S,W)
    * @return The next direction (N,E,S,W)
    */
    static inline char Reverse(char d);

    std::ostream& operator<<(std::ostream& os) const;
};

#endif // __DAY16_DIRECTION_H__
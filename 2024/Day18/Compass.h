#ifndef __DAY16_DIRECTION_H__
#define __DAY16_DIRECTION_H__

#include <stdexcept>
#include <iostream>

struct Compass {
    static const char UNKNOWN = 0;
    static const char NORTH = 'N';
    static const char EAST = 'E';
    static const char SOUTH = 'S';
    static const char WEST = 'W';

    char d = UNKNOWN;                   // Compass (N,E,S,W)

    Compass() = default;
    Compass(char d);
    Compass(const Compass & other);
    Compass & operator=(const Compass & other);
    
    operator char() const;

    bool valid() const;
    bool initialized() const;

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
    static char Clockwise(char d);
    /*
    * Return the next direction when turning counter clockwise
    * @param d The current direction (N,E,S,W)
    * @return The next direction (N,E,S,W)
    */
    static char CounterClockwise(char d);
    /*
    * Return the next direction when turning 180°
    * @param d The current direction (N,E,S,W)
    * @return The next direction (N,E,S,W)
    */
    static char Reverse(char d);

    friend std::ostream& operator<<(std::ostream& os, const Compass & c);
};


#endif // __DAY16_DIRECTION_H__
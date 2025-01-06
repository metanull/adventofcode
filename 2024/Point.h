#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <cmath>

struct Point {
    long x;
    long y;

    Point();
    Point(long X, long Y);
    Point(const Point & p);

    Point & operator=(const Point & M);
    friend bool operator==(const Point& A, const Point& B);
    friend bool operator<(const Point& A, const Point& B);
    friend bool operator> (const Point& lhs, const Point& rhs);
    friend bool operator<=(const Point& lhs, const Point& rhs);
    friend bool operator>=(const Point& lhs, const Point& rhs);

    Point & operator+=(const Point & M);
    Point & operator-=(const Point & M);

    bool inBounds(const Point& O, const Point& Z) const;

    static long determinant(const Point& A, const Point& B);
    static Point origin();

    Point & Up();
    Point & Down();
    Point & Right();
    Point & Left();

    bool TryUp(const Point & Z);
    bool TryUp(const Point & O, const Point & Z);
    bool TryDown(const Point & Z);
    bool TryDown(const Point & O, const Point & Z);
    bool TryRight(const Point & Z);
    bool TryRight(const Point & O, const Point & Z);
    bool TryLeft(const Point & Z);
    bool TryLeft(const Point & O, const Point & Z);

    friend std::ostream& operator<< (std::ostream& stream, const Point & point);
    friend bool cmpX(const Point & a, const Point & b);
    friend bool cmpY(const Point & a, const Point & b);


};

#endif // POINT_H
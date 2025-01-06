#include "Point.h"

#include <iostream>
Point::Point() : x(0), y(0) {}
Point::Point(long X, long Y) : x(X), y(Y) {}
Point::Point(const Point & p) : x(p.x), y(p.y) {}

long Point::determinant(const Point& A, const Point& B) {
    return (B.x - A.x) * (B.y - A.y);
}

Point Point::origin() {
    return Point(0, 0);
}

Point& Point::operator=(const Point& M) {
    if (this == &M) {
        return *this;
    }
    x = M.x;
    y = M.y;
    return *this;
}

bool operator==(const Point& A, const Point& B) {
    return A.x == B.x && A.y == B.y;
}

bool operator<(const Point& A, const Point& B) {
    return A.x < B.x && A.y < B.y;
}

bool operator>(const Point& lhs, const Point& rhs) {
    return rhs < lhs;
}

bool operator<=(const Point& lhs, const Point& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const Point& lhs, const Point& rhs) {
    return !(lhs < rhs);
}

Point& Point::operator+=(const Point& M) {
    x += M.x;
    y += M.y;
    return *this;
}

Point& Point::operator-=(const Point& M) {
    x -= M.x;
    y -= M.y;
    return *this;
}

bool Point::inBounds(const Point& O, const Point& Z) const {
    return x >= O.x && x < Z.x && y >= O.y && y < Z.y;
}

Point& Point::Up() {
    y++;
    return *this;
}

Point& Point::Down() {
    y--;
    return *this;
}

Point& Point::Right() {
    x++;
    return *this;
}

Point& Point::Left() {
    x--;
    return *this;
}

bool Point::TryUp(const Point& Z) {
    return TryUp(Point::origin(), Z);
}

bool Point::TryUp(const Point& O, const Point& Z) {
    Point N(*this);
    N.Up();
    if (!N.inBounds(O, Z)) {
        return false;
    }
    *this = N;
    return true;
}

bool Point::TryDown(const Point& Z) {
    return TryDown(Point::origin(), Z);
}

bool Point::TryDown(const Point& O, const Point& Z) {
    Point N(*this);
    N.Down();
    if (!N.inBounds(O, Z)) {
        return false;
    }
    *this = N;
    return true;
}

bool Point::TryRight(const Point& Z) {
    return TryRight(Point::origin(), Z);
}

bool Point::TryRight(const Point& O, const Point& Z) {
    Point N(*this);
    N.Right();
    if (!N.inBounds(O, Z)) {
        return false;
    }
    *this = N;
    return true;
}

bool Point::TryLeft(const Point& Z) {
    return TryLeft(Point::origin(), Z);
}

bool Point::TryLeft(const Point& O, const Point& Z) {
    Point N(*this);
    N.Left();
    if (!N.inBounds(O, Z)) {
        return false;
    }
    *this = N;
    return true;
}
std::ostream& operator<< (std::ostream& stream, const Point & point) {
    stream << "(" << point.x << "; " << point.y << ")";
    return stream;
}
bool cmpX(const Point & a, const Point & b) {
    return a.x < b.x;
}
bool cmpY(const Point & a, const Point & b) {
    return a.y < b.y;
}       
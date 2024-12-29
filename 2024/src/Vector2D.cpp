#include "..\include\Vector2D.h"


Vector2D::Vector2D(long long x, long long y) : x(x), y(y) {
}

// Method to calculate the determinant of two vectors
long long Vector2D::determinant(const Vector2D& other) const {
    return x * other.y - y * other.x;
}
// Method to calculate the determinant of two vectors
long long Vector2D::determinant(const Vector2D& a, const Vector2D& b) {
    return a.determinant(b);
}

// Multiplication operation accepting a long long parameter
Vector2D Vector2D::operator*(long long scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

// Addition operator accepting another vector as parameter
Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

// Subtraction operator
Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

// Get the magnitude (length) of the vector
long double Vector2D::magnitude() const {
    return sqrt(x * x + y * y);
}
// Get the direction of the vector
long double Vector2D::direction() const {
    return atan2(y, x);
}

// Method to test collinearity with another vector
bool Vector2D::isCollinear(const Vector2D& other) const {
    return determinant(other) == 0;
}

// Method to check if a point belongs to a line defined by this vector and a position vector
bool Vector2D::isPointOnLine(const Vector2D& point, const Vector2D& position) const {
    Vector2D relativePoint = point - position;
    return isCollinear(relativePoint);
}

// Method to get the normal vector
Vector2D Vector2D::normal() const {
    return Vector2D(-y, x);
}

// Find the opposite vector (same direction, same magnitude but extends in the opposite direction)
Vector2D Vector2D::opposite() const {
    return Vector2D(-x, -y);
}


// Method to print the vector (for debugging purposes)
void Vector2D::print() const {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

bool FindIntersectionExact(const Vector2D& m, const Vector2D& a, const Vector2D& b, Vector2D& result) {
    long long det = a.determinant(b);
    if (det == 0) {
        return false; // No solution if vectors a and b are collinear
    }
    auto _x = m.determinant(b);
    auto _y = a.determinant(m);
    if(_x % det != 0 || _y % det != 0){
        return false;
    }
    result.x = _x / det;
    result.y = _y / det;
    return true;
}


bool FindIntersectionExact(const Vector2D& m, const Vector2D& a, const Vector2D& b, std::pair<long double,long double> & result) {
    long long det = a.determinant(b);
    if (det == 0) {
        return false; // No solution if vectors a and b are collinear
    }
    result.first = (long double)(m.determinant(b)) / (long double)det;
    result.second = (long double)(a.determinant(m)) / (long double)det;
    return true;
}


std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

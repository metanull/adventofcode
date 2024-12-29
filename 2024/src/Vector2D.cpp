#include "..\include\Vector2D.h"


Vector2D::Vector2D(long long x, long long y) : x(x), y(y) {
}

Vector2D Vector2D::Origin() {
    return Vector2D(0,0);
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
    Vector2D v(x,y);
    v *= scalar;
    return v;
}

// Operator *=
Vector2D& Vector2D::operator*=(long long scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// Test if Integer Division is possible
bool Vector2D::isDivisible(long long scalar) const {
    return x % scalar == 0 && y % scalar == 0;
}

// Division operation accepting a long long parameter
Vector2D Vector2D::operator/(long long scalar) const {
    Vector2D v(x,y);
    v /= scalar;
    return v;
}

// Operator /=
Vector2D& Vector2D::operator/=(long long scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

// Addition operator accepting another vector as parameter
Vector2D Vector2D::operator+(const Vector2D& other) const {
    Vector2D v(x,y);
    v += other;
    return v;
}

// Operator +=
Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// Subtraction operator
Vector2D Vector2D::operator-(const Vector2D& other) const {
    Vector2D v(x,y);
    v -= other;
    return v;
}

// Operator -=
Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
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




Line2D::Line2D(const Vector2D& direction, const Vector2D& position) : direction(direction), position(position) {
}

// Method to check if a point belongs to the line
bool Line2D::isPointOnLine(const Vector2D& point) const {
    return direction.isCollinear(point - position);
}

bool Line2D::isCollinear(const Line2D& other) const {
    return direction.isCollinear(other.direction);
}
bool Line2D::isParallel(const Line2D& other) const {
    return direction.isCollinear(other.direction) && !direction.isCollinear(other.position - position);
}
bool Line2D::isPerpendicular(const Line2D& other) const {
    return direction.isCollinear(other.direction.normal());
}

// Method to calculate the intersection point between two lines
bool Line2D::intersection(const Line2D& other, Vector2D& result) const {
    return FindIntersectionExact(direction, other.direction, position - other.position, result);
}
bool Line2D::intersection(const Line2D& other, std::pair<long double,long double>& result) const {
    return FindIntersectionExact(direction, other.direction, position - other.position, result);
}

bool Line2D::operator==(const Line2D& other) const {
    return isCollinear(other) && isPointOnLine(other.position);
}

// Method to find the distance between a point and a line
long double Line2D::distance(const Vector2D& point) const {
    return direction.normal().determinant(point - position) / direction.normal().magnitude();
}

// Find the projection of a point on a line
std::pair<long double,long double> Line2D::projection(const Vector2D& point) const {
    Vector2D relativePoint = point - position;
    long double projectionLength = relativePoint.determinant(direction) / direction.magnitude();

    return std::make_pair((long double)position.x + (long double)direction.x * projectionLength / direction.magnitude(), (long double)position.y + (long double)direction.y * projectionLength / direction.magnitude());
}


// Get a perpendicular line passing through the position point of this line
inline Line2D Line2D::perpendicular() const {
    return Line2D(direction.normal(), position);
}

// Get a perpendicular line at a given point
inline Line2D Line2D::perpendicular(const Vector2D& point) const {
    return Line2D(direction.normal(), point);
}

// Method to print the line (for debugging purposes)
void Line2D::print() const {
    std::cout << "Line: " << direction << " + t" << position << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Line2D& l) {
    os << "Line: " << l.direction << " + t" << l.position;
    return os;
}
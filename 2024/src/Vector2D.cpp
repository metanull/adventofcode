#include "..\include\Vector2D.h"


Vector2D::Vector2D(long long x, long long y) : x(x), y(y) {

}

// Method to calculate the determinant of two vectors

long long Vector2D::determinant(const Vector2D& other) const {
    return x * other.y - y * other.x;
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

// Method to print the vector (for debugging purposes)

void Vector2D::print() const {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

// Friend function to solve for coefficients

bool solveForCoefficients(const Vector2D& m, const Vector2D& a, const Vector2D& b, Vector2D& result) {
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


bool solveForCoefficients(const Vector2D& m, const Vector2D& a, const Vector2D& b, std::pair<double,double> & result) {
    long long det = a.determinant(b);
    if (det == 0) {
        return false; // No solution if vectors a and b are collinear
    }
    result.first = (m.determinant(b)) / det;
    result.second = (a.determinant(m)) / det;
    return true;
}


std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

/*
int main() {
Vector2D<int> v1(3, 4);
Vector2D<int> v2(6, 8);
Vector2D<int> point(9, 12);
Vector2D<int> position(0, 0);

// Test determinant
std::cout << "Determinant: " << v1.determinant(v2) << std::endl;

// Test multiplication
Vector2D<int> v3 = v1 * 2;
v3.print();

// Test addition
Vector2D<int> v4 = v1 + v2;
v4.print();

// Test collinearity
std::cout << "Are v1 and v2 collinear? " << (v1.isCollinear(v2) ? "Yes" : "No") << std::endl;

// Test if point is on line
std::cout << "Is point on line? " << (v1.isPointOnLine(point, position) ? "Yes" : "No") << std::endl;

// Test normal vector
Vector2D<int> normal = v1.normal();
normal.print();

// Test solving for coefficients
Vector2D<int> m(9, 12);
Vector2D<int> a(3, 4);
Vector2D<int> b(6, 8);
long long first, second;
if (solveForCoefficients(m, a, b, first, second)) {
    std::cout << "Solution: " << first << " * a + " << second << " * b = m" << std::endl;
} else {
    std::cout << "No solution found." << std::endl;
}

return 0;
}

*/
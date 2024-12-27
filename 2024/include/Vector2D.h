#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

class Vector2D {
public:

    long long x = (long long)0;
    long long y = (long long)0;
 
    Vector2D(long long x = 0, long long y = 0);

    // Method to calculate the determinant of two vectors
    long long determinant(const Vector2D& other) const;

    // Multiplication operation accepting a long long parameter
    Vector2D operator*(long long scalar) const;

    // Addition operator accepting another vector as parameter
    Vector2D operator+(const Vector2D& other) const;

    // Subtraction operator
    Vector2D operator-(const Vector2D& other) const;

    // Method to test collinearity with another vector
    bool isCollinear(const Vector2D& other) const;

    // Method to check if a point belongs to a line defined by this vector and a position vector
    bool isPointOnLine(const Vector2D& point, const Vector2D& position) const;

    // Method to get the normal vector
    Vector2D normal() const;

    // Method to print the vector (for debugging purposes)
    void print() const;

    // Friend function to solve for coefficients
    /*
    * Solves the system of equations:
    * m = a * first + b * second    (where m is the OM vector, a and b are the vectors to be combined)
    * Returns false if the vectors a and b are collinear; or if the solution would not fit in the long long type (e.g. i long long is an integer and the solution is a double)
    * Returns true and sets the values of result otherwise
    */
    friend bool solveForCoefficients(const Vector2D& m, const Vector2D& a, const Vector2D& b, Vector2D& result);
    /*
    * Solves the system of equations:
    * m = a * first + b * second    (where m is the OM vector, a and b are the vectors to be combined)
    * Returns false if the vectors a and b are collinear
    * Returns true and sets the values of result otherwise
    */
    friend bool solveForCoefficients(const Vector2D& m, const Vector2D& a, const Vector2D& b, std::pair<double,double>& result);

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
};

#endif // VECTOR2D_H
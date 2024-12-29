
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

/*
* Class to represent a 2D vector or a 2D point
* Indeed a point can be represented as a vector from the origin to the point
*/
class Vector2D {
public:

    long long x = (long long)0;
    long long y = (long long)0;
 
    Vector2D() = default;
    Vector2D(long long x, long long y);
    Vector2D(const Vector2D& other) = default;
    Vector2D& operator=(const Vector2D& other) = default;

    // Method to calculate the determinant of two vectors
    long long determinant(const Vector2D& other) const;
    static long long determinant(const Vector2D& a, const Vector2D& b);

    // Get the magnitude (length) of the vector
    long double magnitude() const;
    // Get the direction of the vector
    long double direction() const;

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

    // Find the opposite vector (same direction, same magnitude but extends in the opposite direction)
    Vector2D opposite() const;

    // Method to print the vector (for debugging purposes)
    void print() const;

    /*
    * Considering a point o (the origin) and and point m; verify if the OM vector can be obtained by 
    * adding multiples of the vectors a and b.
    * In other words, find the intersection between:
    * - a line A, defined by the point o and vector a; and 
    * - a line B, defined by the point m and vector b.
    * Return that point
    * 
    * Returns false if the vectors a and b are collinear
    * Returns false if the solution is not exact (e.g. coordinates are do not fit in the long long type (e.g. i long long is an integer and the solution is a double)
    * Returns true and sets the values of result otherwise
    */
    friend bool FindIntersectionExact(const Vector2D& m, const Vector2D& a, const Vector2D& b, Vector2D& result);
    /*
    * Considering a point o (the origin) and and point m; verify if the OM vector can be obtained by 
    * adding multiples of the vectors a and b.
    * In other words, find the intersection between:
    * - a line A, defined by the point o and vector a; and 
    * - a line B, defined by the point m and vector b.
    * Return that point
    * 
    * Returns false if the vectors a and b are collinear
    * Returns true and sets the values of result otherwise
    */
    friend bool FindIntersection(const Vector2D& m, const Vector2D& a, const Vector2D& b, std::pair<long double,long double>& result);

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
};

/*
class Line2D {
public:
    // Direction vector of the line
    Vector2D direction;
    // A Point of the line (position vector)
    Vector2D position;

    Line2D() = default;
    Line2D(const Line2D& other) = default;
    Line2D& operator=(const Line2D& other) = default;
    Line2D(const Vector2D& direction, const Vector2D& position);

    // Method to check if a point belongs to the line
    bool isPointOnLine(const Vector2D& point) const;

    bool isCollinear(const Line2D& other) const;
    bool isParallel(const Line2D& other) const;
    bool isPerpendicular(const Line2D& other) const;

    // Method to calculate the intersection point between two lines
    bool intersection(const Line2D& other, Vector2D& result) const;
    bool intersection(const Line2D& other, std::pair<long double,long double>& result) const;
    
    bool operator==(const Line2D& other) const;

    // Method to find the distance between a point and a line
    long double distance(const Vector2D& point) const;

    // Find the projection of a point on a line
    Vector2D projection(const Vector2D& point) const;


    // Get a perpendicular line passing through the position point of this line
    inline Line2D perpendicular() const {
        return Line2D(direction.normal(), position);
    }

    // Get a perpendicular line at a given point
    inline Line2D perpendicular(const Vector2D& point) const {
        return Line2D(direction.normal(), point);
    }

    // Method to print the line (for debugging purposes)
    void print() const;

    friend std::ostream& operator<<(std::ostream& os, const Line2D& l); 
};
*/

#endif // VECTOR2D_H
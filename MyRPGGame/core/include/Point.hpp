#pragma once
#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>
#include "Constants.h"
#include <cmath>

class Point {
private:
    real x;
    real y;
    // adding height element for jumping, fall damage, etc.
    real z;
    
public:
    Point() : x(0), y(0), z(0) {}
    Point(real x, real y, real Z = 0);
    ~Point() = default;

    real getX();
    real getY();
    real getZ();
    void setX(real X);
    void setY(real Y);
    void setZ(real Z);
    real distance(Point p) const;
    int operator-(const Point &other) const;
//    bool operator==(const Point& other) const;
};

#endif /* Point_hpp */

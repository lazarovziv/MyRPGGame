#pragma once
#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include <cmath>
#include "Point.hpp"

class Circle {
private:
    Point *center = nullptr;
    real radius;
    
public:
    Circle();
    Circle(real x, real y, real r);
    explicit Circle(Point *center, real r);
    ~Circle();
    
    Point* getCenter();
    real getRadius() const;
    void setRadius(real r);
    void setCenter(real x, real y);
    void setCenter(Point *newCenter);
    bool intersects(Circle* c);

    bool isPointInCircle(Point *point);

    bool operator==(const Circle& other) const;
};

#endif /* Circle_hpp */

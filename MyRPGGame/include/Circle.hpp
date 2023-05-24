#pragma once
#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include <cmath>
#include "Point.hpp"

class Circle {
private:
    Point *center = nullptr;
    float radius;
    
public:
    Circle();
    Circle(int x, int y, float r);
    explicit Circle(Point *center, float r);
    ~Circle();
    
    Point* getCenter();
    float getRadius() const;
    void setRadius(float r);
    void setCenter(int x, int y);
    void setCenter(Point *newCenter);
    bool intersects(Circle* c);

    bool isPointInCircle(Point *point);

    bool operator==(const Circle& other) const;
};

#endif /* Circle_hpp */

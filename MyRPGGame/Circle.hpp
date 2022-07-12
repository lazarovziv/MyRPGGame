#pragma once
#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include "Point.hpp"

class Circle {
private:
    Point* center;
    float radius;
    
public:
    Circle();
    Circle(float x, float y, float radius);
    ~Circle() = default;
    
    Point* getCenter();
    float getRadius();
    void setRadius(float r);
    bool intersects(Circle c);
};

#endif /* Circle_hpp */

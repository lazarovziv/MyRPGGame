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
    Circle(float x, float y, float r);
    ~Circle() = default;
    
    Point* getCenter();
    float getRadius();
    void setRadius(float r);
    void setCenter(int x, int y);
    bool intersects(Circle* c);
};

#endif /* Circle_hpp */

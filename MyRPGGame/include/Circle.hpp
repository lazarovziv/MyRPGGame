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
    ~Circle();
    
    Point* getCenter();
    float getRadius();
    void setRadius(float r);
    void setCenter(int x, int y);
    bool intersects(Circle* c);
};

#endif /* Circle_hpp */

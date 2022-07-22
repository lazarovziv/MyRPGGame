#pragma once
#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>

class Point {
private:
    float x;
    float y;
    
public:
    Point();
    Point(float x, float y);
    ~Point() = default;
    
    float getX();
    float getY();
    void setX(float X);
    void setY(float Y);
    float distance(Point p);
};

#endif /* Point_hpp */

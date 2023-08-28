#pragma once
#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>
#include <cmath>

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
    float distance(Point p) const;
    int operator-(const Point &other) const;
//    bool operator==(const Point& other) const;
};

#endif /* Point_hpp */

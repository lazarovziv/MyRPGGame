#pragma once
#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>
#include <cmath>

class Point {
private:
    int x;
    int y;
    
public:
    Point();
    Point(int x, int y);
    ~Point() = default;
    
    int getX();
    int getY();
    void setX(int X);
    void setY(int Y);
    float distance(Point p);
};

#endif /* Point_hpp */

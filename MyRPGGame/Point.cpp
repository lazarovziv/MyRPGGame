#include "Point.hpp"
#include <cmath>

Point::Point(float X, float Y) {
    x = X;
    y = Y;
}

float Point::getX() {
    return x;
}

float Point::getY() {
    return y;
}

void Point::setX(float X) {
    x = X;
}

void Point::setY(float Y) {
    y = Y;
}

float Point::distance(Point p) {
    return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
}

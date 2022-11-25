#include "Point.hpp"
#include <cmath>

Point::Point(int X, int Y) {
    x = X;
    y = Y;
}

int Point::getX() {
    return x;
}

int Point::getY() {
    return y;
}

// memory leak, use pointers
void Point::setX(int X) {
    x = X;
}
// likewise
void Point::setY(int Y) {
    y = Y;
}
// probably here too
float Point::distance(Point p) {
    return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
}

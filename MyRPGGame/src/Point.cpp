#include "../include/Point.hpp"

Point::Point() {
    x = 0;
    y = 0;
}

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

int Point::operator-(const Point &other) const {
    return abs(x - other.x) + abs(y - other.y);
}

bool Point::isEqual(Point *other) {
    return x == other->getX() && y == other->getY();
}

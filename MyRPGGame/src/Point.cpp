#include "../include/Point.hpp"

Point::Point() {
    x = 0;
    y = 0;
}

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

// memory leak, use pointers
void Point::setX(float X) {
    x = X;
}
// likewise
void Point::setY(float Y) {
    y = Y;
}
// probably here too
float Point::distance(Point p) const {
    return (float) sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
}

int Point::operator-(const Point &other) const {
    return abs(x - other.x) + abs(y - other.y);
}

/*
bool Point::operator==(const Point &other) const {
    return x == other.x && y == other.y;
}*/

#include "Point.hpp"

Point::Point(real X, real Y, real Z) {
    x = X;
    y = Y;
    z = Z;
}

real Point::getX() {
    return x;
}

real Point::getY() {
    return y;
}

real Point::getZ() {
    return z;
}

// memory leak, use pointers
void Point::setX(real X) {
    x = X;
}
// likewise
void Point::setY(real Y) {
    y = Y;
}

void Point::setZ(real Z) {
    z = Z;
}
// probably here too
real Point::distance(Point p) const {
    return (real) sqrt(pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2));
}

int Point::operator-(const Point &other) const {
    return abs(x - other.x) + abs(y - other.y) + abs(z - other.z);
}

/*
bool Point::operator==(const Point &other) const {
    return x == other.x && y == other.y;
}*/

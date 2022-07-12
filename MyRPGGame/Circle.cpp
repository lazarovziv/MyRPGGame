#include "Circle.hpp"
#include <cmath>

Circle::Circle(float x, float y, float radius) {
    center = new Point(x, y);
    this->radius = radius;
}

Point* Circle::getCenter() {
    return center;
}

float Circle::getRadius() {
    return radius;
}

void Circle::setRadius(float r) {
    radius = r;
}

bool Circle::intersects(Circle c) {
    float distance = center->distance(*(c.getCenter()));
    // they don't intersect
    if (distance > radius + c.radius) return false;
    // they intersect
    if (distance < radius + c.radius) return true;
    // if one is containes within another (shouldn't reach this point
    if (distance < abs(radius - c.radius)) return true;
    return false;
}

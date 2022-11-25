#include "../include/Circle.hpp"
#include <cmath>

Circle::Circle(int x, int y, float r) {
    center = new Point(x, y);
    radius = r;
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

void Circle::setCenter(int x, int y) {
    center->setX(x);
    center->setY(y);
}

bool Circle::intersects(Circle* c) {
    float distance = center->distance(*(c->getCenter()));
    // they don't intersect
    if (distance > radius + c->radius) return false;
    // they intersect
    if (distance < radius + c->radius) return true;
    // if one is contains within another (shouldn't reach this point
    if (distance < std::abs(radius - c->radius)) return true;
    return false;
}

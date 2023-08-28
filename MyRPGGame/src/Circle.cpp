#include "../include/Circle.hpp"


Circle::Circle(float x, float y, float r) {
    center = new Point(x, y);
    radius = r;
}

Circle::Circle(Point *center, float r) {
    this->center = center;
    radius = r;
}

Circle::~Circle() {
    delete center;
}

Point* Circle::getCenter() {
    return center;
}

float Circle::getRadius() const {
    return radius;
}

void Circle::setRadius(float r) {
    radius = r;
}

void Circle::setCenter(float x, float y) {
    center->setX(x);
    center->setY(y);
}

void Circle::setCenter(Point *newCenter) {
    center = newCenter;
}

bool Circle::intersects(Circle* c) {
    float distance = center->distance(*(c->getCenter()));
    // they don't intersect
    if (distance > radius + c->radius) return false;
    // they intersect
    if (distance <= radius + c->radius) return true;
    // if one is contains within another (shouldn't reach this point but keeping it)
    if (distance < std::abs(radius - c->radius)) return true;
    return false;
}

bool Circle::isPointInCircle(Point *point) {
    return center->distance(*point) <= radius;
}

bool Circle::operator==(const Circle &other) const {
    return center == other.center && radius == other.radius;
}

#include "../include/RigidBody.hpp"

namespace physics {

    RigidBody::RigidBody(RigidBodyType type, real x, real y, real z) {
        position = Vector(x, y, z);
        restitution = 1;
        inverseMass = 0; // infinite mass
        bodyType = type;
        damping = 0.995;
    }

    Vector RigidBody::getVelocity() const {
        return velocity;
    }

    Vector RigidBody::getAcceleration() const {
        return acceleration;
    }

    Vector RigidBody::getPosition() const {
        return position;
    }

    real RigidBody::getRestitution() const {
        return restitution;
    }

    real RigidBody::getInverseMass() const {
        return inverseMass;
    }

    real RigidBody::getMass() const {
        // referring to infinite mass as 0 or something
        if (inverseMass == 0) return 0;
        return 1/inverseMass;
    }

    RigidBodyType RigidBody::getBodyType() const {
        return bodyType;
    }

    bool RigidBody::hasFiniteMass() const {
        return inverseMass != 0;
    }

    void RigidBody::setPosition(const real x, const real y, const real z) {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    void RigidBody::setPosition(const physics::Vector &other) {
        position.x = other.x;
        position.y = other.y;
        position.z = other.z;
    }

    void RigidBody::setMass(const real mass) {
        if (mass == 0) {
            throw std::runtime_error("Division by zero!");
        }
        inverseMass = 1 / mass;
    }

    void RigidBody::setRestitution(const real e) {
        restitution = e;
    }

    void RigidBody::incrementVelocity(const Vector &v) {
        velocity += v;
    }

    void RigidBody::incrementAcceleration(const real amount) {
        acceleration += amount;
    }

    void RigidBody::resetForceAccumulator() {
        forceAccumulator.x = 0;
        forceAccumulator.y = 0;
        forceAccumulator.z = 0;
    }

    // needs to be called before the rigid body is integrated (before changing its position)
    void RigidBody::addForce(const physics::Vector &force) {
        forceAccumulator += force;
    }

    void RigidBody::operator +=(const Vector &other) {
        position += other;
    }

    real RigidBody::distance(const physics::RigidBody &other) const {
        return (real) sqrt(pow(position.x - other.position.y, 2) + pow(position.y - other.position.y, 2));
    }

    void RigidBody::update(const real dt) {
        // updating infinite mass bodies is irrelevant
        if (inverseMass <= 0) return;
        // applying forces to the position
        position += velocity * dt + acceleration * dt * dt * (real) 0.5;

        Vector resultingAcceleration = acceleration;
        resultingAcceleration += forceAccumulator * inverseMass;
        velocity += resultingAcceleration * dt;
        // drag
        velocity *= pow(damping, dt);

        resetForceAccumulator();
    }

    Circle::Circle(real x, real y, real z, real r) : RigidBody(RigidBodyType::CIRCLE, x, y, z) {
        radius = r;
    }

    real Circle::getRadius() const {
        return Circle::radius;
    }

    Box Box::RIGHT_END_SCREEN = Box{(real) Constants::FULL_SCREEN_WIDTH + 0.5,
                                    (real) Constants::FULL_SCREEN_HEIGHT/2,
                                    Constants::REAL_MAX, // very tall
                                    (real) 1,
                                    (real) Constants::FULL_SCREEN_HEIGHT};
    Box Box::LEFT_END_SCREEN = Box{(real) -0.5,
                                    (real) Constants::FULL_SCREEN_HEIGHT/2,
                                    Constants::REAL_MAX, // very tall
                                    (real) 1,
                                    (real) Constants::FULL_SCREEN_HEIGHT};
    Box Box::TOP_END_SCREEN = Box{(real) Constants::FULL_SCREEN_WIDTH/2,
                                    (real) -0.5,
                                    Constants::REAL_MAX, // very tall
                                    (real) Constants::FULL_SCREEN_WIDTH,
                                    (real) 1};
    Box Box::BOTTOM_END_SCREEN = Box{(real) Constants::FULL_SCREEN_WIDTH/2,
                                  (real) Constants::FULL_SCREEN_HEIGHT + 0.5,
                                  Constants::REAL_MAX, // very tall
                                  (real) Constants::FULL_SCREEN_WIDTH,
                                  (real) 1};

    Box::Box(real x, real y, real z, real width, real height, real depth) : RigidBody(RigidBodyType::BOX, x, y, z) {
        this->width = width;
        this->height = height;
    }

    real Box::getWidth() const {
        return Box::width;
    }

    real Box::getHeight() const {
        return Box::height;
    }

    real Box::getDepth() const {
        return Box::depth;
    }

    Vector Box::getTopRightCornerPosition() {
        return Vector{Box::position.x + Box::width/2, Box::position.y + Box::height/2, Box::position.z + Box::depth/2};
    }

    Vector Box::getBottomLeftCornerPosition() {
        return Vector{Box::position.x - Box::width/2, Box::position.y - Box::height/2, Box::position.z - Box::depth/2};
    }
}

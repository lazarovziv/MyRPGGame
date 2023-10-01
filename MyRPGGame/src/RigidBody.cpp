#include "../include/RigidBody.hpp"

namespace physics {

    RigidBody::RigidBody(RigidBodyType type, real x, real y, real z) {
        position = std::make_unique<Vector>(x, y, z);
        velocity = std::make_unique<Vector>(0, 0);
        acceleration = std::make_unique<Vector>(0, 0);
        forceAccumulator = std::make_unique<Vector>(0, 0);
        restitution = 1;
        inverseMass = (real) 0; // infinite mass
        bodyType = type;
        damping = 0.995;
    }

    Vector RigidBody::getVelocity() {
        return (*velocity);
    }

    Vector RigidBody::getAcceleration() {
        return (*acceleration);
    }

    Vector RigidBody::getPosition() {
        return (*position);
    }

    real RigidBody::getRestitution() const {
        return restitution;
    }

    real RigidBody::getInverseMass() const {
        return inverseMass;
    }

    real RigidBody::getMass() const {
        // referring to infinite mass as 0 or something
        if (infiniteMass) return 0;
        return 1/inverseMass;
    }

    RigidBodyType RigidBody::getBodyType() const {
        return bodyType;
    }

    bool RigidBody::hasFiniteMass() const {
        return !infiniteMass;
    }

    void RigidBody::setPosition(const real x, const real y, const real z) {
        position->x = x;
        position->y = y;
        position->z = z;
    }

    void RigidBody::setPosition(const physics::Vector &other) {
        position->x = other.x;
        position->y = other.y;
        position->z = other.z;
    }

    void RigidBody::setMass(const real mass) {
        if (mass == 0) {
            throw std::runtime_error("Division by zero!");
        }
        inverseMass = 1 / mass;
        // just updated mass to be finite
        infiniteMass = false;
    }

    void RigidBody::setRestitution(const real e) {
        restitution = e;
    }

    void RigidBody::incrementVelocity(const Vector &v) {
        (*velocity) += v;
    }

    void RigidBody::incrementAcceleration(const real amount) {
        (*acceleration) += amount;
    }

    void RigidBody::resetForceAccumulator() {
        forceAccumulator->resetCoordinates();
    }

    // needs to be called before the rigid body is integrated (before changing its position)
    void RigidBody::addForce(const physics::Vector &force) {
        (*forceAccumulator) += force;
    }

    void RigidBody::operator +=(const Vector &other) {
        (*position) += other;
    }

    real RigidBody::distance(const physics::RigidBody &other) const {
        return position->distance((*other.position));
    }

    void RigidBody::update(const real dt) {
        // updating infinite mass bodies is irrelevant
        if (infiniteMass) {
            std::cout << "Infinite mass" << std::endl;
            return;
        }
        // applying forces to the position
        (*position) += (*velocity) * dt + (*acceleration) * dt * dt * (real) 0.5;

        Vector resultingAcceleration = (*acceleration);
        resultingAcceleration += (*forceAccumulator) * inverseMass;
        (*velocity) += resultingAcceleration * dt;
        // drag
        (*velocity) *= pow(damping, dt);

        resetForceAccumulator();
    }

    Circle::Circle(real x, real y, real z, real r) : RigidBody(RigidBodyType::CIRCLE, x, y, z) {
        radius = r;
    }

    real Circle::getRadius() const {
        return Circle::radius;
    }

    Polygon Polygon::RIGHT_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH + 0.5,
                                    Constants::FULL_SCREEN_HEIGHT/2,
                                    Constants::REAL_MAX, // very tall
                                                { // vertices of the polygon (rectangle)
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH+1, Constants::FULL_SCREEN_HEIGHT}, // bottom right
        Vector{Constants::FULL_SCREEN_WIDTH, (real) 0}, // top left
        Vector{Constants::FULL_SCREEN_WIDTH+1, (real) 0}, // top right
                                                }};
    Polygon Polygon::LEFT_END_SCREEN = Polygon{(real) -0.5,
                                    Constants::FULL_SCREEN_HEIGHT/2,
                                    Constants::REAL_MAX, // very tall
                                               { // vertices of the polygon (rectangle)
        Vector{(real) -1, Constants::FULL_SCREEN_HEIGHT}, // bottom left
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT}, // bottom right
        Vector{(real) -1, (real) 0}, // top left
        Vector{(real) 0, (real) 0}, // top right
                                               }};
    Polygon Polygon::TOP_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH/2,
                                    (real) -0.5,
                                    Constants::REAL_MAX, // very tall
                                              { // vertices of the polygon (rectangle)
        Vector{(real) 0, (real) 0}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH, (real) 0}, // bottom right
        Vector{(real) 0, (real) -1}, // top left
        Vector{Constants::FULL_SCREEN_WIDTH, (real) -1}, // top right
                                              }};
    Polygon Polygon::BOTTOM_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH/2,
                                  Constants::FULL_SCREEN_HEIGHT + 0.5,
                                  Constants::REAL_MAX, // very tall
                                                 { // vertices of the polygon (rectangle)
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT+1}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT+1}, // bottom right
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT}, // top left
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // top right
                                                 }};

    Polygon::Polygon(const real x, const real y, const real z, const std::vector<Vector> &newVertices)
    : RigidBody(RigidBodyType::POLYGON, x, y, z) {
        Polygon::vertices = std::make_unique<std::vector<Vector>>();
        for (auto &v : newVertices) {
            Polygon::vertices->push_back(v);
        }
    }

    std::vector<Vector>* Polygon::getVertices() {
        return Polygon::vertices.get();
    }
}

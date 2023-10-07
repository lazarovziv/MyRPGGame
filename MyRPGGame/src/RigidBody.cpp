#include "../include/RigidBody.hpp"

namespace physics {

    RigidBody::RigidBody(RigidBodyType type, real x, real y, real z, real mass) {
        position = std::make_unique<Vector>(x, y, z);
        velocity = std::make_unique<Vector>(0, 0);
        acceleration = std::make_unique<Vector>(0, 0);
        forceAccumulator = std::make_unique<Vector>(0, 0);
        restitution = 0.69;
        setMass(mass);
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

    real RigidBody::getMass() const {
        return mass;
    }

    real RigidBody::getInverseMass() const {
        return inverseMass;
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
        if (mass == Constants::REAL_MAX) {
            this->mass = mass;
            inverseMass = 0;
            infiniteMass = true;
            return;
        }
        this->mass = mass;
        inverseMass = 1 / mass;
        // just updated mass to be finite
        infiniteMass = false;
    }

    void RigidBody::scaleVelocity(const real amount) {
        (*velocity) *= amount;
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

    void RigidBody::incrementAcceleration(const Vector amount) {
        (*acceleration) += amount;
    }

    void RigidBody::resetForceAccumulator() {
        forceAccumulator->resetCoordinates();
    }

    // needs to be called before the rigid body is integrated (before changing its position)
    void RigidBody::addForce(const physics::Vector &force) {
        (*forceAccumulator) += force; // * mass * Constants::RIGID_BODY_FORCE_SCALE;
    }

    void RigidBody::resetVelocity() {
        velocity->resetCoordinates();
    }

    void RigidBody::resetAcceleration() {
        acceleration->resetCoordinates();
    }

    void RigidBody::operator +=(const Vector &other) {
        if (RigidBody::infiniteMass) return;
        (*position) += other;
    }

    real RigidBody::distance(const physics::RigidBody &other) const {
        return position->distance((*other.position));
    }

    void RigidBody::update(const real dt) {
        // updating infinite mass bodies is irrelevant
        if (infiniteMass) return;
        // acceleration = force * mass
        Vector resultingAcceleration = (*acceleration);
        resultingAcceleration += (*forceAccumulator) * inverseMass;
        (*velocity) += resultingAcceleration * dt; // * dt
        // TODO: clamp velocity
        // drag
        (*velocity) *= pow(damping, dt);
        // applying forces to the position
        (*position) += (*velocity) * dt + (*acceleration) * dt * dt * (real) 0.5;
        // defined friction
        scaleVelocity((real) 1/Constants::FRICTION_DEGRADATION_CONSTANT);
        // clamping velocity
        if (velocity->magnitude() < Constants::VELOCITY_MAGNITUDE_MIN) velocity->resetCoordinates();
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
        Vector{Constants::FULL_SCREEN_WIDTH+Polygon::MAP_POLYGON_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // bottom right
        Vector{Constants::FULL_SCREEN_WIDTH, (real) 0}, // top left
        Vector{Constants::FULL_SCREEN_WIDTH+Polygon::MAP_POLYGON_WIDTH, (real) 0}, // top right
                                                },
                                                Constants::REAL_MAX};
    Polygon Polygon::LEFT_END_SCREEN = Polygon{(real) -0.5,
                                    Constants::FULL_SCREEN_HEIGHT/2,
                                    Constants::REAL_MAX, // very tall
                                               { // vertices of the polygon (rectangle)
        Vector{(real) -Polygon::MAP_POLYGON_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // bottom left
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT}, // bottom right
        Vector{(real) -Polygon::MAP_POLYGON_WIDTH, (real) 0}, // top left
        Vector{(real) 0, (real) 0}, // top right
                                               },
                                               Constants::REAL_MAX};
    Polygon Polygon::TOP_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH/2,
                                              (real) -0.5,
                                            Constants::REAL_MAX, // very tall
                                              { // vertices of the polygon (rectangle)
        Vector{(real) 0, (real) 0}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH, (real) 0}, // bottom right
        Vector{(real) 0, (real) -Polygon::MAP_POLYGON_HEIGHT}, // top left
        Vector{Constants::FULL_SCREEN_WIDTH, (real) -Polygon::MAP_POLYGON_HEIGHT}, // top right
                                              },
                                              Constants::REAL_MAX};
    Polygon Polygon::BOTTOM_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH/2,
                                  Constants::FULL_SCREEN_HEIGHT + 0.5,
                                  Constants::REAL_MAX, // very tall
                                                 { // vertices of the polygon (rectangle)
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT+Polygon::MAP_POLYGON_HEIGHT}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT+Polygon::MAP_POLYGON_HEIGHT}, // bottom right
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT}, // top left
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // top right
                                                 },
                                                 Constants::REAL_MAX};

    Polygon::Polygon(const real x, const real y, const real z, const std::vector<Vector> &newVertices, const real mass)
    : RigidBody(RigidBodyType::POLYGON, x, y, z, mass) {
        Polygon::vertices = std::make_unique<std::vector<Vector>>();
        for (auto &v : newVertices) {
            Polygon::vertices->push_back(v);
        }
    }

    std::vector<Vector>* Polygon::getVertices() {
        return Polygon::vertices.get();
    }
}

#include "../include/RigidBody.hpp"

namespace physics {

    RigidBody::RigidBody() {
        restitution = 1;
        inverseMass = 0; // infinite mass
    }

    Vector RigidBody::getVelocity() {
        return velocity;
    }

    Vector RigidBody::getAcceleration() {
        return acceleration;
    }

    real RigidBody::getRestitution() {
        return restitution;
    }

    real RigidBody::getInverseMass() {
        return inverseMass;
    }

    void RigidBody::setMass(real mass) {
        if (mass == 0) {
            throw std::runtime_error("Division by zero!");
        }
        inverseMass = 1 / mass;
    }

    void RigidBody::incrementVelocity(physics::Vector &v) {
        velocity += v;
    }

    void RigidBody::incrementAcceleration(physics::Vector &v) {
        acceleration += v;
    }
}

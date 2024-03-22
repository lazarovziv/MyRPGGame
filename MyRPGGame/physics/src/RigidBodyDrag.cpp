#include "../include/RigidBodyDrag.hpp"

namespace physics {

    void RigidBodyDrag::update(physics::RigidBody *body, real dt) {
        Vector dragForce = body->getVelocity();
        // calculating the drag force scalar
        real dragCoefficient = dragForce.magnitude();
        dragCoefficient = k1 * dragCoefficient + k2 * dragCoefficient * dragCoefficient;
        // getting the force opposite direction
        dragForce = dragForce.normalized();
        dragForce *= -dragCoefficient;
        // applying to the body
        body->addForce(dragForce);
    }
}

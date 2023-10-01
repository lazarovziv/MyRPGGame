#include "../include/Collision.hpp"

namespace physics {

    bool physics::resolveCollisions(physics::RigidBody *first, physics::RigidBody *second, real dt) {
        // getting vector between positions
        Vector separator = second->getPosition() - first->getPosition();
        Vector relativeDirection = separator.normalized();
        // checking moving in the same direction
        Vector relativeVelocity = first->getVelocity() - second->getVelocity();
        if (relativeVelocity.dot(relativeDirection) > 0) return false;
        // calculating the penetration
        real penetrationDistance = Constants::REAL_MAX;
        // dividing into all types of bodies
        if (first->getBodyType() == RigidBodyType::CIRCLE) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
                // distance of the overlapping area
                penetrationDistance = ((Circle*)first)->getRadius() + ((Circle*)second)->getRadius() - separator.norma();
            } else if (second->getBodyType() == RigidBodyType::POLYGON) {

            }
        } else if (first->getBodyType() == RigidBodyType::POLYGON) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
            } else if (second->getBodyType() == RigidBodyType::POLYGON) {

            }
        } // more body types

        // calculated relevant penetration distance
        if (penetrationDistance < 0) return false;
        // setting the separation distance for each body
        real firstDistance = second->getMass()/(first->getMass() + second->getMass());
        real secondDistance = first->getMass()/(first->getMass() + second->getMass());
        (*first) += relativeDirection * dt * -penetrationDistance * firstDistance;
        (*second) += relativeDirection * dt * penetrationDistance * secondDistance;

        return true;
    }

    bool physics::isCirclePolygonColliding(physics::Circle *first, physics::Polygon *second) {
        return false;
    }
}

#include "../include/Collision.hpp"

namespace physics {

    bool physics::resolveCollisions(physics::RigidBody *first, physics::RigidBody *second, real dt) {
        // referring to first and second as of type Circle
        Vector separator = second->getPosition() - first->getPosition();
        Vector relativeDirection = separator.normalized();
        Vector relativeVelocity = first->getVelocity() - second->getVelocity();
        // if moving in the same direction
        if (relativeVelocity.dot(relativeDirection) > 0) return false;
        real penetrationDistance = 0;
        // divide into different types of bodies
        if (first->getBodyType() == RigidBodyType::CIRCLE) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
                penetrationDistance = ((Circle*)first)->getRadius() + ((Circle*)second)->getRadius() - separator.norma();
            } else if (second->getBodyType() == RigidBodyType::BOX) {
                Vector firstSeparatorDistance = Vector{abs(first->getPosition().x - second->getPosition().x),
                                                       abs(first->getPosition().y - second->getPosition().y)};
                if (firstSeparatorDistance.x > ((Box*)second)->getWidth()/2 + ((Circle*)first)->getRadius()) return false;
                if (firstSeparatorDistance.y > ((Box*)second)->getHeight()/2 + ((Circle*)first)->getRadius()) return false;
                penetrationDistance = firstSeparatorDistance.distance(second->getPosition());
                if (penetrationDistance <= pow(((Circle*)first)->getRadius(), 2)) return false;
            }
        } else if (first->getBodyType() == RigidBodyType::BOX) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
                Vector secondSeparatorDistance = Vector{abs(second->getPosition().x - first->getPosition().x),
                                                       abs(second->getPosition().y - first->getPosition().y)};
                if (secondSeparatorDistance.x > ((Box*)first)->getWidth()/2 + ((Circle*)second)->getRadius()) return false;
                if (secondSeparatorDistance.y > ((Box*)first)->getHeight()/2 + ((Circle*)second)->getRadius()) return false;
                penetrationDistance = secondSeparatorDistance.distance(first->getPosition());
                if (penetrationDistance <= pow(((Circle*)second)->getRadius(), 2)) return false;
            } else if (second->getBodyType() == RigidBodyType::BOX) {
                // TODO: todo
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
}

#include "../include/Collision.hpp"

namespace physics {

    bool physics::resolveCollision(RigidBody *first, RigidBody *second, real dt) {
        if (!resolveVelocity(first, second, dt)) return false;
        return resolveInterpenetration(first, second, dt);
    }

    real physics::calculateSeparatingVelocity(RigidBody *first, RigidBody *second) {
        Vector relativeVelocity = first->getVelocity();
        relativeVelocity -= second->getVelocity();
        // calculate the direction
        Vector normal = (first->getPosition() - second->getPosition()).normalized();
        // return whether they're at the same direction
        return relativeVelocity.dot(normal);
    }

    bool physics::resolveVelocity(RigidBody *first, RigidBody *second, real dt) {
        real separatingVelocity = physics::calculateSeparatingVelocity(first, second);
        // if traveling at the same direction, no need to resolve velocity
        if (separatingVelocity > 0) return false;

        real restitution = first->getRestitution() < second->getRestitution() ?
                first->getRestitution() : second->getRestitution();
        real sepVelocity = -separatingVelocity * restitution;
        real deltaVelocity = sepVelocity - separatingVelocity;

        real totalInverseMass = first->getInverseMass();
        totalInverseMass += second->getInverseMass();
        // if both have infinite mass, stop
        if (totalInverseMass <= 0) return false;
        // impulse scalar,
        real impulse = deltaVelocity / totalInverseMass;
        // calculate direction normal of collision
        Vector normal = (first->getPosition() - second->getPosition()).normalized();
        // impulse vector
        Vector impulsePerInverseMass = normal * impulse;

        // apply the impulses
        first->incrementVelocity(impulsePerInverseMass * first->getInverseMass() * dt);
        second->incrementVelocity(impulsePerInverseMass * -second->getInverseMass() * dt);

        return true;
    }

    bool physics::resolveInterpenetration(RigidBody *first, RigidBody *second, real dt) {
        // calculate penetration based on rigid body types
        Vector firstToSecond = first->getPosition() - second->getPosition();
        real penetration = firstToSecond.magnitude() - ((real) 0.16); // 0.16 is the error epsilon
        // no penetration
        if (penetration <= 0) return false;
        real totalInverseMass = first->getInverseMass() + second->getInverseMass();
        // infinite mass
        if (totalInverseMass <= 0) return false;
        // calculate direction normal of collision
        Vector normal = (first->getPosition() - second->getPosition()).normalized();
        // direction vector
        Vector movePerInverseMass = normal * (-penetration / totalInverseMass);
        // apply penetration (+= operator acts on the position vector)
        (*first) += movePerInverseMass * first->getInverseMass() * dt;
        (*second) += movePerInverseMass * second->getInverseMass() * dt;
        return true;
    }
}

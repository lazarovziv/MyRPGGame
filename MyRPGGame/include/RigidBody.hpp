#ifndef MYRPGGAME_RIGIDBODY_HPP
#define MYRPGGAME_RIGIDBODY_HPP

#include "Vector.hpp"

namespace physics {
    class RigidBody {
    private:
        Vector velocity = Vector::ZERO;
        Vector acceleration = Vector::ZERO;
        real restitution;
        real inverseMass;

    public:
        RigidBody();
        ~RigidBody() = default;

        Vector getVelocity();
        Vector getAcceleration();
        real getRestitution();
        real getInverseMass();
        void setRestitution(real e);
        void setMass(real mass); // set inverseMass accordingly
        void incrementVelocity(Vector &v);
        void incrementAcceleration(Vector &v);
    };
}

#endif //MYRPGGAME_RIGIDBODY_HPP

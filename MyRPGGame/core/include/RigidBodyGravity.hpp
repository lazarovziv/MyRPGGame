
#ifndef MYRPGGAME_RIGIDBODYGRAVITY_HPP
#define MYRPGGAME_RIGIDBODYGRAVITY_HPP

#include "RigidBodyForceGenerator.hpp"

namespace physics {
    class RigidBodyGravity : public RigidBodyForceGenerator {
    private:
        Vector gravity = Vector::ZERO;

    public:
        RigidBodyGravity(const Vector &gravity);

        virtual void update(RigidBody *body, real dt) override;
    };
}

#endif //MYRPGGAME_RIGIDBODYGRAVITY_HPP

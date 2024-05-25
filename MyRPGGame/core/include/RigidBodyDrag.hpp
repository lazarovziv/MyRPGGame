
#ifndef MYRPGGAME_RIGIDBODYDRAG_HPP
#define MYRPGGAME_RIGIDBODYDRAG_HPP

#include "RigidBodyForceGenerator.hpp"

namespace physics {
    class RigidBodyDrag : public RigidBodyForceGenerator {
    private:
        // velocity drag coefficient
        real k1;
        // velocity squared drag coefficient
        real k2;

    public:
        RigidBodyDrag(real k1, real k2) : k1(k1), k2(k2) {};

        virtual void update(RigidBody *body, real dt) override;
    };
}

#endif //MYRPGGAME_RIGIDBODYDRAG_HPP

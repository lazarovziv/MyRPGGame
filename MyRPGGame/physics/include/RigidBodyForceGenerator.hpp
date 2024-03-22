
#ifndef MYRPGGAME_RIGIDBODYFORCEGENERATOR_HPP
#define MYRPGGAME_RIGIDBODYFORCEGENERATOR_HPP

#include "RigidBody.hpp"

namespace physics {
    // acts as an interface, with all methods being virtual
    class RigidBodyForceGenerator {
    public:
        virtual void update(RigidBody *body, real dt) = 0;
    };
}

#endif //MYRPGGAME_RIGIDBODYFORCEGENERATOR_HPP

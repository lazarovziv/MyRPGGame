#include "RigidBodyGravity.hpp"

namespace physics {

    RigidBodyGravity::RigidBodyGravity(const physics::Vector &gravity) {
        this->gravity = Vector{gravity.x, gravity.y, gravity.z};
    }

    void RigidBodyGravity::update(physics::RigidBody *body, real dt) {
        // no force can be applied to body with infinite mass as
//        if (!body->hasFiniteMass()) return;
        body->add_force(gravity * body->get_mass());
    }
}

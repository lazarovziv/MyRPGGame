#include "../include/RigidBodyForceRegistry.hpp"

namespace physics {

    void RigidBodyForceRegistry::addItem(physics::RigidBody *body, physics::RigidBodyForceGenerator *generator) {
        RigidBodyForceItem item;
        item.body = body;
        item.generator = generator;
        registrations.push_back(item);
    }

    void RigidBodyForceRegistry::removeItem(physics::RigidBody *body, physics::RigidBodyForceGenerator *generator) {

    }

    void RigidBodyForceRegistry::clearRegistrations() {
        registrations.clear();
    }

    void RigidBodyForceRegistry::update(real dt) {
        for (registry::iterator it = registrations.begin(); it != registrations.end(); it++) {
            it->generator->update(it->body, dt);
        }
    }
}

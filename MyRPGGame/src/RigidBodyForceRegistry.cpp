#include "../include/RigidBodyForceRegistry.hpp"

namespace physics {

    void RigidBodyForceRegistry::addItem(physics::RigidBody *body, physics::RigidBodyForceGenerator *generator) {
        RigidBodyForceItem item = {};
        item.body = body;
        item.generator = generator;
        registrations.push_back(item);
    }

    void RigidBodyForceRegistry::removeItem(physics::RigidBody *body, physics::RigidBodyForceGenerator *generator) {
        RigidBodyForceItem item = {};
        item.body = body;
        item.generator = generator;
//        registrations.erase(std::find(registrations.begin(), registrations.end(), item));
    }

    void RigidBodyForceRegistry::clearRegistrations() {
        registrations.clear();
    }

    std::vector<RigidBodyForceRegistry::RigidBodyForceItem> RigidBodyForceRegistry::getItems() const {
        return registrations;
    }

    void RigidBodyForceRegistry::update(real dt) {
        for (auto &item : registrations) {
            item.generator->update(item.body, dt);
        }
    }
}

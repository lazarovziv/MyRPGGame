#include "RigidBodyForceRegistry.hpp"

namespace physics {

    void RigidBodyForceRegistry::add_item(physics::RigidBody *body, physics::RigidBodyForceGenerator *generator) {
        RigidBodyForceItem item = {};
        item.body = body;
        item.generator = generator;
        registrations.push_back(item);
    }

    void RigidBodyForceRegistry::remove_item(physics::RigidBody *body, physics::RigidBodyForceGenerator *generator) {
        RigidBodyForceItem item = {};
        item.body = body;
        item.generator = generator;
        registrations.erase(std::remove_if(registrations.begin(),
                                           registrations.end(),
                                           [&](RigidBodyForceItem const &item) {
                    return item.body == body;
                }),registrations.end());
    }

    void RigidBodyForceRegistry::clear_registrations() {
        registrations.clear();
    }

    std::vector<RigidBodyForceRegistry::RigidBodyForceItem> RigidBodyForceRegistry::get_items() const {
        return registrations;
    }

    void RigidBodyForceRegistry::update(real dt) {
        for (auto &item : registrations) {
            item.generator->update(item.body, dt);
        }
    }
}

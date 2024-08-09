
#ifndef MYRPGGAME_RIGIDBODYFORCEREGISTRY_HPP
#define MYRPGGAME_RIGIDBODYFORCEREGISTRY_HPP

#include "RigidBodyForceGenerator.hpp"

namespace physics {
    // contains all force generators
    class RigidBodyForceRegistry {
    protected:
        struct RigidBodyForceItem {
            RigidBody *body;
            RigidBodyForceGenerator *generator;
        };

        typedef std::vector<RigidBodyForceItem> registry;
        registry registrations;

    public:
        void add_item(RigidBody *body, RigidBodyForceGenerator *generator);
        void remove_item(RigidBody *body, RigidBodyForceGenerator *generator);
        void clear_registrations();
        registry get_items() const;

        void update(real dt);
    };
}

#endif //MYRPGGAME_RIGIDBODYFORCEREGISTRY_HPP

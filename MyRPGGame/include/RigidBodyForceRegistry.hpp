
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
        void addItem(RigidBody *body, RigidBodyForceGenerator *generator);
        void removeItem(RigidBody *body, RigidBodyForceGenerator *generator);
        void clearRegistrations();

        void update(real dt);
    };
}

#endif //MYRPGGAME_RIGIDBODYFORCEREGISTRY_HPP

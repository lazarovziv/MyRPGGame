#include "systems.hpp"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace rg {
    void Renderer::render(const std::unique_ptr<sf::RenderWindow>& window,
                          std::unique_ptr<sf::View>& cameraView,
                          const glm::vec3& cameraPosition,
                          const entt::registry& registry) {
        cameraView->setCenter(cameraPosition.x, cameraPosition.y);
        window->setView(*cameraView);

        auto view = registry.view<Renderable, Transform>();

        // clearing the previous frame
        window->clear();

        // draw background

        // traversing all entities
        for (auto& entity: view) {
            auto renderable = view.get<Renderable>(entity);
            sf::Sprite& sprite = renderable.sprite;
            auto transform = view.get<Transform>(entity);
            glm::vec3& position = transform.position;

            renderable.setPosition(position);

            // rendering at the relevant location
            window->draw(sprite);
        }

        window->display();
    }

    // important: if registry will be const, the view will be const and the
    // modifications of the components' data won't work
    void Movement::update(entt::registry& registry, const real dt) {
        auto view = registry.view<RigidBody, Moveable>();

        view.each([&](auto& rigidBody, auto& moveable) {
            glm::vec3 force = moveable.direction * (Constants::FORCE_SCALAR * dt);
            rigidBody.addForce(force);
        });
    }

    // should be called after the entities' movements
    bool Physics::resolveCollisions(RigidBody& rbFirst, RigidBody& rbSecond,
                                    const Transform& tFirst, const Transform& tSecond,
                                    const real dt) {
        // no collision effect will occurr with non-moveable objects
        if (rbFirst.hasInfiniteMass() && rbSecond.hasInfiniteMass())
            return false;
        // direction vector to project first body
        const glm::vec3 axisToProject = tSecond.position - tFirst.position;
        // normalizing manually instead of glm::normalize since we'll use the norma value
        real axisNorma = std::sqrt(glm::dot(axisToProject, axisToProject));
        const glm::vec3 axisNormalized = axisToProject / axisNorma;

        // TODO: calculate penetration distance and create forces accordingly
        const real penetrationDistance = tFirst.size + tSecond.size - axisNorma;
        if (penetrationDistance < 0)
            return false;

        // using restitution and getting collision response values
        const glm::vec3 relativeVelocity = -axisToProject;
        const real e = std::min(rbFirst.restitution, rbSecond.restitution);
        real j = -(1.0f + e) * glm::dot(relativeVelocity, axisNormalized);
        j /= rbFirst.inverseMass +
                rbSecond.inverseMass; // if axis wasn't normalized, magnitude was
        // needed in the
        // denominator, multiplied by the inverse masses sum
        const glm::vec3 impulse = axisNormalized * j;
        if (!rbFirst.hasInfiniteMass())
            rbFirst.velocity +=
                    impulse * (-rbFirst.inverseMass * Constants::FORCE_SCALAR * dt);
        if (!rbSecond.hasInfiniteMass())
            rbSecond.velocity +=
                    impulse * (rbSecond.inverseMass * Constants::FORCE_SCALAR *
                               dt); // incremented position before
        return true;
    }

    void Physics::update(entt::registry& registry, const real dt) {
        auto view = registry.view<RigidBody, Transform>();

        // calculate collision
        for (const entt::entity first: view) {
            auto& rbFirst = view.get<RigidBody>(first);
            auto& tFirst = view.get<Transform>(first);

            // dont calculate collisions for infinite mass objects,
            // but will calculate collisions for non infinite mass objects with
            // infinite mass one
            if (rbFirst.hasInfiniteMass())
                continue;

            // getting entity's position on the grid
            // TODO: get coordinates relative to the window and not on the absolute
            // position - relevant when maps are larger than the window size
            const int firstGridX = static_cast<int>(tFirst.position.x);
            const int firstGridY = static_cast<int>(tFirst.position.y);

            // traversing other objects that are different and are close by in the
            // window grid
            for (const entt::entity second: view) {
                auto& rbSecond = view.get<RigidBody>(second);
                auto& tSecond = view.get<Transform>(second);
                // don't calculate collisions between an entity and itself
                if (first == second)
                    continue;

                // getting entity's position on the grid and checking if we should
                // calculate the collision with another
                const int secondGridX = static_cast<int>(tSecond.position.x);
                const int secondGridY = static_cast<int>(tSecond.position.y);

                const int distance = manhattanDistance(firstGridX, secondGridX,
                                                       firstGridY, secondGridY);
                if (distance > gridCheckMaxDistance)
                    continue;

                resolveCollisions(rbFirst, rbSecond, tFirst, tSecond, dt);
            }
        }

        view.each([&](auto& rigidBody, auto& transform) {
            // not updating infinite mass bodies
            if (rigidBody.infiniteMass)
                return;
            // acceleration = force * mass
            rigidBody.acceleration +=
                    rigidBody.forceAccumulator * rigidBody.inverseMass;
            // if (acceleration->magnitude() >
            // Constants::ACCELERATION_MAGNITUDE_MAX) (*acceleration) *= 0.85;
            rigidBody.velocity += rigidBody.acceleration * dt;
            // drag
            rigidBody.velocity *= pow(rigidBody.damping, dt);
            // applying forces to the position
            transform.position +=
                    rigidBody.velocity * dt +
                    rigidBody.acceleration * dt * dt * static_cast<real>(0.5);
            // defined friction
            rigidBody.velocity *= static_cast<real>(1) /
                    Constants::GRASS_FRICTION_DEGRADATION_CONSTANT;
            // clamping velocity
            const real velocityMagnitude =
                    glm::dot(rigidBody.velocity, rigidBody.velocity);
            if (velocityMagnitude < Constants::VELOCITY_MAGNITUDE_MIN)
                rigidBody.resetVelocity();
            if (velocityMagnitude > Constants::VELOCITY_MAGNITUDE_MAX)
                rigidBody.velocity *= static_cast<real>(0.95);

            rigidBody.resetForceAccumulator();
            rigidBody.resetAcceleration();
        });
    }
} // namespace rg

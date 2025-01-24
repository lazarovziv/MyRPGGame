#pragma once

#include <SFML/Graphics.hpp>
#include <glm/vec3.hpp>

#include "components.hpp"

#include "entt/entt.hpp"

#include <memory>
#include <string>
#include <vector>

namespace rg {
    class Renderer {
    public:
        // a single renderer is required
        Renderer(const Renderer&) = delete;

        void operator=(const Renderer&) = delete;

        void render(const std::unique_ptr<sf::RenderWindow>& window,
                    std::unique_ptr<sf::View>& cameraView,
                    const glm::vec3& cameraPosition,
                    const entt::registry& registry);
    };

    class Movement {
    public:
        Movement(const Movement&) = delete;

        void operator=(const Movement&) = delete;

        void update(entt::registry& registry, real dt);
    };

    class Physics {
    private:
        // don't check with entities with a manhattan distance that is larger than
        // gridCheckMaxDistance
        static constexpr int gridCheckMaxDistance = static_cast<int>(Constants::TILE_SIZE * 2);

    public:
        Physics() = default;

        Physics(const Physics&) = delete;

        void operator=(const Physics&) = delete;

        bool resolveCollisions(RigidBody& rbFirst, RigidBody& rbSecond,
                               const Transform& tFirst, const Transform& tSecond, real dt);

        void update(entt::registry& registry, real dt);

    private:
        static int manhattanDistance(const int firstX, const int secondX,
                                     const int firstY, const int secondY) {
            int x = firstX - secondX;
            // taking the absolute value
            x = x >= 0 ? x : -x;
            int y = firstY - secondY;
            y = y >= 0 ? y : -y;
            return x + y;
        };
    };
} // namespace rg

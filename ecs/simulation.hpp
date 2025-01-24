#include "entt/entt.hpp"
#include <glm/vec3.hpp>
#include "Constants.h"
#include "components.hpp"


inline void createHorizontalEntities(entt::registry& registry, const unsigned int length,
                                     const unsigned int y) noexcept {
    unsigned int numEntities = length / rg::Constants::TILE_SIZE * 2;

    for (int i = 0; i < numEntities; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<rg::Transform>(entity, rg::Constants::TILE_SIZE * (i + 1) / 2,
                                                          static_cast<real>(y), static_cast<real>(0));
        const std::string& texturePath = rg::Constants::getResourcePath("LPC/player/uriel_spritesheet.png");
        auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath);
        registry.emplace<rg::Moveable>(entity);
        registry.emplace<rg::RigidBody>(entity, rg::Constants::INFINITE_MASS_MASS_VALUE);
    }
}

inline void createVerticalEntities(entt::registry& registry, const unsigned int length, const unsigned int x) noexcept {
    unsigned int numEntities = length / rg::Constants::TILE_SIZE * 2;

    for (int i = 0; i < numEntities; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<rg::Transform>(entity, static_cast<real>(x),
                                                          rg::Constants::TILE_SIZE * (i + 1) / 2, static_cast<real>(0));
        const std::string& texturePath = rg::Constants::getResourcePath("LPC/player/uriel_spritesheet.png");
        auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath);
        registry.emplace<rg::Moveable>(entity);
        registry.emplace<rg::RigidBody>(entity, rg::Constants::INFINITE_MASS_MASS_VALUE);
    }
}


inline void createGrassEntities(entt::registry& registry, const unsigned int amount) noexcept {
    for (int i = 0; i < amount; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<rg::Transform>(entity,
                                                          rg::Constants::TILE_SIZE / 2 * (i + 1),
                                                          static_cast<real>(rg::Constants::WINDOW_HEIGHT),
                                                          static_cast<real>(0));
        const std::string& texturePath = rg::Constants::getResourcePath("LPC/Terrain/flowers.png"); // row 3 col 9
        auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath, rg::Constants::TILE_SIZE / 2, 3, 9);
        registry.emplace<rg::Grass>(entity);
    }
}


inline void prepareSimulation(entt::registry& registry) noexcept {
    createHorizontalEntities(registry, rg::Constants::WINDOW_WIDTH, 400);
    createHorizontalEntities(registry, rg::Constants::WINDOW_WIDTH, rg::Constants::WINDOW_HEIGHT + 200);

    createVerticalEntities(registry, rg::Constants::WINDOW_HEIGHT * 2, 0);
    createVerticalEntities(registry, rg::Constants::WINDOW_HEIGHT * 2, rg::Constants::WINDOW_WIDTH);

    createGrassEntities(registry, 25);
}

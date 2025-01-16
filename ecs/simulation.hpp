#include "entt/entt.hpp"
#include <glm/vec3.hpp>
#include "Constants.h"
#include "components.hpp"


void createHorizontalEntities(entt::registry& registry, const unsigned int length, const unsigned int y) {
    unsigned int numEntities = length / mrg::Constants::TILE_SIZE;

    for (int i = 0; i < numEntities; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<mrg::Transform>(entity, mrg::Constants::TILE_SIZE/2);
        const std::string &texturePath = "../../../graphics/player/uriel_spritesheet.png";
        auto &renderable = registry.emplace<mrg::Renderable>(entity, texturePath);
        transform.position = glm::vec3{mrg::Constants::TILE_SIZE * (i+1), y, 0};
        auto &moveable = registry.emplace<mrg::Moveable>(entity);
        moveable.direction = glm::vec3{0, 0, 0};
        auto &rigidBody = registry.emplace<mrg::RigidBody>(entity, /* 4000 */ mrg::Constants::INFINITE_MASS_MASS_VALUE);
    }
}

void createVerticalEntities(entt::registry& registry, const unsigned int length, const unsigned int x) {
    unsigned int numEntities = length / mrg::Constants::TILE_SIZE;

    for (int i = 0; i < numEntities; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<mrg::Transform>(entity, mrg::Constants::TILE_SIZE/2);
        const std::string &texturePath = "../../../graphics/player/uriel_spritesheet.png";
        auto &renderable = registry.emplace<mrg::Renderable>(entity, texturePath);
        transform.position = glm::vec3{x, mrg::Constants::TILE_SIZE * (i+1), 0};
        auto &moveable = registry.emplace<mrg::Moveable>(entity);
        moveable.direction = glm::vec3{0, 0, 0};
        auto &rigidBody = registry.emplace<mrg::RigidBody>(entity, /* 4000 */ mrg::Constants::INFINITE_MASS_MASS_VALUE);
    }
}

void prepareSimulation(entt::registry& registry) {
    createHorizontalEntities(registry, mrg::Constants::WINDOW_WIDTH, 400);
    createHorizontalEntities(registry, mrg::Constants::WINDOW_WIDTH, mrg::Constants::WINDOW_HEIGHT + 200);
    
    createVerticalEntities(registry, mrg::Constants::WINDOW_HEIGHT * 2, 0);
    createVerticalEntities(registry, mrg::Constants::WINDOW_HEIGHT * 2, mrg::Constants::WINDOW_WIDTH);
}
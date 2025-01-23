#include "entt/entt.hpp"
#include <glm/vec3.hpp>
#include "Constants.h"
#include "components.hpp"


void createHorizontalEntities(entt::registry& registry, const unsigned int length, const unsigned int y) {
    unsigned int numEntities = length / rg::Constants::TILE_SIZE * 2;

    for (int i = 0; i < numEntities; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<rg::Transform>(entity);
        const std::string& texturePath = rg::Constants::getGraphicsPath("graphics/player/uriel_spritesheet.png");
        auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath);
        transform.position = glm::vec3{rg::Constants::TILE_SIZE * (i+1) / 2, y, 0};
        registry.emplace<rg::Moveable>(entity);
        registry.emplace<rg::RigidBody>(entity, rg::Constants::INFINITE_MASS_MASS_VALUE);
    }
}

void createVerticalEntities(entt::registry& registry, const unsigned int length, const unsigned int x) {
    unsigned int numEntities = length / rg::Constants::TILE_SIZE * 2;

    for (int i = 0; i < numEntities; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<rg::Transform>(entity);
        const std::string &texturePath = rg::Constants::getGraphicsPath("graphics/player/uriel_spritesheet.png");
        auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath);
        transform.position = glm::vec3{x, rg::Constants::TILE_SIZE * (i+1) / 2, 0};
        registry.emplace<rg::Moveable>(entity);
        registry.emplace<rg::RigidBody>(entity, rg::Constants::INFINITE_MASS_MASS_VALUE);
    }
}

void createGrassEntities(entt::registry& registry, const unsigned int amount) {
    for (int i = 0; i < amount; ++i) {
        entt::entity entity = registry.create();
        auto& transform = registry.emplace<rg::Transform>(entity, rg::Constants::TILE_SIZE/2);
        const std::string &texturePath = rg::Constants::getResourcePath("res/LPC/Terrain/flowers.png"); // row 3 col 9
        auto& renderable = registry.emplace<rg::Renderable>(entity, texturePath, rg::Constants::TILE_SIZE/2, 3, 9);
        transform.position = glm::vec3{rg::Constants::TILE_SIZE/2 * (i+1), rg::Constants::WINDOW_HEIGHT, 0};
        registry.emplace<rg::Grass>(entity);
    }
}

void prepareSimulation(entt::registry& registry) {
    createHorizontalEntities(registry, rg::Constants::WINDOW_WIDTH, 400);
    createHorizontalEntities(registry, rg::Constants::WINDOW_WIDTH, rg::Constants::WINDOW_HEIGHT + 200);
    
    createVerticalEntities(registry, rg::Constants::WINDOW_HEIGHT * 2, 0);
    createVerticalEntities(registry, rg::Constants::WINDOW_HEIGHT * 2, rg::Constants::WINDOW_WIDTH);

    // createGrassEntities(registry, 25);
}
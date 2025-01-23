#pragma once

#include "Constants.h"

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// std
#include <cmath>
#include <iostream>
#include <map>
#include <string>

namespace rg {

// will hold all graphics data that's relevant for rendering, as the renderer
// will take all other necessary components
struct Renderable {
    sf::Sprite sprite;

    Renderable() = delete;
    explicit Renderable(const std::string& textureFilePath,
               unsigned int tileSize = Constants::TILE_SIZE,
               unsigned int row = 33, unsigned int col = 0);

    void setPosition(const glm::vec3& position);
    void setTexture(const std::string& textureFilePath,
                    unsigned int tileSize, unsigned int row,
                    unsigned int col);
};

struct Transform {
    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 scale{0, 0, 0};
    // corresponds to a circle's radius
    real size = static_cast<real>(Constants::TILE_SIZE) / 4;

    Transform() = default;
    explicit Transform(real s);
};

struct RigidBody {
    glm::vec3 velocity{0, 0, 0};
    glm::vec3 acceleration{0, 0, 0};
    // used for D'Alembert rule, accumulating all forces that act on the rigid
    // body
    glm::vec3 forceAccumulator{0, 0, 0};
    real restitution;
    real mass = 1;
    real damping;
    real inverseMass;
    bool infiniteMass = false;

    bool hasInfiniteMass() const { return infiniteMass; };

    static void resetVector(glm::vec3& vec) {
        vec.x = 0;
        vec.y = 0;
        vec.z = 0;
    }

    void resetVelocity() {
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
    }

    void resetAcceleration() {
        acceleration.x = 0;
        acceleration.y = 0;
        acceleration.z = 0;
    }

    void resetForceAccumulator() {
        forceAccumulator.x = 0;
        forceAccumulator.y = 0;
        forceAccumulator.z = 0;
    }

    inline void addForce(const glm::vec3& force) { forceAccumulator += force; }

    explicit RigidBody(real mass = 1);
};

struct Gravity {
    glm::vec3 force{0, 9.81f, 0};
};

struct Moveable {
    glm::vec3 direction{0, 0, 0};
    Constants::SpriteMoveDirection spriteDirection;
};

// placeholder for indicating whether an entity is the player's entity
struct Player {};

struct Grass {};

} // namespace rg
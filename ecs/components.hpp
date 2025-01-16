#pragma once

#include "Constants.h"

#include <SFML/Graphics.hpp>
#include <glm/vec3.hpp>

// std
#include <cmath>
#include <iostream>
#include <map>
#include <string>

namespace mrg {

// will hold all graphics data that's relevant for rendering, as the renderer will take all other necessary
// components
struct Renderable {
    sf::Sprite sprite;

    explicit Renderable(const std::string& textureFilePath);
    void setPosition(const glm::vec3& position);
};

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    // corresponds to a circle's radius
    real size = static_cast<real>(Constants::TILE_SIZE) / 4;

    Transform() {}
    Transform(real s);
};

struct RigidBody {
    glm::vec3 velocity{0, 0, 0};
    glm::vec3 acceleration{0, 0, 0};
    // used for D'Alembert rule, accumulating all forces that act on the rigid body
    glm::vec3 forceAccumulator{0, 0, 0};
    real restitution;
    real mass = 1;
    real damping;
    real inverseMass;
    bool infiniteMass = false;

    inline bool hasInfiniteMass() const { return infiniteMass; };

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
struct Player {

};

} // namespace mrg
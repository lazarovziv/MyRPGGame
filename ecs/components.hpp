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
    sf::Texture texture;
    sf::Sprite sprite;

    Renderable(const std::string &textureFilePath);
    void setPosition(glm::vec3 &position);
};

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct RigidBody {
    glm::vec3 velocity;
    glm::vec3 acceleration;
    // used for D'Alembert rule, accumulating all forces that act on the rigid body
    glm::vec3 forceAccumulator;
    real restitution;
    real mass = 1;
    real damping;
    real inverseMass;
    bool infiniteMass = false;
};

struct Gravity {
    glm::vec3 force = glm::vec3{0, (real)9.81, 0};
};

struct Moveable {
    glm::vec3 direction;
};

// placeholder for indicating whether an entity is the player's entity
struct Player {

};

} // namespace mrg
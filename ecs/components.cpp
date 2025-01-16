#include "components.hpp"

#include "TextureManager.h"

namespace mrg {

Transform::Transform(const real s) : size{s} {}

Renderable::Renderable(const std::string &texturePath) {
    auto textureManager = TextureManager::getInstance();
    // shared ptr
    auto texture = textureManager->retrieveTexture(texturePath);
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 33 * Constants::TILE_SIZE, Constants::TILE_SIZE, Constants::TILE_SIZE));
    sprite.setOrigin(Constants::TILE_SIZE / 2, Constants::TILE_SIZE / 2);
}

RigidBody::RigidBody(const real mass) {
    if (mass == Constants::INFINITE_MASS_MASS_VALUE) {
        this->mass = mass;
        inverseMass = 0;
        infiniteMass = true;
    } else {
        this->mass = mass;
        inverseMass = static_cast<real>(1) / mass;
        // just updated mass to be finite
        infiniteMass = false;
    }
    restitution = 0.69;
    damping = 0.995;
}

void Renderable::setPosition(const glm::vec3& position) { sprite.setPosition(position.x, position.y); }

} // namespace mrg
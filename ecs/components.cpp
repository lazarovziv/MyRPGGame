#include "components.hpp"

#include "SpriteManager.hpp"
#include "TextureManager.hpp"

namespace rg {
    Transform::Transform(const real x, const real y, const real z) {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    Renderable::Renderable(const std::string& texturePath, const unsigned int tileSize, const unsigned int row,
                           const unsigned int col) {
        unsigned int r = SpriteManager::getInstance()->getRowOfEntityActionState(Constants::EntityActionState::IDLE);
        unsigned int c = SpriteManager::getInstance()->getColOfDirection(Constants::SpriteMoveDirection::BOTTOM);
        setTexture(texturePath, tileSize, r, c);
    }

    void Renderable::setTexture(const std::string& texturePath, const unsigned int tileSize, const unsigned int row,
                                const unsigned int col) {
        auto textureManager = TextureManager::getInstance();
        auto texture = textureManager->retrieveTexture(texturePath);
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(col * tileSize, row * tileSize, tileSize, tileSize));
        sprite.setOrigin(tileSize / 2, tileSize / 2);
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
} // namespace rg

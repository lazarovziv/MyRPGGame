#include "Projectile.hpp"

Projectile::Projectile(real v, real x, real y, ProjectileType t) {
    velocity = v;
    position.x = x;
    position.y = y;
    type = t;

    sf::Texture texture;
    sprite = new sf::Sprite();
    texture.loadFromFile("/home/ziv/projects/cpp/MyRPGGame/graphics/player_8.png");
    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
}

sf::IntRect Projectile::getRectangle() {
    return (sf::IntRect) sprite->getGlobalBounds();
}


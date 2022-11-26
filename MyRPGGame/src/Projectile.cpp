#include "../include/Projectile.hpp"

Projectile::Projectile(int v, int x, int y, ProjectileType t) {
    velocity = v;
    position.x = x;
    position.y = y;
    type = t;

    Texture texture;
    sprite = new Sprite();
    texture.loadFromFile("/home/ziv/projects/cpp/MyRPGGame/graphics/player_8.png");
    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
}

IntRect Projectile::getRectangle() {
    return (IntRect) sprite->getGlobalBounds();
}


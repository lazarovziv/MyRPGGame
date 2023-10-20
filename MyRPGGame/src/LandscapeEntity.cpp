#include "../include/LandscapeEntity.hpp"

LandscapeEntity::LandscapeEntity(LandscapeType type, physics::Vector initialPosition, std::vector<physics::Vector> &vertices, real mass)
: GameEntity(initialPosition, physics::RigidBodyType::POLYGON, vertices, mass) {
    this->type = type;
//    RectangleShape rectangle0(Vector2f(200, 200));
    switch (this->type) {
        case LandscapeType::TREE:
            if (texture.loadFromFile("../graphics/trees/maple_64.png")) std::cout << "Tree loaded." << std::endl;
            id = 1; // totally arbitrary`
            break;
        case LandscapeType::GRASS:
            texture.loadFromFile("../graphics/grass.png");
            id = 2; // totally arbitrary
            break;
        case LandscapeType::FLOWER:
            texture.loadFromFile("../graphics/flower.png");
            id = 3; // totally arbitrary
            break;
        case LandscapeType::HOUSE:
            texture.loadFromFile("../graphics/house.png");
            id = 4; // totally arbitrary
            break;
    }

    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2); // or set texture.size / 2, texture.size / 2
    sprite->setPosition(position.x, position.y);
    // setting the landscape as inelastic as possible
    rigidBody->setRestitution(0);
//    entityCircle->setRadius(entityCircle->getRadius()*2);
}

LandscapeType LandscapeEntity::getType() {
    return type;
}

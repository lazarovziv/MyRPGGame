#include "LandscapeEntity.hpp"

LandscapeEntity::LandscapeEntity(LandscapeType type, physics::Vector initialPosition, std::vector<physics::Vector> &vertices, real mass)
: GameEntity(initialPosition, physics::RigidBodyType::POLYGON, vertices, mass) {
    this->type = type;
//    RectangleShape rectangle0(Vector2f(200, 200));
    switch (this->type) {
        case LandscapeType::TREE:
            if (texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "trees/maple_64.png")) std::cout << "Tree loaded." << std::endl;
            id = 1; // totally arbitrary`
            break;
        case LandscapeType::GRASS:
            texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "grass.png");
            id = 2; // totally arbitrary
            break;
        case LandscapeType::FLOWER:
            texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "flower.png");
            id = 3; // totally arbitrary
            break;
        case LandscapeType::HOUSE:
            texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "house.png");
            id = 4; // totally arbitrary
            break;
    }

    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2); // or set texture.size / 2, texture.size / 2
    sprite->setPosition(position.x, position.y);
    // setting the landscape as inelastic as possible
    rigid_body->set_restitution(0);

    is_landscape = true;
//    entityCircle->setRadius(entityCircle->getRadius()*2);
}

LandscapeType LandscapeEntity::get_type() {
    return type;
}

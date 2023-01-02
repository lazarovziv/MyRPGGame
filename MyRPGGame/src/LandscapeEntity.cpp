#include "../include/LandscapeEntity.hpp"

LandscapeEntity::LandscapeEntity(LandscapeType type, int x, int y) {
    position.x = x;
    position.y = y;
    this->type = type;
//    RectangleShape rectangle0(Vector2f(200, 200));
    sprite = new Sprite();

    switch (this->type) {
        case LandscapeType::TREE:
            if (texture.loadFromFile("../graphics/trees/maple_64.png")) std::cout << "Tree loaded." << endl;
            id = 1; // totally arbitrary
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
    entityCircle = new Circle(position.x, position.y, Constants::TILE_SIZE/4);
}

LandscapeEntity::~LandscapeEntity() {
    delete sprite;
    delete entityCircle;
    delete attackRangeCircle;
    delete weapon;
}

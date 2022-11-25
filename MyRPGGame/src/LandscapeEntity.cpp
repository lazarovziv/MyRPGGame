#include "../include/LandscapeEntity.hpp"

LandscapeEntity::LandscapeEntity(LandscapeType type, float x, float y) {
    position.x = x;
    position.y = y;
    this->type = type;
//    RectangleShape rectangle0(Vector2f(200, 200));
    Texture landscapeTexture;

    switch (this->type) {
        case LandscapeType::TREE:
            landscapeTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/tree.png");
            id = 1; // totally arbitrary
            break;
        case LandscapeType::GRASS:
            landscapeTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/grass.png");
            id = 2; // totally arbitrary
            break;
        case LandscapeType::FLOWER:
            landscapeTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/flower.png");
            id = 3; // totally arbitrary
            break;
        case LandscapeType::HOUSE:
            landscapeTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/house.png");
            id = 4; // totally arbitrary
            break;
    }
    sprite.setTexture(landscapeTexture);
    sprite.setOrigin(32, 32); // or set texture.size / 2, texture.size / 2
    sprite.setPosition(position);

}

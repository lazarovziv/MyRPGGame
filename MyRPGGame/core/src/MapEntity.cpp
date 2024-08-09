#include "MapEntity.hpp"

MapEntity::MapEntity(int x, int y) {
    walkableOn = true;
    landscape = false;
    mapType = MapEntityType::NONE;
    topExit = bottomExit = rightExit = leftExit = false;
    currentGameEntity = nullptr;

    position.x = x;
    position.y = y;

    // init sprite
    sprite = new Sprite();
    if (!texture.loadFromFile("../graphics/grass_64x64.png")) {
        std::cout << "Texture NOT loaded properly!" << endl;
        texture.setSmooth(true);
    } else std::cout << "Texture loaded properly." << endl;
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
}

MapEntity::MapEntity(GameEntity *entity, MapEntityType type) {
    // set movement attributes based on map type
    switch (type) {
        case PLAYER:
        case NPC:
        case ENEMY:
            walkableOn = false;
            landscape = false;
            break;
        case LANDSCAPE:
            walkableOn = false;
            landscape = true;
            break;
        case NONE:
            walkableOn = true;
            landscape = false;
            break;
    }
    // assuming tile isn't an exit tile
    topExit = bottomExit = rightExit = leftExit = false;

    // init entity and location
    currentGameEntity = entity;
    position.x = currentGameEntity->getPosition().x;
    position.y = currentGameEntity->getPosition().y;

    // init sprite
    sprite = new Sprite();
    if (!texture.loadFromFile("../graphics/grass_64x64.png")) {
        std::cout << "Texture NOT loaded properly!" << endl;
        texture.setSmooth(true);
    } else std::cout << "Texture loaded properly." << endl;
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
}

MapEntity::~MapEntity() {
    delete sprite;
    delete currentGameEntity;
}

bool MapEntity::isWalkableOn() {
    return walkableOn;
}

bool MapEntity::isLandscape() {
    return landscape;
}

bool MapEntity::isTopExit() {
    return topExit;
}

bool MapEntity::isBottomExit() {
    return bottomExit;
}

bool MapEntity::isRightExit() {
    return rightExit;
}

bool MapEntity::isLeftExit() {
    return leftExit;
}

Vector2i MapEntity::getPosition() {
    return position;
}

Sprite *MapEntity::getSprite() {
    return sprite;
}

GameEntity *MapEntity::getCurrentGameEntity() {
    return currentGameEntity;
}

MapEntityType MapEntity::getMapType() {
    return mapType;
}

void MapEntity::setWalkableOn(bool isWalkableOn) {
    walkableOn = isWalkableOn;
}

void MapEntity::setLandscape(bool isLandscape) {
    landscape = isLandscape;
}

void MapEntity::setIsTopExit(bool isTop) {
    topExit = isTop;
}

void MapEntity::setIsBottomExit(bool isBottom) {
    bottomExit = isBottom;
}

void MapEntity::setIsRightExit(bool isRight) {
    rightExit = isRight;
}

void MapEntity::setIsLeftExit(bool isLeft) {
    leftExit = isLeft;
}

// TODO: check for valid values
void MapEntity::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void MapEntity::setCurrentGameEntity(GameEntity *entity) {
    currentGameEntity = entity;
}

void MapEntity::setMapType(MapEntityType type) {
    mapType = type;
}

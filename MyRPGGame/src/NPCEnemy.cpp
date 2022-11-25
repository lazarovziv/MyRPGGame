#include "../include/NPCEnemy.hpp"

NPCEnemy::NPCEnemy() {
    
}

NPCEnemy::NPCEnemy(int type, int x, int y) {
    this->type = type;
    level = 1;
    maxHealthPoints = 20;
    currentHealthPoints = maxHealthPoints;
    maxManaPoints = 10;
    currentManaPoints = maxManaPoints;
    attackPoints = 2;
    defencePoints = 1;
    currentDefencePoints = defencePoints;
    expPointsWorth = 10;
    inBattle = false;
    dead = false;
    moveDirection = MoveDirection::UP;
    spawn(x, y);

    texture = new Texture();
    texture->loadFromFile("../graphics/dorio_64.png");
    sprite->setTexture(*texture);
    sprite->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    weapon = new Weapon(WeaponType::BARE_HANDED);
    entityCircle = new Circle(position.x, position.y, Constants::TILE_SIZE/4);
    attackRangeCircle = new Circle(position.x, position.y, (entityCircle->getRadius() * (float) 11/3) + weapon->getHitRadius());

    lastTimeMoved = std::clock();
     // TODO: choose random floats in defined location radius for each enemy in map
}

NPCEnemy::~NPCEnemy() {
    delete sprite;
    delete texture;
//    delete moveDirectionsSpritesMap;
    delete entityCircle;
    delete attackRangeCircle;
    delete spawnArea;
}

int NPCEnemy::getBattleTimeout() {
    return battleTimeout;
}

float NPCEnemy::getWanderAreaRadius() {
    return wanderAreaRadius;
}

float NPCEnemy::getBattleAreaRadius() {
    return battleAreaRadius;
}

int NPCEnemy::getExpPointsWorth() {
    return expPointsWorth;
}

int NPCEnemy::getType() {
    return type;
}

void NPCEnemy::spawn(int x, int y) {
    this->setPosition(x, y);
}

Circle* NPCEnemy::getSpawnArea() {
    return spawnArea;
}

void NPCEnemy::setSpawnArea(int centerX, int centerY, float radius) {
    if (spawnArea == nullptr) {
        spawnArea = new Circle(centerX, centerY, radius);
        return;
    }
    spawnArea->setCenter(centerX, centerY);
    spawnArea->setRadius(radius);
}

// direction is chosen randomly
bool NPCEnemy::canMove() {
    std::clock_t nowTime = std::clock();
    // TODO: choose direction randomly
    // checking if entity can move due to moveInterval value
    double diff = (nowTime - lastTimeMoved) / (double) CLOCKS_PER_SEC;
    if (diff >= moveInterval) {
        // updating lastTimeMoved to latest move made time
        lastTimeMoved = nowTime;
        return true;
    }
    return false;
}

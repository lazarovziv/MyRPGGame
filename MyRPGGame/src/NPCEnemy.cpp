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
//    spawn(x, y);

//    texture = new Texture();
    sprite = new Sprite();
    // TextureLoader.getInstance()->loadTexture("dorio_64.png");
    texture.loadFromFile("../graphics/dorio_64.png");
    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    weapon = new Weapon(WeaponType::BARE_HANDED);
    entityCircle = new Circle(position.x, position.y, Constants::TILE_SIZE/4);
    attackRangeCircle = new Circle(position.x, position.y, (entityCircle->getRadius() * (float) 11/3) + weapon->getHitRadius());

    lastTimeMoved = std::clock();
    lastTimeBattled = lastTimeMoved;
    lastTimeWandered = lastTimeMoved;
     // TODO: choose random floats in defined location radius for each enemy in map
}

NPCEnemy::NPCEnemy(int type, Point *center) : GameEntity(center) {
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
//    spawn(x, y);

//    texture = new Texture();
    // TextureLoader.getInstance()->loadTexture("dorio_64.png");
    texture.loadFromFile("../graphics/dorio_64.png");
    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    weapon = new Weapon(WeaponType::BARE_HANDED);
    spawnArea = new Circle(entityCircle->getCenter(), Constants::TILE_SIZE);
    attackRangeCircle->setRadius(attackRangeCircle->getRadius() + weapon->getHitRadius());
    wanderAreaRadius = entityCircle->getRadius() * 12;
    wanderAreaCircle = new Circle(entityCircle->getCenter(), wanderAreaRadius);
    battleAreaRadius = (float) (wanderAreaRadius * 1.5);
    battleAreaCircle = new Circle(entityCircle->getCenter(), battleAreaRadius);

    lastTimeMoved = std::clock();
    lastTimeBattled = lastTimeMoved;
    // TODO: choose random floats in defined location radius for each enemy in map
}

NPCEnemy::~NPCEnemy() {
    delete weapon;
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
    double diff = (double) (nowTime - lastTimeMoved) / (double) CLOCKS_PER_SEC;
    if (diff >= moveInterval) {
        // updating lastTimeMoved to latest move made time
        lastTimeMoved = nowTime;
        return true;
    }
    return false;
}

bool NPCEnemy::canGoToBattle() {
    std::clock_t nowTime = std::clock();
    double diff = (double) (nowTime - lastTimeBattled) / (double) CLOCKS_PER_SEC;
    if (diff >= battleTimeout) {
        lastTimeBattled = nowTime;
        return true;
    }
    return false;
}

bool NPCEnemy::canGoToWanderArea() {
    std::clock_t nowTime = std::clock();
    double diff = (double) (nowTime - lastTimeWandered) / (double) CLOCKS_PER_SEC;
    if (diff >= wanderTimeout) {
        lastTimeWandered = nowTime;
        return true;
    }
    return false;
}

bool NPCEnemy::isInBattleArea() {
    return battleAreaCircle->isPointInCircle(entityCircle->getCenter());
}

bool NPCEnemy::isInWanderArea() {
    return wanderAreaCircle->isPointInCircle(entityCircle->getCenter());
}

Circle *NPCEnemy::getWanderAreaCircle() {
    return wanderAreaCircle;
}

Circle *NPCEnemy::getBattleAreaCircle() {
    return battleAreaCircle;
}

void NPCEnemy::setMoveInterval(float interval) {
    moveInterval = interval;
}

// TODO: what to do here? how to use observers to my advantage
void NPCEnemy::notify() {
//    update();
}

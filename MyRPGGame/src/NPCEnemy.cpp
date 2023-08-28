#include "../include/NPCEnemy.hpp"

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

    // TextureLoader.getInstance()->loadTexture("dorio_64.png");
    texture.loadFromFile("../graphics/enemies/green_orcs/spritesheet.png");
    texture.setSmooth(true);
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    weapon = new Weapon(entityCircle->getCenter(), WeaponType::MACE);
    attackRangeCircle->setRadius(attackRangeCircle->getRadius() + weapon->getHitRadius());
    wanderAreaRadius = entityCircle->getRadius() * 12;
    wanderAreaCircle = new Circle(entityCircle->getCenter(), wanderAreaRadius);
    battleAreaRadius = (float) (wanderAreaRadius * 2.5);
    battleAreaCircle = new Circle(entityCircle->getCenter(), battleAreaRadius);

    lastTimeMoved = std::clock();
    lastTimeBattled = lastTimeMoved;
    // TODO: choose random floats in defined location radius for each enemy in map
}

NPCEnemy::~NPCEnemy() {
    delete weapon;
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

// direction is chosen randomly
bool NPCEnemy::canMove() {
    std::clock_t nowTime = std::clock();
    // checking if entity can move due to moveInterval value
    double diff = (double) (nowTime - lastTimeMoved) / (double) CLOCKS_PER_SEC;
    if (diff >= moveInterval) {
        // updating lastTimeMoved to latest move made time
        lastTimeMoved = nowTime;
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
    if (battleAreaCircle == nullptr) return false;
    return battleAreaCircle->isPointInCircle(entityCircle->getCenter());
}

bool NPCEnemy::isInWanderArea() {
    if (wanderAreaCircle == nullptr) return false;
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

// TODO: add more functionality or else use GameEntity's update method
void NPCEnemy::update(Point ***points, float dt) {
    if (!dead) {
        sprite->setPosition(position.x, position.y);
        // updating entity circle and attack circle
        entityCircle->setCenter(points[(int)position.y][(int)position.x]);
        attackRangeCircle->setCenter(points[(int)position.y][(int)position.x]);
    }
}

// TODO: what to do here? how to use observers to my advantage
void NPCEnemy::notify() {
    // updating battle state
    // enemy chased player and went out of battle area
    if (inBattle && !isInBattleArea()) inBattle = false;
}

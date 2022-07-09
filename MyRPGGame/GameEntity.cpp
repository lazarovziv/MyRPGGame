#include "GameEntity.hpp"

GameEntity::GameEntity() {
    level = 1;
    maxHealthPoints = 50;
    currentHealthPoints = maxHealthPoints;
    maxManaPoints = 30;
    currentManaPoints = maxManaPoints;
    attackPoints = 10;
    defencePoints = 5;
    speed = 1.f;
    inBattle = false;
    moveDirection = MoveDirection::RIGHT;
}

void GameEntity::increaseLevel(int amount) {
    level += amount;
}

void GameEntity::increaseMaxHealthPoints(int amount) {
    maxHealthPoints += amount;
    // also adding amount to current health points
    if (currentHealthPoints + amount >= maxHealthPoints) {
        currentHealthPoints = maxHealthPoints;
    } else currentHealthPoints += amount;
}

void GameEntity::increaseMaxManaPoints(int amount) {
    maxManaPoints += amount;
    // same like increaseMaxHealthPoints
    if (currentManaPoints + amount >= maxManaPoints) {
        currentManaPoints = maxManaPoints;
    } else currentManaPoints += amount;
}

void GameEntity::increaseSpeed(float amount) {
    // setting upper limit to 3
//    if (speed + amount < 3) {
//        speed += amount;
//    }
    speed += amount;
}

void GameEntity::increaseAttackPoints(int amount) {
    attackPoints += amount;
}

void GameEntity::increaseDefencePoints(int amount) {
    defencePoints += amount;
}

void GameEntity::changeInBattleState() {
    inBattle = !inBattle;
}

void GameEntity::setMoveDirection(MoveDirection direction) {
    moveDirection = direction;
}

void GameEntity::setX(float x) {
    position.x = x;
}

void GameEntity::setY(float y) {
    position.y = y;
}

void GameEntity::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

//void GameEntity::setCurrentGameMap(GameMap &map) {
//    currentGameMap = &map; // check if it works
//}
//
//GameMap* GameEntity::getCurrentGameMap() {
//    return currentGameMap;
//}

void GameEntity::decreaseMaxHealthPoints(int amount) {
    // don't let it decrease (throw an error)
    if (maxHealthPoints - amount <= 0) return;
    maxHealthPoints -= amount; // not changing currentHealthPoints like when increasing
}

void GameEntity::decreaseMaxManaPoints(int amount) {
    if (maxManaPoints - amount <= 0) return;
    maxManaPoints -= amount;
}

void GameEntity::decreaseSpeed(float speed) {
    if (this->speed - speed <= 0) return;
    this->speed -= speed;
}

void GameEntity::decreaseAttackPoints(int amount) {
    if (attackPoints - amount <= 0) return;
    attackPoints -= amount;
}

void GameEntity::decreaseDefencePoints(int amount) {
    if (defencePoints - amount <= 0) return;
    defencePoints -= amount;
}

long GameEntity::getID() {
    return id;
}

char* GameEntity::getName() {
    return name;
}

int GameEntity::getLevel() {
    return level;
}

int GameEntity::getMaxHealthPoints() {
    return maxHealthPoints;
}

int GameEntity::getCurrentHealthPoints() {
    return currentHealthPoints;
}

int GameEntity::getMaxManaPoints() {
    return maxManaPoints;
}

int GameEntity::getCurrentManaPoints() {
    return currentManaPoints;
}

int GameEntity::getAttackPoints() {
    return attackPoints;
}

int GameEntity::getDefencePoints() {
    return defencePoints;
}

float GameEntity::getSpeed() {
    return speed;
}

bool GameEntity::isInBattle() {
    return inBattle;
}

MoveDirection GameEntity::getMoveDirection() {
    return moveDirection;
}

Vector2f GameEntity::getPosition() {
    return position;
}

Sprite GameEntity::getSprite() {
    return sprite;
}

FloatRect GameEntity::getRectangle() {
    return sprite.getGlobalBounds();
}

void GameEntity::update() {
    sprite.setPosition(position);
}

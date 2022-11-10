#include "GameEntity.hpp"
#include <cmath>
#include <iostream>

GameEntity::GameEntity() {
    level = 1;
    maxHealthPoints = 50;
    currentHealthPoints = maxHealthPoints;
    maxManaPoints = 30;
    currentManaPoints = maxManaPoints;
    attackPoints = 1;
    defencePoints = 5;
    currentDefencePoints = defencePoints;
    speed = 1.f;
    inBattle = false;
    dead = false;
    moveDirection = MoveDirection::RIGHT;
    
//    movement = new GameEntityMovement(this);
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
    if (entityCircle != nullptr) {
        entityCircle->getCenter()->setX(position.x);
        entityCircle->getCenter()->setY(position.y);
        // if not created
    } else entityCircle = new Circle(position.x, position.y, 45);
}

void GameEntity::setWeapon(WeaponType type) {
    weapon = new Weapon(type);
//    // initializing attackRangeCircle according to weapon
//    if (attackRangeCircle != nullptr) {
//        attackRangeCircle->setRadius(entityCircle->getRadius() + weapon->getHitRadius());
//    } else {
//        attackRangeCircle = new Circle(position.x, position.y, entityCircle->getRadius() + weapon->getHitRadius());
//        cout << attackRangeCircle->getCenter()->getX() << endl;
//    }
}

void GameEntity::decreaseMaxHealthPoints(int amount) {
    // don't let it decrease (throw an error)
    if (maxHealthPoints - amount <= 0) return;
    maxHealthPoints -= amount; // not changing currentHealthPoints like when increasing
}

void GameEntity::decreaseCurrentHealthPoints(int amount) {
    if (currentHealthPoints > 0) {
        if (currentHealthPoints - amount <= 0) currentHealthPoints = 0;
        else currentHealthPoints -= amount;
    }
    if (currentHealthPoints <= 0) dead = true;
}

void GameEntity::decreaseMaxManaPoints(int amount) {
    if (maxManaPoints - amount <= 0) maxManaPoints = 0;
    maxManaPoints -= amount;
}

void GameEntity::decreaseSpeed(float speed) {
    if (this->speed - speed <= 0) this->speed = 0;
    this->speed -= speed;
}

void GameEntity::decreaseAttackPoints(int amount) {
    if (attackPoints - amount <= 0) attackPoints = 0;
    attackPoints -= amount;
}

void GameEntity::decreaseDefencePoints(int amount) {
    if (defencePoints - amount <= 0) defencePoints = 0;
    defencePoints -= amount;
}

void GameEntity::decreaseCurrentDefencePoints(int amount) {
    if (currentDefencePoints - amount <= 0) currentDefencePoints = 0;
    else currentDefencePoints -= amount;
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

int GameEntity::getCurrentDefencePoints() {
    return currentDefencePoints;
}

float GameEntity::getSpeed() {
    return speed;
}

bool GameEntity::isInBattle() {
    return inBattle;
}

bool GameEntity::isDead() {
    return dead;
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

void GameEntity::attack(GameEntity &entity) {
    // attack only if entity is in range
    if (isEntityInAttackRange(entity)) {
        std::cout << "In attack range!" << endl;
        std::cout << "Health Points: " << entity.currentHealthPoints << endl;
        std::cout << "Defence Points: " << entity.currentDefencePoints << endl;
        if (entity.currentDefencePoints > 0) {
            if (entity.currentDefencePoints - attackPoints < 0) {
                entity.currentDefencePoints = 0;
            } else entity.decreaseCurrentDefencePoints(attackPoints);
            // broke defence points
        } else {
            entity.decreaseCurrentHealthPoints(attackPoints);
        }
    }
}

bool GameEntity::move(MoveDirection direction) {
    return false;
}

bool GameEntity::isEntityInAttackRange(GameEntity &entity) {
    return attackRangeCircle->intersects(*(entity.entityCircle));
}

bool GameEntity::intersects(GameEntity &entity) {
    return entityCircle->intersects(*(entity.entityCircle));
}

Circle* GameEntity::getCircle() {
    return entityCircle;
}

Circle* GameEntity::getAttackRangeCircle() {
    return attackRangeCircle;
}

void GameEntity::update() {
    if (!dead) {
        sprite.setPosition(position);
        // updating entity circle and attack circle
        entityCircle->getCenter()->setX(position.x);
        entityCircle->getCenter()->setY(position.y);
        attackRangeCircle->getCenter()->setX(position.x);
        attackRangeCircle->getCenter()->setY(position.y);
    }
}

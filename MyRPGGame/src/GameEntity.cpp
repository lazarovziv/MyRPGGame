#include "../include/GameEntity.hpp"

GameEntity::GameEntity() {
    level = 1;
    maxHealthPoints = 50;
    currentHealthPoints = maxHealthPoints;
    maxManaPoints = 30;
    currentManaPoints = maxManaPoints;
    attackPoints = 1;
    defencePoints = 5;
    currentDefencePoints = defencePoints;
    speed = Constants::BASE_ENTITY_SPEED;
    inBattle = false;
    dead = false;
    moveDirection = MoveDirection::RIGHT;
    sprite = new Sprite();

    // TODO: add diagonal keys and values
    moveDirectionsSpritesMap[MoveDirection::DOWN] = 0;
    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 1;
    moveDirectionsSpritesMap[MoveDirection::LEFT] = 2;
    moveDirectionsSpritesMap[MoveDirection::UP] = 3;
    moveDirectionsSpritesMap[MoveDirection::UP_RIGHT] = 3;
    moveDirectionsSpritesMap[MoveDirection::UP_LEFT] = 3;
    moveDirectionsSpritesMap[MoveDirection::DOWN_RIGHT] = 0;
    moveDirectionsSpritesMap[MoveDirection::DOWN_LEFT] = 0;

//    movement = new GameEntityMovement(this);
}

GameEntity::GameEntity(Point *center) {
    level = 1;
    maxHealthPoints = 50;
    currentHealthPoints = maxHealthPoints;
    maxManaPoints = 30;
    currentManaPoints = maxManaPoints;
    attackPoints = 1;
    defencePoints = 5;
    currentDefencePoints = defencePoints;
    speed = Constants::BASE_ENTITY_SPEED;
    inBattle = false;
    dead = false;
    moveDirection = MoveDirection::RIGHT;
    sprite = new Sprite();

    // TODO: add diagonal keys and values
    moveDirectionsSpritesMap[MoveDirection::DOWN] = 0;
    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 1;
    moveDirectionsSpritesMap[MoveDirection::LEFT] = 2;
    moveDirectionsSpritesMap[MoveDirection::UP] = 3;
    moveDirectionsSpritesMap[MoveDirection::UP_RIGHT] = 3;
    moveDirectionsSpritesMap[MoveDirection::UP_LEFT] = 3;
    moveDirectionsSpritesMap[MoveDirection::DOWN_RIGHT] = 0;
    moveDirectionsSpritesMap[MoveDirection::DOWN_LEFT] = 0;

    entityCircle = new Circle(center, (float) Constants::TILE_SIZE/4);
    attackRangeCircle = new Circle(entityCircle->getCenter(), (float) Constants::TILE_SIZE/4);

    position.x = entityCircle->getCenter()->getX();
    position.y = entityCircle->getCenter()->getY();

    lastTimeBattled = std::clock();
//    movement = new GameEntityMovement(this);
}

GameEntity::~GameEntity() {
    delete sprite;
//    delete weapon;
    delete entityCircle;
    delete attackRangeCircle;
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

void GameEntity::increaseSpeed(int amount) {
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
    // increasing current defence points altogether
    currentDefencePoints += amount;
}

void GameEntity::changeInBattleState() {
    inBattle = !inBattle;
}

void GameEntity::setMoveDirection(MoveDirection direction) {
    moveDirection = direction;
}

void GameEntity::incrementStep() {
    if (step <= 2) step++;
    else step = 0;
}

void GameEntity::setX(int x) {
    position.x = x;
}

void GameEntity::setY(int y) {
    position.y = y;
}

void GameEntity::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
    if (entityCircle != nullptr) {
        entityCircle->getCenter()->setX(position.x);
        entityCircle->getCenter()->setY(position.y);
        // if not created
    }
//    else entityCircle = new Circle(position.x, position.y, (float) 3 * Constants::TILE_SIZE / 4);
}

void GameEntity::setPosition(Point *point) {
    position.x = point->getX();
    position.y = point->getY();
    if (entityCircle != nullptr) {
        entityCircle->setCenter(point);
        // if not created
    }
    if (attackRangeCircle != nullptr) {
        attackRangeCircle->setCenter(point);
    }
//    else entityCircle = new Circle(position.x, position.y, (float) 3 * Constants::TILE_SIZE / 4);
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

void GameEntity::setIsInBattle(bool inBattle) {
    this->inBattle = inBattle;
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

void GameEntity::decreaseSpeed(int speed) {
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

int GameEntity::getSpeed() {
    return speed;
}

int GameEntity::getStep() {
    return step;
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

map<MoveDirection, int> GameEntity::getMoveDirectionsSpritesMap() {
    return moveDirectionsSpritesMap;
}

Vector2i GameEntity::getPosition() {
    return position;
}

Sprite* GameEntity::getSprite() {
    return sprite;
}

IntRect GameEntity::getRectangle() {
    return (IntRect) sprite->getGlobalBounds();
}

void GameEntity::setIntRectPosition(int left, int top, int width, int height) {
    spriteRect.left = left;
    spriteRect.top = top;
    spriteRect.width = width;
    spriteRect.height = height;
    sprite->setTextureRect(spriteRect);
}

void GameEntity::attack(GameEntity &entity) {
    // attack only if entity is in range
    if (isEntityInAttackRange(entity)) {
        // setting entities' battle state
        inBattle = true;
        entity.setIsInBattle(true);
        std::cout << "In attack range!" << endl;
        std::cout << "Health Points: " << entity.currentHealthPoints << endl;
        std::cout << "Defence Points: " << entity.currentDefencePoints << endl;
        if (entity.currentDefencePoints > 0) {
            int defenceAttackPtsDiff = attackPoints - entity.currentDefencePoints;
            // if attack will break entity's defence
            if (defenceAttackPtsDiff > 0) {
                // zeroing defence points
                entity.decreaseCurrentDefencePoints(entity.currentDefencePoints);
                // decrease entity's health points by difference
                entity.decreaseCurrentHealthPoints(defenceAttackPtsDiff);
                // decrease defence points by attack points
            } else entity.decreaseCurrentDefencePoints(attackPoints);
            // broke defence points
        } else {
            entity.decreaseCurrentHealthPoints(attackPoints);
            // changing battle state for entities
            if (entity.isDead()) {
                entity.setIsInBattle(false);
                inBattle = false;
            }
        }
    }
}

bool GameEntity::canAttack() {
    std::clock_t nowTime = std::clock();
    double diff = (double) (nowTime - lastTimeBattled) / (double) CLOCKS_PER_SEC;
    if (diff >= battleTimeout) {
        lastTimeBattled = nowTime;
        return true;
    }
    return false;
}

bool GameEntity::didJustMove() {
    return justMoved;
}

void GameEntity::setJustMoved(bool flag) {
    justMoved = flag;
}

bool GameEntity::move(MoveDirection direction) {
    return false;
}

bool GameEntity::isEntityInAttackRange(GameEntity &entity) {
    return attackRangeCircle->intersects(entity.entityCircle);
}

bool GameEntity::intersects(GameEntity &entity) {
    return entityCircle->intersects(entity.entityCircle);
}

Circle* GameEntity::getCircle() {
    return entityCircle;
}

Circle* GameEntity::getAttackRangeCircle() {
    return attackRangeCircle;
}

void GameEntity::update(Point ***points) {
    if (!dead) {
        sprite->setPosition(position.x, position.y);
        // updating entity circle and attack circle
        entityCircle->setCenter(points[position.y][position.x]);
        attackRangeCircle->setCenter(points[position.y][position.x]);
    }
}

void GameEntity::pushToMoveStack(Point *move) {
    movesStack.push(move);
}

Point *GameEntity::popMove() {
    if (numOfMovesAvailable() == 0) return nullptr;
    auto *move = movesStack.top();
    movesStack.pop();
    return move;
}

int GameEntity::numOfMovesAvailable() {
    return movesStack.size();
}

bool GameEntity::areAvailableMoves() {
    return !movesStack.empty();
}

void GameEntity::clearMoveStack() {
    while (!movesStack.empty()) movesStack.pop();
}

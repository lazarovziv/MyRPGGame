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

//    moveDirectionsSpritesMap[MoveDirection::DOWN] = 10; // change to 3
    moveDirectionsSpritesMap[MoveDirection::DOWN] = 3;
//    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 11; // change to 4
    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 4;
//    moveDirectionsSpritesMap[MoveDirection::LEFT] = 9; // change to 2
    moveDirectionsSpritesMap[MoveDirection::LEFT] = 2;
//    moveDirectionsSpritesMap[MoveDirection::UP] = 8; // change to 1
    moveDirectionsSpritesMap[MoveDirection::UP] = 1;

    movementStateRowMap[EntityMovementState::IDLE] = Constants::IDLE_ROW;
    movementStateRowMap[EntityMovementState::CLIMB] = Constants::CLIMB_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = Constants::COMBAT_BACKSLASH_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = Constants::COMBAT_HALFSLASH_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = Constants::COMBAT_IDLE_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = Constants::COMBAT_SLASH_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::WALK] = Constants::WALK_ROW;
    movementStateRowMap[EntityMovementState::JUMP] = Constants::JUMP_ROW;
    movementStateRowMap[EntityMovementState::SITTING] = Constants::SITTING_ROW;
    movementStateRowMap[EntityMovementState::RUN] = Constants::RUN_ROW;
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
    maxStaminaPoints = 35;
    currentStaminaPoints = maxStaminaPoints;
    speed = Constants::BASE_ENTITY_SPEED;
    inBattle = false;
    dead = false;
    moveDirection = MoveDirection::DOWN;
    movementState = EntityMovementState::WALK;
    sprite = new Sprite();

    //    moveDirectionsSpritesMap[MoveDirection::DOWN] = 10; // change to 3
    moveDirectionsSpritesMap[MoveDirection::DOWN] = 3;
//    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 11; // change to 4
    moveDirectionsSpritesMap[MoveDirection::RIGHT] = 4;
//    moveDirectionsSpritesMap[MoveDirection::LEFT] = 9; // change to 2
    moveDirectionsSpritesMap[MoveDirection::LEFT] = 2;
//    moveDirectionsSpritesMap[MoveDirection::UP] = 8; // change to 1
    moveDirectionsSpritesMap[MoveDirection::UP] = 1;

    movementStateRowMap[EntityMovementState::IDLE] = Constants::IDLE_ROW;
    movementStateRowMap[EntityMovementState::CLIMB] = Constants::CLIMB_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = Constants::COMBAT_BACKSLASH_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = Constants::COMBAT_HALFSLASH_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = Constants::COMBAT_IDLE_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = Constants::COMBAT_SLASH_ONE_HANDED_ROW;
    movementStateRowMap[EntityMovementState::WALK] = Constants::WALK_ROW;
    movementStateRowMap[EntityMovementState::JUMP] = Constants::JUMP_ROW;
    movementStateRowMap[EntityMovementState::SITTING] = Constants::SITTING_ROW;
    movementStateRowMap[EntityMovementState::RUN] = Constants::RUN_ROW;

    entityCircle = make_unique<Circle>(center->getX(), center->getY(), (float) Constants::TILE_SIZE/4);
    entityRightCircle = make_unique<Circle>(center->getX() + speed, center->getY(), (float) Constants::TILE_SIZE/4);
    entityLeftCircle = make_unique<Circle>(center->getX() - speed, center->getY(), (float) Constants::TILE_SIZE/4);
    entityTopCircle = make_unique<Circle>(center->getX(), center->getY() - speed, (float) Constants::TILE_SIZE/4);
    entityBottomCircle = make_unique<Circle>(center->getX(), center->getY() + speed, (float) Constants::TILE_SIZE/4);

    attackRangeCircle = make_unique<Circle>(entityCircle->getCenter(), (float) Constants::TILE_SIZE/4);

    position.x = entityCircle->getCenter()->getX();
    position.y = entityCircle->getCenter()->getY();
}

GameEntity::~GameEntity() {
    delete sprite;
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

void GameEntity::setSpeed(float newSpeed) {
    speed = newSpeed;
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
    // adjusting weapon direction
    weapon->setTransitionDirection(direction);
}

void GameEntity::setMovementState(EntityMovementState state) {
    movementState = state;
}

void GameEntity::incrementStep() {
    if (step < Constants::NUM_STEPS) step++;
    else step = 0;
}

void GameEntity::setX(float x) {
    position.x = x;
}

void GameEntity::setY(float y) {
    position.y = y;
}

// TODO: add direction vector and normalize it for diagonal movement?
void GameEntity::setPosition(float x, float y) {
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

void GameEntity::setPosition(Vector2f directionVector, float dt) {
    position.x = position.x + speed * dt * directionVector.x;
    position.y = position.y + speed * dt * directionVector.y;
}

bool GameEntity::createMovementStateSprite(EntityMovementState state) {
    Texture textureToCreate;
    Sprite *spriteToCreate = new Sprite();
    switch (state) {
        case EntityMovementState::IDLE: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "idle/body_idle.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::CLIMB: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "climb/body_climb.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::WALK: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "walk/body_walk.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::JUMP: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "jump/body_jump.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::RUN: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "run/body_run.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::SITTING: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "sitting/body_sitting.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::COMBAT_IDLE_ONE_HANDED: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "combat/body_combat_1h_idle.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::COMBAT_SLASH_ONE_HANDED: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "combat/body_combat_1h_slash.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "combat/body_combat_1h_backslash.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
        case EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED: {
            if (!textureToCreate.loadFromFile(Constants::PLAYER_IMAGES_PATH + "combat/body_combat_1h_halfslash.png")) {
                cout << "Texture wasn't loaded properly." << endl;
                return false;
            }
            break;
        }
    }
    // texture is loaded

    // add texture to vector to keep in memory for future use
    movementStateTextures.push_back(textureToCreate);
    // load the texture into the sprite
    spriteToCreate->setTexture(textureToCreate);
    spriteToCreate->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection]*Constants::TILE_SIZE,
                                               0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    spriteToCreate->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    spriteToCreate->setPosition(position.x, position.y);
    // set created sprite as the movement state sprite
    movementStateSpritesMap[state] = spriteToCreate;
    return true;
}

bool GameEntity::addMovementStateSprite(EntityMovementState state, Sprite *newSprite) {
    // calling the function with a nullptr parameter only if we want to initialize a new sprite
    if (sprite == nullptr) {
        return createMovementStateSprite(state);
    }
    // we want to change the current sprite
    delete movementStateSpritesMap[state];
    // create the new one
    movementStateSpritesMap[state] = newSprite;
    return true;
}

void GameEntity::setWeapon(WeaponType type) {
    weapon = new Weapon(entityCircle->getCenter(), type);
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

int GameEntity::getMaxStaminaPoints() {
    return maxStaminaPoints;
}

int GameEntity::getCurrentStaminaPoints() {
    return currentStaminaPoints;
}

float GameEntity::getSpeed() {
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

EntityMovementState GameEntity::getMovementState() {
    return movementState;
}

map<MoveDirection, int> GameEntity::getMoveDirectionsSpritesMap() {
    return moveDirectionsSpritesMap;
}

map<EntityMovementState, int> GameEntity::getMovementStateRowMap() {
    return movementStateRowMap;
}

Vector2f GameEntity::getPosition() {
    return position;
}

Sprite* GameEntity::getSprite() {
    return sprite;
}

void GameEntity::setSprite(Sprite *newSprite) {
    sprite = newSprite;
}

Sprite* GameEntity::getMovementStateSprite(EntityMovementState state) {
    return movementStateSpritesMap[state];
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

bool GameEntity::canAttack() const {
    return battleInterval >= BATTLE_INTERVAL_DEFAULT;
}

void GameEntity::resetBattleInterval() {
    battleInterval = 0;
}

bool GameEntity::didJustMove() {
    return justMoved;
}

void GameEntity::setJustMoved(bool flag) {
    justMoved = flag;
}

// direction is chosen randomly
bool GameEntity::canGoIdle() const {
    return moveInterval >= MOVE_INTERVAL_DEFAULT;
}

void GameEntity::resetMoveInterval() {
    moveInterval = 0;
    resetIdleAnimationInterval();
}

void GameEntity::setLastTimeMoved(std::clock_t time) {

}

void GameEntity::setIsIdle(bool flag) {
    idle = flag;
}

bool GameEntity::isIdle() {
    return idle;
}

void GameEntity::resetDistanceTraveledSinceIdle() {
    distanceTraveledSinceIdle = 0;
}

void GameEntity::incrementDistanceTraveledSinceIdle(float distance) {
    distanceTraveledSinceIdle += distance;
}

bool GameEntity::canAnimateMovement() {
    if (distanceTraveledSinceIdle >= speed) {
        resetDistanceTraveledSinceIdle();
        return true;
    }
    return false;
}

bool GameEntity::canAnimateIdle() {
    if (idleAnimationInterval >= Constants::NUM_FRAMES_IDLE_ANIMATION) {
        resetIdleAnimationInterval();
        return true;
    }
    return false;
}

void GameEntity::resetIdleAnimationInterval() {
    idleAnimationInterval = 0;
}

void GameEntity::incrementIdleAnimationInterval(float dt) {
    idleAnimationInterval += dt;
}

bool GameEntity::move(MoveDirection direction) {
    return false;
}

bool GameEntity::isEntityInAttackRange(GameEntity &entity) {
    return attackRangeCircle->intersects(entity.entityCircle.get());
}

bool GameEntity::intersects(GameEntity &entity) {
    return entityCircle->intersects(entity.entityCircle.get());
}

Circle* GameEntity::getCircle() {
    return entityCircle.get();
}

Circle* GameEntity::getRightCircle() {
    return entityRightCircle.get();
}

Circle* GameEntity::getLeftCircle() {
    return entityLeftCircle.get();
}

Circle* GameEntity::getTopCircle() {
    return entityTopCircle.get();
}

Circle* GameEntity::getBottomCircle() {
    return entityBottomCircle.get();
}

Circle* GameEntity::getAttackRangeCircle() {
    return attackRangeCircle.get();
}

Weapon* GameEntity::getWeapon() {
    return weapon;
}

void GameEntity::update(Point ***points, float dt) {
    if (!dead) {
        sprite->setPosition(position.x, position.y);
        // updating intervals
        moveInterval += dt;
        battleInterval += dt;
        // updating entity circles and attack circle
        entityCircle->setCenter(position.x, position.y);
        entityRightCircle->setCenter(position.x + speed * dt, position.y);
        entityLeftCircle->setCenter(position.x - speed * dt, position.y);
        entityTopCircle->setCenter(position.x, position.y - speed * dt);
        entityBottomCircle->setCenter(position.x, position.y + speed * dt);

        attackRangeCircle->setCenter(position.x, position.y);
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

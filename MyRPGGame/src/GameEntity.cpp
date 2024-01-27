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
//    sprite = new sf::Sprite();
    sprite = std::make_unique<sf::Sprite>();

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

GameEntity::GameEntity(physics::Vector initialPosition, physics::RigidBodyType rigidBodyType,
                       const std::vector<physics::Vector> &vertices, real mass) {
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
//    sprite = new sf::Sprite();
    sprite = std::make_unique<sf::Sprite>();

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

    movementStateColMap[EntityMovementState::IDLE] = 0;
    movementStateColMap[EntityMovementState::CLIMB] = 0;
    movementStateColMap[EntityMovementState::COMBAT_BACKSLASH_ONE_HANDED] = 0;
    movementStateColMap[EntityMovementState::COMBAT_HALFSLASH_ONE_HANDED] = 0;
    movementStateColMap[EntityMovementState::COMBAT_IDLE_ONE_HANDED] = 0;
    movementStateColMap[EntityMovementState::COMBAT_SLASH_ONE_HANDED] = 0;
    movementStateColMap[EntityMovementState::WALK] = 0;
    movementStateColMap[EntityMovementState::JUMP] = 0;
    movementStateColMap[EntityMovementState::SITTING] = 0;
    movementStateColMap[EntityMovementState::RUN] = 0;

    switch (rigidBodyType) {
        case physics::RigidBodyType::CIRCLE: {
            rigidBody = std::make_unique<physics::Circle>(initialPosition.x, initialPosition.y, initialPosition.z,
                                                          (real) Constants::TILE_SIZE/4);
            break;
        }
        case physics::RigidBodyType::POLYGON: {
            rigidBody = std::make_unique<physics::Polygon>(initialPosition.x, initialPosition.y, initialPosition.z,
                                                           vertices, mass);
            break;
        }
        default:
            break;
    }
    // updating the mass because derived classes aren't setting new values to the mass in their constructors
    rigidBody->setMass(mass);

    position.x = rigidBody->getPosition().x;
    position.y = rigidBody->getPosition().y;
}

GameEntity::~GameEntity() {
//    delete sprite;
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

void GameEntity::increaseSpeed(const real amount) {
    // setting upper limit to 3
//    if (speed + amount < 3) {
//        speed += amount;
//    }
    speed += amount;
}

void GameEntity::setSpeed(real newSpeed) {
    speed = newSpeed;
}

void GameEntity::increaseAttackPoints(const int amount) {
    attackPoints += amount;
}

void GameEntity::increaseDefencePoints(const int amount) {
    defencePoints += amount;
    // increasing current defence points altogether
    currentDefencePoints += amount;
}

void GameEntity::changeInBattleState() {
    inBattle = !inBattle;
}

void GameEntity::setMoveDirection(const MoveDirection direction) {
    moveDirection = direction;
    // adjusting weapon direction
//    weapon->setTransitionDirection(direction);
}

void GameEntity::setMovementState(const EntityMovementState state) {
    movementState = state;
}

void GameEntity::incrementStep() {
    if (step < Constants::NUM_STEPS) step++;
    else step = 0;
}

void GameEntity::setX(const real x) {
    position.x = x;
}

void GameEntity::setY(const real y) {
    position.y = y;
}

// TODO: add direction vector and normalize it for diagonal movement?
void GameEntity::setPosition(const real x, const real y, const real z) {
    rigidBody->setPosition(x, y, z);
}

void GameEntity::setPosition(const physics::Vector &newPosition) {
    rigidBody->setPosition(newPosition.x, newPosition.y, newPosition.z);
}

void GameEntity::move(const physics::Vector &directionVector, const real dt) {
    // TODO: delete this after anti gravity force will be added
    if (directionVector == physics::Vector::ZERO) {
        positionUpdated = false;
        running = false;
        moving = false;
        return;
    }
    real currentSpeed = running ? speed * 2.f : speed; // multiply by dt?
    moving = true;

    rigidBody->addForce(directionVector * currentSpeed);
    // incrementing the distance traveled a bit lower than it should be when running to look realistic when animating
    incrementDistanceTraveledSinceIdle(running ? ((real) 2/3) * currentSpeed * dt : currentSpeed * dt);
    // setting the moveDirection used for animating
    setMoveDirection(directionVector);
    positionUpdated = true;
    // TODO: add reset to all entity movement states columns values besides the relevant state that'll be used
    // OR reset only the last state the player made
    for (auto &state : Constants::COMBAT_STATES) movementStateColMap[state] = 0;
}

void GameEntity::jump(physics::Vector &directionVector, const real dt) {
    real currentJumpScaler = running ? speed * 2 : speed;
    // TODO: make the force to be applied when the animation finishes
    physics::Vector jumpDirectionVector = physics::Vector::ZERO;
    if (directionVector == physics::Vector::ZERO) {
        // setting the force direction based on the move direction of the jump
        switch (moveDirection) {
            case MoveDirection::UP: {
                jumpDirectionVector = physics::Vector::UP_DIRECTION;
                // scaling the y value if entity means to jump vertically
                jumpDirectionVector.y *= currentJumpScaler;
                break;
            }
            case MoveDirection::DOWN: {
                jumpDirectionVector = physics::Vector::DOWN_DIRECTION;
                // scaling the y value if entity means to jump vertically
                jumpDirectionVector.y *= currentJumpScaler;
                break;
            }
            case MoveDirection::RIGHT: {
                jumpDirectionVector = physics::Vector::RIGHT_DIRECTION;
                // scaling the y value if entity means to jump horizontally
                jumpDirectionVector.x *= currentJumpScaler;
                break;
            }
            case MoveDirection::LEFT: {
                jumpDirectionVector = physics::Vector::LEFT_DIRECTION;
                // scaling the y value if entity means to jump horizontally
                jumpDirectionVector.x *= currentJumpScaler;
                break;
            }
            default: {
                jumpDirectionVector = physics::Vector::ZERO;
            }
        }
    }
    // incrementing the z axis for using high floors (to be implemented later)
    jumpDirectionVector.z += currentJumpScaler;
    // applying the force (should be applied when animation is about to finish)
    rigidBody->addForce(jumpDirectionVector);
    incrementJumpHeightSinceOnGround(currentJumpScaler * dt);
    setMoveDirection(directionVector);
}

void GameEntity::setMoveDirection(const physics::Vector directionVector) {
    real horizontalDirection = directionVector.x;
    real verticalDirection = directionVector.y;
    if (horizontalDirection > 0) moveDirection = MoveDirection::RIGHT;
    else if (horizontalDirection < 0) moveDirection = MoveDirection::LEFT;
    else {
        if (verticalDirection > 0) moveDirection = MoveDirection::DOWN;
        else if (verticalDirection < 0) moveDirection = MoveDirection::UP;
    }
}

void GameEntity::setWeapon(const WeaponType type) {
    weapon = std::make_unique<Weapon>(rigidBody->getPosition(), type);
}

void GameEntity::setIsInBattle(const bool flag) {
    this->inBattle = flag;
}

void GameEntity::decreaseMaxHealthPoints(const real amount) {
    // don't let it decrease (throw an error)
    if (maxHealthPoints - amount <= 0) return;
    maxHealthPoints -= amount; // not changing currentHealthPoints like when increasing
}

void GameEntity::decreaseCurrentHealthPoints(const real amount) {
    if (currentHealthPoints > 0) {
        if (currentHealthPoints - amount <= 0) currentHealthPoints = 0;
        else currentHealthPoints -= amount;
    }
    if (currentHealthPoints <= 0) dead = true;
}

void GameEntity::decreaseMaxManaPoints(const real amount) {
    if (maxManaPoints - amount <= 0) maxManaPoints = 0;
    maxManaPoints -= amount;
}

void GameEntity::decreaseSpeed(const real amount) {
    if (this->speed - amount <= 0) this->speed = 0;
    this->speed -= amount;
}

void GameEntity::decreaseAttackPoints(const real amount) {
    if (attackPoints - amount <= 0) attackPoints = 0;
    attackPoints -= amount;
}

void GameEntity::decreaseDefencePoints(const real amount) {
    if (defencePoints - amount <= 0) defencePoints = 0;
    defencePoints -= amount;
}

void GameEntity::decreaseCurrentDefencePoints(const real amount) {
    if (currentDefencePoints - amount <= 0) currentDefencePoints = 0;
    else currentDefencePoints -= amount;
}

long GameEntity::getID() const {
    return id;
}

char* GameEntity::getName() {
    return name;
}

int GameEntity::getLevel() const {
    return level;
}

real GameEntity::getMaxHealthPoints() const {
    return maxHealthPoints;
}

real GameEntity::getCurrentHealthPoints() const {
    return currentHealthPoints;
}

real GameEntity::getMaxManaPoints() const {
    return maxManaPoints;
}

real GameEntity::getCurrentManaPoints() const {
    return currentManaPoints;
}

real GameEntity::getAttackPoints() const {
    return attackPoints;
}

real GameEntity::getDefencePoints() const {
    return defencePoints;
}

real GameEntity::getCurrentDefencePoints() const {
    return currentDefencePoints;
}

real GameEntity::getMaxStaminaPoints() const {
    return maxStaminaPoints;
}

real GameEntity::getCurrentStaminaPoints() const {
    return currentStaminaPoints;
}

real GameEntity::getSpeed() const {
    return speed;
}

int GameEntity::getStep() const {
    return step;
}

bool GameEntity::isInBattle() const {
    return inBattle;
}

bool GameEntity::isDead() const {
    return dead;
}

MoveDirection GameEntity::getMoveDirection() const {
    return moveDirection;
}

EntityMovementState GameEntity::getMovementState() const {
    return movementState;
}

std::map<MoveDirection, int> GameEntity::getMoveDirectionsSpritesMap() const {
    return moveDirectionsSpritesMap;
}

int GameEntity::getMovementStateColCount(const EntityMovementState state) const {
    return movementStateColMap.at(state);
}

void GameEntity::incrementMovementStateColCount(const EntityMovementState state) {
    if (movementStateColMap[state] < Constants::MOVEMENT_STATE_NUM_COLS.at(state)-1) {
        movementStateColMap[state] = movementStateColMap[state] + 1;
        return;
    }
    movementStateColMap[state] = 0;
}

void GameEntity::resetMovementStateColCount(const EntityMovementState state) {
    movementStateColMap[state] = 0;
}

std::map<EntityMovementState, int> GameEntity::getMovementStateRowMap() {
    return movementStateRowMap;
}

sf::Vector2f GameEntity::getRenderPosition() const {
    return position;
}

sf::Sprite* GameEntity::getSprite() const {
    return sprite.get();
}

void GameEntity::setSprite(sf::Sprite *newSprite) {
//    sprite = newSprite;
}

sf::Sprite* GameEntity::getMovementStateSprite(const EntityMovementState state) {
    return movementStateSpritesMap[state];
}

sf::IntRect GameEntity::getRectangle() const {
    return (sf::IntRect) sprite->getGlobalBounds();
}

void GameEntity::setIntRectPosition(const int left, const int top, const int width, const int height) {
    spriteRect.left = left;
    spriteRect.top = top;
    spriteRect.width = width;
    spriteRect.height = height;
    sprite->setTextureRect(spriteRect);
}

physics::RigidBody* GameEntity::getRigidBody() const {
    return rigidBody.get();
}

physics::Vector GameEntity::getPosition() const {
    return (*rigidBody).getPosition();
}

bool GameEntity::isRunning() const {
    return running;
}

void GameEntity::setIsRunning(const bool flag) {
    running = flag;
}

bool GameEntity::isMoving() const {
    return moving;
}

void GameEntity::setIsMoving(const bool flag) {
    moving = flag;
}

bool GameEntity::isAttacking() const {
    return attacking;
}

void GameEntity::setIsAttacking(const bool flag) {
    attacking = flag;
}

bool GameEntity::isJumping() const {
    return jumping;
}

void GameEntity::setIsJumping(const bool flag) {
    jumping = flag;
}

bool GameEntity::canAttack() const {
    return battleInterval >= BATTLE_INTERVAL_DEFAULT && !moving;
}

void GameEntity::resetBattleInterval() {
    battleInterval = 0;
}

void GameEntity::increaseBattleInterval(const real dt) {
    battleInterval += dt;
}

void GameEntity::resetBattleIntervalForSwing() {
    battleInterval = -SWING_INTERVAL_DEFAULT;
}

bool GameEntity::didJustMove() const {
    return justMoved;
}

void GameEntity::setJustMoved(const bool flag) {
    justMoved = flag;
}

// direction is chosen randomly
bool GameEntity::canGoIdle() const {
    return moveInterval >= MOVE_INTERVAL_DEFAULT;
}

bool GameEntity::canChangeDirection() const {
    return changeMoveDirectionInterval >= CHANGE_MOVE_DIRECTION_INTERVAL;
}

void GameEntity::resetChangeDirectionInterval() {
    changeMoveDirectionInterval = 0;
}

void GameEntity::resetMoveInterval() {
    moveInterval = 0;
    resetIdleAnimationInterval();
}

void GameEntity::setIsIdle(const bool flag) {
    idle = flag;
}

bool GameEntity::isIdle() const {
    return idle;
}

void GameEntity::resetDistanceTraveledSinceIdle() {
    distanceTraveledSinceIdle = 0;
}

void GameEntity::incrementDistanceTraveledSinceIdle(const real distance) {
    distanceTraveledSinceIdle += distance;
}

void GameEntity::incrementJumpHeightSinceOnGround(const real distance) {
    jumpHeightSinceOnGround += distance;
}

bool GameEntity::canAnimateMovement(const bool check) {
    if (distanceTraveledSinceIdle >= speed) {
        if (!check) resetDistanceTraveledSinceIdle();
        return true;
    }
    return false;
}

bool GameEntity::canAnimateIdle(const bool check) {
    if (idleAnimationInterval >= Constants::NUM_FRAMES_IDLE_ANIMATION) {
        if (!check) resetIdleAnimationInterval();
        return true;
    }
    return false;
}

bool GameEntity::canAnimateJump(const bool check) {
    if (jumpHeightSinceOnGround >= JUMP_HEIGHT_INTERVAL_DEFAULT) {
        if (!check) resetJumpHeightSinceOnGroundInterval();
        return true;
    }
    return false;
}

void GameEntity::resetIdleAnimationInterval() {
    idleAnimationInterval = 0;
//    resetMovementStateColCount(EntityMovementState::COMBAT_SLASH_ONE_HANDED);
//    resetMovementStateColCount(EntityMovementState::WALK);
}

void GameEntity::resetJumpHeightSinceOnGroundInterval() {
    jumpHeightSinceOnGround = 0;
}

void GameEntity::resetJumpInterval() {
//    jumpHeightSinceOnGround = -JUMP_INTERVAL_DEFAULT;
    jumpHeightSinceOnGround = 0;
}

void GameEntity::incrementIdleAnimationInterval(const real dt) {
    idleAnimationInterval += dt;
}

Weapon* GameEntity::getWeapon() {
    return weapon.get();
}

void GameEntity::printPosition() const {
    rigidBody->getPosition().printCoordinates();
}

void GameEntity::update(const real dt) {
    if (!dead) {
        rigidBody->update(dt);

        position.x = rigidBody->getPosition().x;
        position.y = rigidBody->getPosition().y;
        sprite->setPosition(position.x, position.y);
        // updating intervals (checking values to prevent overflow if not acted long enough)
        moveInterval += dt;
        battleInterval += dt;
        if (!isPlayer) changeMoveDirectionInterval += dt;
    }
}

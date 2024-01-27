#include "../include/GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, std::shared_ptr<GameMap> map) {
    this->entity = entity;
    currentMap = map;

    animationManager = std::make_unique<AnimationManager>(this->entity);
}

// expecting a normalized vector (no check of norma value to avoid sqrt invocation)
Constants::MoveSuccessValues GameEntityMovement::move(const physics::Vector &direction, const real dt) {
    // no need to create extra variables if not using them
    if (direction == physics::Vector::ZERO) {
        entity->move(direction, dt);
        animationManager->animate(EntityMovementState::IDLE, dt);
        // since we're idle, resetting the travel we covered until now
        entity->resetDistanceTraveledSinceIdle();
        entity->incrementIdleAnimationInterval(dt);
        return Constants::MoveSuccessValues::NOT_MOVED;
    }
    // direction vector is passed normalized
    entity->move(direction, dt);
    // TODO: handle collisions in the physics manager
    return Constants::MoveSuccessValues::SUCCESS;
}

bool GameEntityMovement::moveRandomly(const int randomDirection, const real dt) {
    // DOWN, RIGHT, LEFT, UP
    switch (randomDirection) {
        case 0:
            if (move(physics::Vector::RIGHT_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animationManager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        case 1:
            if (move(physics::Vector::LEFT_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animationManager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        case 2:
            if (move(physics::Vector::UP_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animationManager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        case 3:
            if (move(physics::Vector::DOWN_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animationManager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        default: {
            move(physics::Vector::ZERO, dt); // don't move
            return true;
        }
    }
    return false;
}

void GameEntityMovement::setEntity(GameEntity &e) {
    this->entity = &e;
    animationManager->setEntity(entity);
}

GameEntity *GameEntityMovement::getEntity() {
    return entity;
}

void GameEntityMovement::setCurrentMap(std::shared_ptr<GameMap> map) {
    currentMap = map;
}

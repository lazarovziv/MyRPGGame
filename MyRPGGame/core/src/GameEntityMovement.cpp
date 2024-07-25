#include "../include/GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, std::shared_ptr<GameMap> map) {
    this->entity = entity;
    current_map = map;

    animation_manager = std::make_unique<AnimationManager>(this->entity);
}

// expecting a normalized vector (no check of norma value to avoid sqrt invocation)
Constants::MoveSuccessValues GameEntityMovement::move(const physics::Vector &direction, const real dt) {
    // no need to create extra variables if not using them
    if (direction == physics::Vector::ZERO) {
        entity->move(direction, dt);
        animation_manager->animate(EntityMovementState::IDLE, dt);
        // since we're idle, resetting the travel we covered until now
        entity->reset_distance_traveled_since_idle();
        entity->increment_idle_animation_interval(dt);
        return Constants::MoveSuccessValues::NOT_MOVED;
    }
    // direction vector is passed normalized
    entity->move(direction, dt);
    // TODO: handle collisions in the physics manager
    return Constants::MoveSuccessValues::SUCCESS;
}

bool GameEntityMovement::jump(const physics::Vector &direction, const real dt) {
    // if jump wasn't successful
    if (!entity->jump(direction, dt)) return false;
    // in the middle of a jump
    if (!entity->is_jumping()) {
        entity->set_is_jumping(true);
    }
    animation_manager->animate(EntityMovementState::JUMP, dt);
    return true;
}

bool GameEntityMovement::move_randomly(const int randomDirection, const real dt) {
    // DOWN, RIGHT, LEFT, UP
    switch (randomDirection) {
        case 0:
            if (move(physics::Vector::RIGHT_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animation_manager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        case 1:
            if (move(physics::Vector::LEFT_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animation_manager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        case 2:
            if (move(physics::Vector::UP_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animation_manager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        case 3:
            if (move(physics::Vector::DOWN_DIRECTION, dt) == Constants::MoveSuccessValues::SUCCESS) {
                animation_manager->animate(EntityMovementState::WALK, dt);
                return true;
            }
        default: {
            move(physics::Vector::ZERO, dt); // don't move
            return true;
        }
    }
    return false;
}

void GameEntityMovement::set_entity(GameEntity &e) {
    this->entity = &e;
    animation_manager->set_entity(entity);
}

GameEntity *GameEntityMovement::get_entity() {
    return entity;
}

void GameEntityMovement::set_current_map(std::shared_ptr<GameMap> map) {
    current_map = map;
}

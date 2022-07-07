#include "GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(GameEntity* entity) {
    this->entity = entity;
}

bool GameEntityMovement::move(MoveDirection direction) {
    GameMap* map = entity->getCurrentGameMap();
    // size of array
    int unreachableAreasSize = sizeof(*(map->getUnreachableAreas())) / sizeof(map->getUnreachableAreas()[0]);
    bool canCollide = false;
    
    float entitySpeed = entity->getSpeed();
    FloatRect entityRect = entity->getRectangle();
    
    if (direction == MoveDirection::UP) {
        if (entity->getPosition().y - entity->getSpeed() < 0) return false;
        // temp rectangle for moving up
        FloatRect rect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
        // check if the move can ovelap with any of the unreachable areas
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x, entity->getPosition().y - entitySpeed);
        }
    } else if (direction == MoveDirection::DOWN) {
        if (entity->getPosition().y + entity->getSpeed() >= screenHeight) return false;
        // temp rectangle for moving down
        FloatRect rect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
        // check if the move can ovelap with any of the unreachable areas
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x, entity->getPosition().y + entitySpeed);
        }
    } else if (direction == MoveDirection::RIGHT) {
        if (entity->getPosition().x + entity->getSpeed() >= screenWidth) return false;
        // temp rectangle for moving right
        FloatRect rect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x + entitySpeed, entity->getPosition().y);
        }
    } else if (direction == MoveDirection::LEFT) {
        if (entity->getPosition().x - entity->getSpeed() < 0) return false;
        FloatRect rect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x - entitySpeed, entity->getPosition().y);
        }
    }
    
    entity->setMoveDirection(direction);
    return true;
}

void GameEntityMovement::setScreenWidth(int width) {
    screenWidth = width;
}

void GameEntityMovement::setScreenHeight(int height) {
    screenHeight = height;
}

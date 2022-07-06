#include "GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(Game* game, GameEntity* entity, GameMap* map) {
    this->game = game;
    this->entity = entity;
    this->map = map;
}

bool GameEntityMovement::move(MoveDirection direction) {
    // size of array
    int unreachableAreasSize = sizeof(*(map->getUnreachableAreas())) / sizeof(map->getUnreachableAreas()[0]);
    bool canCollide = false;
    
    if (direction == MoveDirection::UP) {
        if (entity->getPosition().y - entity->getSpeed() < 0) return false;
        // temp rectangle for moving up
        FloatRect rect(entity->getRectangle().left, entity->getRectangle().top - entity->getSpeed(), entity->getRectangle().width, entity->getRectangle().height);
        // check if the move can ovelap with any of the unreachable areas
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x, entity->getPosition().y - entity->getSpeed());
        }
    } else if (direction == MoveDirection::DOWN) {
        if (entity->getPosition().y + entity->getSpeed() >= game->getScreenHeight()) return false;
        // temp rectangle for moving down
        FloatRect rect(entity->getRectangle().left, entity->getRectangle().top + entity->getSpeed(), entity->getRectangle().width, entity->getRectangle().height);
        // check if the move can ovelap with any of the unreachable areas
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x, entity->getPosition().y + entity->getSpeed());
        }
    } else if (direction == MoveDirection::RIGHT) {
        if (entity->getPosition().x + entity->getSpeed() >= game->getScreenWidth()) return false;
        // temp rectangle for moving right
        FloatRect rect(entity->getRectangle().left + entity->getSpeed(), entity->getRectangle().top, entity->getRectangle().width, entity->getRectangle().height);
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x + entity->getSpeed(), entity->getPosition().y);
        }
    } else if (direction == MoveDirection::LEFT) {
        if (entity->getPosition().x - entity->getSpeed() < 0) return false;
        FloatRect rect(entity->getRectangle().left - entity->getSpeed(), entity->getRectangle().top, entity->getRectangle().width, entity->getRectangle().height);
        for (int i = 0; i < unreachableAreasSize; i++) {
            if (map->getUnreachableAreas()[i].intersects(rect)) {
                canCollide = true;
                break;
            }
        }
        if (!canCollide) {
            // in legal boundaries
            entity->setPosition(entity->getPosition().x - entity->getSpeed(), entity->getPosition().y);
        }
    }
    
    entity->setMoveDirection(direction);
    return true;
}

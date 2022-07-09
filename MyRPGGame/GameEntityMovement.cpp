#include "GameEntityMovement.hpp"
#include "Game.hpp"

GameEntityMovement::GameEntityMovement(GameEntity* entity) {
    this->entity = entity;
    screenWidth = Game::SCREEN_WIDTH;
    screenHeight = Game::SCREEN_HEIGHT;
    tileSize = Game::TILE_SIZE;
}

bool GameEntityMovement::move(MoveDirection direction) {
    GameMap* map = Game::getInstance()->getCurrentGameMap();
    // size of array
    int unreachableAreasSize = map->getNumOfUnreachableAreas();
    
    float entitySpeed = entity->getSpeed();
    FloatRect entityRect = entity->getRectangle();
    
    float entityX = entity->getPosition().x;
    float entityY = entity->getPosition().y;
    
    bool canCollide = false;
    
    if (direction == MoveDirection::UP) {
        if (entity->getPosition().y - entitySpeed <= tileSize/2) {
            // check if edge of screen is an exit point for current game map
            if (map->isExitableFromTop()) {
                // check if we're at current map's exit point
                if (entityX <= map->getTopExitMaxX() && entityX >= map->getTopExitMinX()) {
                    cout << "Reached Top Exit" << endl;
                    // set current map to map above
                    Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
                    // set entity position to down enter point (changed current map)
                    float targetX = (Game::getInstance()->getCurrentGameMap()->getBottomEnterMinX() +  Game::getInstance()->getCurrentGameMap()->getBottomEnterMaxX()) / 2;
                    float targetY = Game::SCREEN_HEIGHT - tileSize/2;
                    entity->setPosition(targetX, targetY);
                    return true;
                }
            }
            entity->setPosition(entity->getPosition().x, tileSize/2);
            return false;
        }
        // temp rectangle for moving up
        FloatRect rect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
        // check if the move can ovelap with any of the unreachable areas
        for (int i = 0; i < unreachableAreasSize; i++) {
//            // bottom y of area because going up
//            float areaY = map->getUnreachableAreas()[i].top + map->getUnreachableAreas()[i].height;
//            float minY = min(entityRect.top - entitySpeed, areaY);
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
        if (entity->getPosition().y + entitySpeed >= screenHeight - tileSize/2) {
            entity->setPosition(entity->getPosition().x, screenHeight - tileSize/2);
            return false;
        }
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
        if (entity->getPosition().x + entitySpeed >= screenWidth - tileSize/2) {
            entity->setPosition(screenWidth - tileSize/2, entity->getPosition().y);
            return false;
        }
        // temp rectangle for moving right
        FloatRect rect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
        // check if the move can ovelap with any of the unreachable areas
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
        if (entity->getPosition().x - entitySpeed <= tileSize/2) {
            entity->setPosition(tileSize/2, entity->getPosition().y);
            return false;
        }
        // temp rectangle for moving left
        FloatRect rect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
        // check if the move can ovelap with any of the unreachable areas
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
    return canCollide;
}

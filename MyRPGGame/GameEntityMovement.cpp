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
            // check if top of screen is an exit point for current game map
            if (map->isExitableFromTop()) {
                // check if we're at current map's top exit point
                if (entityX + tileSize/2 <= map->getTopExitMaxX() && entityX - tileSize/2 >= map->getTopExitMinX()) {
                    cout << "Reached Top Exit" << endl;
                    // set current map to map above
                    Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() - 1);
                    // set entity position to down enter point (changed current map)
                    float targetY = screenHeight - tileSize/2;
                    entity->setPosition(entityX, targetY);
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
            // check if bottom of screen is an exit point for current game map
            if (map->isExitableFromBottom()) {
                // check if we're at current map's top exit point
                if (entityX + tileSize/2 <= map->getBottomExitMaxX() && entityX - tileSize/2 >= map->getBottomExitMinX()) {
                    cout << "Reached Bottom Exit" << endl;
                    // set current map to map below
                    Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
                    // set entity position to top enter point (changed current map)
                    float targetY = tileSize/2;
                    entity->setPosition(entityX, targetY);
                    return true;
                }
            }
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
            // check if right of screen is an exit point for current game map
            if (map->isExitableFromRight()) {
                // check if we're at current map's right exit point
                if (entityY + tileSize/2 <= map->getRightExitMinY() && entityY - tileSize/2 >= map->getRightExitMaxY()) {
                    cout << "Reached Right Exit" << endl;
                    // set current map to map to the right
                    Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
                    // set entity position to right enter point (changed current map)
                    float targetX = tileSize/2;
                    entity->setPosition(targetX, entityY);
                    return true;
                }
            }
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
            // check if left of screen is an exit point for current game map
            if (map->isExitableFromLeft()) {
                // check if we're at current map's left exit point
                if (entityY + tileSize/2 <= map->getLeftExitMinY() && entityY - tileSize/2 >= map->getLeftExitMaxY()) {
                    cout << "Reached Left Exit" << endl;
                    // set current map to map to the left
                    Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
                    // set entity position to left enter point (changed current map)
                    float targetX = screenWidth - tileSize/2;
                    entity->setPosition(targetX, entityY);
                    return true;
                }
            }
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

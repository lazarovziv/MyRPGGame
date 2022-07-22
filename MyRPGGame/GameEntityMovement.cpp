#include "GameEntityMovement.hpp"
#include "Game.hpp"
#include "Constants.h"

GameEntityMovement::GameEntityMovement(GameEntity* entity) {
    this->entity = entity;
    screenWidth = Constants::SCREEN_WIDTH;
    screenHeight = Constants::SCREEN_HEIGHT;
    tileSize = Constants::TILE_SIZE;
}

bool GameEntityMovement::move(MoveDirection direction) {
    GameMap* map = Game::getInstance()->getCurrentGameMap();
    
    float entitySpeed = entity->getSpeed();
    FloatRect entityRect = entity->getRectangle();
    
    float entityX = entity->getPosition().x;
    float entityY = entity->getPosition().y;
    
    if (direction == MoveDirection::UP) {
        // try and move up, if false, collided with something
        if (moveUp(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::UP);
            return true;
        } else return false;
    } else if (direction == MoveDirection::DOWN) {
        // try and move down, if false, collided with something
        if (moveDown(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::DOWN);
            return true;
        } else return false;
    } else if (direction == MoveDirection::RIGHT) {
        // try and move right, if false, collided with something
        if (moveRight(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::RIGHT);
            return true;
        } else return false;
    } else if (direction == MoveDirection::LEFT) {
        // try and move left, if false, collided with something
        if (moveLeft(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::LEFT);
            return true;
        } else return false;
    }
    return false;
}

bool GameEntityMovement::moveUp(GameMap* map, float entityX, float entityY, float entitySpeed, FloatRect &entityRect) {
    int unreachableAreasSize = map->getNumOfUnreachableAreas();
    bool canCollide = false;
    // reached top of screen
    if (entityY - entitySpeed <= tileSize/2) {
        // check if top of screen is an exit point for current game map
        if (map->isExitableFromTop()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize/2 <= map->getTopExitMaxX() && entityX - tileSize/2 >= map->getTopExitMinX()) {
                cout << "Reached Top Exit" << endl;
                // set current map to map above
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() - 1, map->getWorldMapCol());
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() - 1);
                // set entity position to down enter point (changed current map)
                entity->setPosition(entityX, screenHeight - tileSize/2);
                return true;
            }
        }
        entity->setPosition(entityX, tileSize/2);
        return false;
    }
    // temp rectangle for moving up
    FloatRect rect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
    // temp circle for moving up
    Circle circle(entity->getCircle()->getCenter()->getX(), entity->getCircle()->getCenter()->getY() - entitySpeed, entity->getCircle()->getRadius());
    // check if the move can ovelap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getUnreachableAreasSprites()[i].getGlobalBounds().intersects(rect)) {
            canCollide = true;
            break;
        }
    }
    // check for collisions with enemies
    for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
        if (map->getEnemies()[i].getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }
    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(entityX, entityY - entitySpeed);
    }
    return !canCollide;
}

bool GameEntityMovement::moveDown(GameMap *map, float entityX, float entityY, float entitySpeed, FloatRect &entityRect) {
    int unreachableAreasSize = map->getNumOfUnreachableAreas();
    bool canCollide = false;
    // reached bottom of screen
    if (entityY + entitySpeed >= screenHeight - tileSize/2) {
        // check if bottom of screen is an exit point for current game map
        if (map->isExitableFromBottom()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize/2 <= map->getBottomExitMaxX() && entityX - tileSize/2 >= map->getBottomExitMinX()) {
                cout << "Reached Bottom Exit" << endl;
                // set current map to map below
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() + 1, map->getWorldMapCol());
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
                // set entity position to top enter point (changed current map)
                entity->setPosition(entityX, tileSize/2);
                return true;
            }
        }
        entity->setPosition(entityX, screenHeight - tileSize/2);
        return false;
    }
    // temp rectangle for moving down
    FloatRect rect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
    // temp circle for moving down
    Circle circle(entity->getCircle()->getCenter()->getX(), entity->getCircle()->getCenter()->getY() + entitySpeed, entity->getCircle()->getRadius());
    // check if the move can ovelap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getUnreachableAreasSprites()[i].getGlobalBounds().intersects(rect)) {
            canCollide = true;
            break;
        }
    }
    // check for collisions with enemies
    for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
        if (map->getEnemies()[i].getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }
    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(entityX, entityY + entitySpeed);
    }
    return !canCollide;
}

bool GameEntityMovement::moveRight(GameMap *map, float entityX, float entityY, float entitySpeed, FloatRect &entityRect) {
    int unreachableAreasSize = map->getNumOfUnreachableAreas();
    bool canCollide = false;
    // reached right of screen
    if (entityX + entitySpeed >= screenWidth - tileSize/2) {
        // check if right of screen is an exit point for current game map
        if (map->isExitableFromRight()) {
            // check if we're at current map's right exit point
            if (entityY + tileSize/2 <= map->getRightExitMinY() && entityY - tileSize/2 >= map->getRightExitMaxY()) {
                cout << "Reached Right Exit" << endl;
                // set current map to map to the right
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() + 1);
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
                // set entity position to right enter point (changed current map)
                entity->setPosition(tileSize/2, entityY);
                return true;
            }
        }
        entity->setPosition(screenWidth - tileSize/2, entityY);
        return false;
    }
    // temp rectangle for moving right
    FloatRect rect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp circle for moving right
    Circle circle(entity->getCircle()->getCenter()->getX() + entitySpeed, entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // check if the move can ovelap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getUnreachableAreasSprites()[i].getGlobalBounds().intersects(rect)) {
            canCollide = true;
            break;
        }
    }
    // check for collisions with enemies
    for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
        if (map->getEnemies()[i].getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }
    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(entityX + entitySpeed, entityY);
    }
    return !canCollide;
}

bool GameEntityMovement::moveLeft(GameMap *map, float entityX, float entityY, float entitySpeed, FloatRect &entityRect) {
    int unreachableAreasSize = map->getNumOfUnreachableAreas();
    bool canCollide = false;
    // reached left of screen
    if (entityX - entitySpeed <= tileSize/2) {
        // check if left of screen is an exit point for current game map
        if (map->isExitableFromLeft()) {
            // check if we're at current map's left exit point
            if (entityY + tileSize/2 <= map->getLeftExitMinY() && entityY - tileSize/2 >= map->getLeftExitMaxY()) {
                cout << "Reached Left Exit" << endl;
                // set current map to map to the left
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() - 1);
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
                // set entity position to left enter point (changed current map)
                entity->setPosition(screenWidth - tileSize/2, entityY);
                return true;
            }
        }
        entity->setPosition(tileSize/2, entityY);
        return false;
    }
    // temp rectangle for moving left
    FloatRect rect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp circle for moving left
    Circle circle(entity->getCircle()->getCenter()->getX() - entitySpeed, entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // check if the move can ovelap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getUnreachableAreasSprites()[i].getGlobalBounds().intersects(rect)) {
            canCollide = true;
            break;
        }
    }
    // check for collisions with enemies
    for (int i = 0; i < map->getNumOfCurrentEnemies(); i++) {
        if (map->getEnemies()[i].getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }
    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(entityX - entitySpeed, entityY);
    }
    return !canCollide;
}

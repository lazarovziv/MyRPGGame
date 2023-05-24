#include "../include/GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, GameMap *map) {
    this->entity = entity;
    currentMap = map;
    isPlayer = player;
    screenWidth = Constants::FULL_SCREEN_WIDTH;
    screenHeight = Constants::FULL_SCREEN_HEIGHT;
    tileSize = Constants::TILE_SIZE;
}

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, GameMap *map, Point ***points) {
    this->entity = entity;
    currentMap = map;
    isPlayer = player;
    screenWidth = Constants::FULL_SCREEN_WIDTH;
    screenHeight = Constants::FULL_SCREEN_HEIGHT;
    tileSize = Constants::TILE_SIZE;
    gameMapsPoints = points;
}

bool GameEntityMovement::moveTowardsEntity(GameEntity *gameEntity, Graph<Point *> *graph) {
    // using A* algorithm as dijkstra was really memory inefficient (maybe my implementation wasn't good enough)
    std::vector<Point *> *paths = graph->findPathTo(entity->getCircle()->getCenter(), gameEntity->getCircle()->getCenter());
    if (paths == nullptr) return false;
    // calling this function means gameEntity has repositioned
    entity->clearMoveStack();
    // insert all points
    for (auto &pair: *paths) {
        if (pair == nullptr) break;
        entity->pushToMoveStack(pair);
    }
    // return whether any move was generated
    return entity->areAvailableMoves();
}

bool GameEntityMovement::moveTowardsPoint(Point *point, Graph<Point *> *graph) {
    // using A* algorithm as dijkstra was really memory inefficient (maybe my implementation wasn't good enough)
    std::vector<Point *> *paths = graph->findPathTo(entity->getCircle()->getCenter(), point);
    if (paths == nullptr) return false;
    // calling this function means gameEntity has repositioned
    entity->clearMoveStack();
    // insert all points
    for (auto &pair: *paths) {
        if (pair == nullptr) break;
        entity->pushToMoveStack(pair);
    }
    // return whether any move was generated
    return entity->areAvailableMoves();
}

bool GameEntityMovement::moveBasedOnPoint(Point *point) {
    if (point == nullptr) return false;
    // same row, can go up or down
    if (point->getX() == entity->getCircle()->getCenter()->getX()) {
        // go down
        if (point->getY() > entity->getCircle()->getCenter()->getY())
            return move(MoveDirection::DOWN) == Constants::MoveSuccessValues::SUCCESS;
        // go up
        return move(MoveDirection::UP) == Constants::MoveSuccessValues::SUCCESS;
    } else {
        // same col, can go right or left
        // go right
        if (point->getX() > entity->getCircle()->getCenter()->getX())
            return move(MoveDirection::RIGHT) == Constants::MoveSuccessValues::SUCCESS;
        // go left
        return move(MoveDirection::LEFT) == Constants::MoveSuccessValues::SUCCESS;
    }
}

Constants::MoveSuccessValues GameEntityMovement::move(MoveDirection direction) {
    GameMap *map = currentMap;

    int entitySpeed = entity->getSpeed();
    IntRect entityRect = entity->getRectangle();

    int entityX = entity->getPosition().x;
    int entityY = entity->getPosition().y;

    Constants::MoveSuccessValues moveSuccessValue;
    if (direction == MoveDirection::UP) {
        moveSuccessValue = moveUp(map, entityX, entityY, entitySpeed, entityRect);
        // try and move up, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
        moveSuccessValue == Constants::MoveSuccessValues::CHANGE_UP) {
            entity->setMoveDirection(MoveDirection::UP);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::DOWN) {
        moveSuccessValue = moveDown(map, entityX, entityY, entitySpeed, entityRect);
        // try and move down, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
        moveSuccessValue == Constants::MoveSuccessValues::CHANGE_DOWN) {
            entity->setMoveDirection(MoveDirection::DOWN);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::RIGHT) {
        moveSuccessValue = moveRight(map, entityX, entityY, entitySpeed, entityRect);
        // try and move right, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_RIGHT) {
            entity->setMoveDirection(MoveDirection::RIGHT);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::LEFT) {
        moveSuccessValue = moveLeft(map, entityX, entityY, entitySpeed, entityRect);
        // try and move left, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_LEFT) {
            entity->setMoveDirection(MoveDirection::LEFT);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    }
    return Constants::MoveSuccessValues::FAILURE;
}

Constants::MoveSuccessValues GameEntityMovement::moveUp(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached top of screen
    if (entityY - entitySpeed <= tileSize / 2) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop() && map->getTopExitCircle() != nullptr) {
            // if reached top exit circle of the map
            if (entity->getCircle()->intersects(map->getTopExitCircle())) {
                cout << "Reached Top Exit" << endl;
                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
                return Constants::MoveSuccessValues::CHANGE_UP;
            }
        }
        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // temp circle for moving up
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX(),
                              entity->getCircle()->getCenter()->getY() - entitySpeed,
                              entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (/*map->getLandscapes()[i]->getRectangle().intersects(*rect) ||*/
        map->getLandscapes()[i]->getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }
    // TODO: make for each game map a list of all entities for which we check if there's a collision instead of using the getPlayer method in Game class
    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(circle)) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = circle->intersects(map->getPlayer()->getCircle());
//    canCollide = circle->intersects(Game::getInstance()->getPlayer()->getCircle());
    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX, entityY - entitySpeed);
        entity->setPosition(gameMapsPoints[entityY - entitySpeed][entityX]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveDown(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached bottom of screen
    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom() && map->getBottomExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getBottomExitCircle())) {
                cout << "Reached Bottom Exit" << endl;
                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
                return Constants::MoveSuccessValues::CHANGE_DOWN;
            }
        }
//        entity->setPosition(entityX, screenHeight - tileSize/2);
        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // temp circle for moving down
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX(),
                              entity->getCircle()->getCenter()->getY() + entitySpeed, entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (/*map->getLandscapes()[i]->getRectangle().intersects(*rect) ||*/
            map->getLandscapes()[i]->getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(circle)) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = circle->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX, entityY + entitySpeed);
        entity->setPosition(gameMapsPoints[entityY + entitySpeed][entityX]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveRight(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight() && map->getRightExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getRightExitCircle())) {
                cout << "Reached Right Exit" << endl;
                entity->setPosition(gameMapsPoints[entityY][tileSize]);
                return Constants::MoveSuccessValues::CHANGE_RIGHT;
            }
        }
//        entity->setPosition(screenWidth - tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // temp circle for moving right
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX() + entitySpeed,
                              entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (/*map->getLandscapes()[i]->getRectangle().intersects(*rect) ||*/
            map->getLandscapes()[i]->getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(circle)) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = circle->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX + entitySpeed, entityY);
        entity->setPosition(gameMapsPoints[entityY][entityX + entitySpeed]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveLeft(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached left of screen
    if (entityX - entitySpeed <= tileSize / 2) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft() && map->getLeftExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getLeftExitCircle())) {
                cout << "Reached Left Exit" << endl;
                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
                return Constants::MoveSuccessValues::CHANGE_LEFT;
            }
        }
//        entity->setPosition(tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // temp circle for moving left
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX() - entitySpeed,
                              entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (/*map->getLandscapes()[i]->getRectangle().intersects(*rect) ||*/
            map->getLandscapes()[i]->getCircle()->intersects(circle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(circle)) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = circle->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX - entitySpeed, entityY);
        entity->setPosition(gameMapsPoints[entityY][entityX - entitySpeed]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

//Constants::MoveSuccessValues GameEntityMovement::moveUpRight(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
//    int unreachableAreasSize = map->getLandscapes().size();
//    bool canCollide = false;
//    // reached right of screen
//    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
//        // check if right of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromRight()) {
//            // check if we're at current map's right exit point
//            if (entityY + tileSize / 2 <= map->getRightExitMinY() &&
//                entityY - tileSize / 2 >= map->getRightExitMaxY()) {
//                cout << "Reached Right Exit" << endl;
//                // set current map to the map to the right
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() + 1);
//                currentMap = Game::getInstance()->getCurrentGameMap();
////                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
//                // set entity position to right enter point (changed current map)
////                entity->setPosition(tileSize/2, entityY);
//                entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
//                return true;
//            }
//        }
////        entity->setPosition(screenWidth - tileSize/2, entityY);
//        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
//        return false;
//    }
//
//    // reached top of the screen
//    if (entityY - entitySpeed <= tileSize / 2) {
//        // check if top of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromTop()) {
//            // check if we're at current map's top exit point
//            if (entityX + tileSize / 2 <= map->getTopExitMaxX() && entityX - tileSize / 2 >= map->getTopExitMinX()) {
//                cout << "Reached Top Exit" << endl;
//                // set current map to map above
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() - 1, map->getWorldMapCol());
//                // set entity position to down enter point (changed current map)
////                entity->setPosition(entityX, screenHeight - tileSize/2);
//                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
//                return true;
//            }
//        }
////        entity->setPosition(entityX, tileSize/2);
//        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
//        return false;
//    }
//
//    // temp rectangle for moving right
//    auto *rightRect = new IntRect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
//    // temp rightCircle for moving right
//    auto *rightCircle = new Circle(entity->getCircle()->getCenter()->getX() + entitySpeed,
//                                   entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
//    // temp rectangle for moving up
//    auto *upRect = new IntRect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
//    // temp upCircle for moving up
//    auto *upCircle = new Circle(entity->getCircle()->getCenter()->getX(),
//                                entity->getCircle()->getCenter()->getY() - entitySpeed,
//                                entity->getCircle()->getRadius());
//
//    // check if the move can overlap with any of the unreachable areas
//    for (int i = 0; i < unreachableAreasSize; i++) {
//        if (/*map->getLandscapes()[i]->getRectangle().intersects(*rightRect) ||
//            map->getLandscapes()[i]->getRectangle().intersects(*upRect) ||*/
//                map->getLandscapes()[i]->getCircle()->intersects(rightCircle) ||
//                map->getLandscapes()[i]->getCircle()->intersects(upCircle)) {
//            canCollide = true;
//            break;
//        }
//    }
//
//    if (isPlayer) {
//        // check for collisions with enemies
//        for (int i = 0; i < map->getEnemies().size(); i++) {
//            if (map->getEnemies()[i]->getCircle()->intersects(rightCircle) ||
//                map->getEnemies()[i]->getCircle()->intersects(upCircle)) {
//                canCollide = true;
//                break;
//            }
//        }
//    } else
//        canCollide = rightCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
//                     upCircle->intersects(Game::getInstance()->getPlayer()->getCircle());
//
//    if (!canCollide) {
//        // in legal boundaries
////        entity->setPosition(entityX + entitySpeed/2, entityY - entitySpeed/2);
//        entity->setPosition(gameMapsPoints[entityY - entitySpeed / 2][entityX + entitySpeed / 2]);
//    }
//
//    delete rightRect;
//    delete rightCircle;
//    delete upRect;
//    delete upCircle;
//
//    return !canCollide;
//}

//Constants::MoveSuccessValues GameEntityMovement::moveUpLeft(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
//    int unreachableAreasSize = map->getLandscapes().size();
//    bool canCollide = false;
//    // reached top of screen
//    if (entityY - entitySpeed <= tileSize / 2) {
//        // check if top of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromTop()) {
//            // check if we're at current map's top exit point
//            if (entityX + tileSize / 2 <= map->getTopExitMaxX() && entityX - tileSize / 2 >= map->getTopExitMinX()) {
//                cout << "Reached Top Exit" << endl;
//                // set current map to map above
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() - 1, map->getWorldMapCol());
//                currentMap = Game::getInstance()->getCurrentGameMap();
//                // set entity position to down enter point (changed current map)
////                entity->setPosition(entityX, screenHeight - tileSize/2);
//                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
//                return true;
//            }
//        }
////        entity->setPosition(entityX, tileSize/2);
//        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
//        return false;
//    }
//
//    // reached left of screen
//    if (entityX - entitySpeed <= tileSize / 2) {
//        // check if left of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromLeft()) {
//            // check if we're at current map's left exit point
//            if (entityY + tileSize / 2 <= map->getLeftExitMinY() && entityY - tileSize / 2 >= map->getLeftExitMaxY()) {
//                cout << "Reached Left Exit" << endl;
//                // set current map to the map to the left
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() - 1);
//                currentMap = Game::getInstance()->getCurrentGameMap();
////                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
//                // set entity position to left enter point (changed current map)
////                entity->setPosition(screenWidth - tileSize/2, entityY);
//                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
//                return true;
//            }
//        }
////        entity->setPosition(tileSize/2, entityY);
//        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
//        return false;
//    }
//    // temp rectangle for moving left
//    auto *leftRect = new IntRect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
//    // temp leftCircle for moving left
//    auto *leftCircle = new Circle(entity->getCircle()->getCenter()->getX() - entitySpeed,
//                                  entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
//    // temp rectangle for moving up
//    auto *upRect = new IntRect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
//    // temp upCircle for moving up
//    auto *upCircle = new Circle(entity->getCircle()->getCenter()->getX(),
//                                entity->getCircle()->getCenter()->getY() - entitySpeed,
//                                entity->getCircle()->getRadius());
//
//    // check if the move can overlap with any of the unreachable areas
//    for (int i = 0; i < unreachableAreasSize; i++) {
//        if (/*map->getLandscapes()[i]->getRectangle().intersects(*leftRect) ||
//            map->getLandscapes()[i]->getRectangle().intersects(*upRect) ||*/
//                map->getLandscapes()[i]->getCircle()->intersects(leftCircle) ||
//                map->getLandscapes()[i]->getCircle()->intersects(upCircle)) {
//            canCollide = true;
//            break;
//        }
//    }
//
//    if (isPlayer) {
//        // check for collisions with enemies
//        for (int i = 0; i < map->getEnemies().size(); i++) {
//            if (map->getEnemies()[i]->getCircle()->intersects(leftCircle) ||
//                map->getEnemies()[i]->getCircle()->intersects(upCircle)) {
//                canCollide = true;
//                break;
//            }
//        }
//    } else
//        canCollide = leftCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
//                     upCircle->intersects(Game::getInstance()->getPlayer()->getCircle());
//
//    if (!canCollide) {
//        // in legal boundaries
////        entity->setPosition(entityX - entitySpeed/2, entityY - entitySpeed/2);
//        entity->setPosition(gameMapsPoints[entityY - entitySpeed / 2][entityX - entitySpeed / 2]);
//    }
//
//    delete leftRect;
//    delete leftCircle;
//    delete upRect;
//    delete upCircle;
//
//    return !canCollide;
//}

//Constants::MoveSuccessValues GameEntityMovement::moveDownRight(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
//    int unreachableAreasSize = map->getLandscapes().size();
//    bool canCollide = false;
//    // reached bottom of screen
//    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
//        // check if bottom of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromBottom()) {
//            // check if we're at current map's top exit point
//            if (entityX + tileSize / 2 <= map->getBottomExitMaxX() &&
//                entityX - tileSize / 2 >= map->getBottomExitMinX()) {
//                cout << "Reached Bottom Exit" << endl;
//                // set current map to map below
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() + 1, map->getWorldMapCol());
//                currentMap = Game::getInstance()->getCurrentGameMap();
////                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
//                // set entity position to top enter point (changed current map)
////                entity->setPosition(entityX, tileSize/2);
//                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
//                return true;
//            }
//        }
////        entity->setPosition(entityX, screenHeight - tileSize/2);
//        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
//        return false;
//    }
//
//    // reached right of screen
//    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
//        // check if right of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromRight()) {
//            // check if we're at current map's right exit point
//            if (entityY + tileSize / 2 <= map->getRightExitMinY() &&
//                entityY - tileSize / 2 >= map->getRightExitMaxY()) {
//                cout << "Reached Right Exit" << endl;
//                // set current map to the map to the right
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() + 1);
//                currentMap = Game::getInstance()->getCurrentGameMap();
////                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
//                // set entity position to right enter point (changed current map)
////                entity->setPosition(tileSize/2, entityY);
//                entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
//                return true;
//            }
//        }
////        entity->setPosition(screenWidth - tileSize/2, entityY);
//        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
//        return false;
//    }
//
//    // temp rectangle for moving down
//    auto *downRect = new IntRect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
//    // temp circle for moving down
//    auto *downCircle = new Circle(entity->getCircle()->getCenter()->getX(),
//                                  entity->getCircle()->getCenter()->getY() + entitySpeed,
//                                  entity->getCircle()->getRadius());
//    // temp rectangle for moving right
//    auto *rightRect = new IntRect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
//    // temp rightCircle for moving right
//    auto *rightCircle = new Circle(entity->getCircle()->getCenter()->getX() + entitySpeed,
//                                   entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
//
//    // check if the move can overlap with any of the unreachable areas
//    for (int i = 0; i < unreachableAreasSize; i++) {
//        if (/*map->getLandscapes()[i]->getRectangle().intersects(*downRect) ||
//            map->getLandscapes()[i]->getRectangle().intersects(*rightRect) ||*/
//                map->getLandscapes()[i]->getCircle()->intersects(rightCircle) ||
//                map->getLandscapes()[i]->getCircle()->intersects(downCircle)) {
//            canCollide = true;
//            break;
//        }
//    }
//
//    if (isPlayer) {
//        // check for collisions with enemies
//        for (int i = 0; i < map->getEnemies().size(); i++) {
//            if (map->getEnemies()[i]->getCircle()->intersects(downCircle) ||
//                map->getEnemies()[i]->getCircle()->intersects(rightCircle)) {
//                canCollide = true;
//                break;
//            }
//        }
//    } else
//        canCollide = downCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
//                     rightCircle->intersects(Game::getInstance()->getPlayer()->getCircle());
//
//    if (!canCollide) {
//        // in legal boundaries
////        entity->setPosition(entityX + entitySpeed/2, entityY + entitySpeed/2);
//        entity->setPosition(gameMapsPoints[entityY + entitySpeed / 2][entityX + entitySpeed / 2]);
//    }
//
//    delete rightRect;
//    delete rightCircle;
//    delete downRect;
//    delete downCircle;
//
//    return !canCollide;
//}

//Constants::MoveSuccessValues GameEntityMovement::moveDownLeft(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
//    int unreachableAreasSize = map->getLandscapes().size();
//    bool canCollide = false;
//    // reached bottom of screen
//    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
//        // check if bottom of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromBottom()) {
//            // check if we're at current map's top exit point
//            if (entityX + tileSize / 2 <= map->getBottomExitMaxX() &&
//                entityX - tileSize / 2 >= map->getBottomExitMinX()) {
//                cout << "Reached Bottom Exit" << endl;
//                // set current map to map below
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() + 1, map->getWorldMapCol());
//                currentMap = Game::getInstance()->getCurrentGameMap();
////                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
//                // set entity position to top enter point (changed current map)
////                entity->setPosition(entityX, tileSize/2);
//                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
//                return true;
//            }
//        }
////        entity->setPosition(entityX, screenHeight - tileSize/2);
//        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
//        return false;
//    }
//
//    // reached left of screen
//    if (entityX - entitySpeed <= tileSize / 2) {
//        // check if left of screen is an exit point for current game map
//        if (isPlayer && map->isExitableFromLeft()) {
//            // check if we're at current map's left exit point
//            if (entityY + tileSize / 2 <= map->getLeftExitMinY() && entityY - tileSize / 2 >= map->getLeftExitMaxY()) {
//                cout << "Reached Left Exit" << endl;
//                // set current map to the map to the left
//                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() - 1);
//                currentMap = Game::getInstance()->getCurrentGameMap();
////                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
//                // set entity position to left enter point (changed current map)
////                entity->setPosition(screenWidth - tileSize/2, entityY);
//                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
//                return true;
//            }
//        }
////        entity->setPosition(tileSize/2, entityY);
//        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
//        return false;
//    }
//
//    // temp rectangle for moving down
//    auto *downRect = new IntRect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
//    // temp circle for moving down
//    auto *downCircle = new Circle(entity->getCircle()->getCenter()->getX(),
//                                  entity->getCircle()->getCenter()->getY() + entitySpeed,
//                                  entity->getCircle()->getRadius());
//    // temp rectangle for moving left
//    auto *leftRect = new IntRect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
//    // temp leftCircle for moving left
//    auto *leftCircle = new Circle(entity->getCircle()->getCenter()->getX() - entitySpeed,
//                                  entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
//
//    // check if the move can overlap with any of the unreachable areas
//    for (int i = 0; i < unreachableAreasSize; i++) {
//        if (/*map->getLandscapes()[i]->getRectangle().intersects(*downRect) ||
//            map->getLandscapes()[i]->getRectangle().intersects(*leftRect) ||*/
//                map->getLandscapes()[i]->getCircle()->intersects(downCircle) ||
//                map->getLandscapes()[i]->getCircle()->intersects(leftCircle)) {
//            canCollide = true;
//            break;
//        }
//    }
//
//    if (isPlayer) {
//        // check for collisions with enemies
//        for (int i = 0; i < map->getEnemies().size(); i++) {
//            if (map->getEnemies()[i]->getCircle()->intersects(downCircle) ||
//                map->getEnemies()[i]->getCircle()->intersects(leftCircle)) {
//                canCollide = true;
//                break;
//            }
//        }
//        dynamic_cast<Player *>(entity)->notifyAll();
////        ((Player *) entity)->notifyAll();
//    } else
//        canCollide = downCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
//                     leftCircle->intersects(Game::getInstance()->getPlayer()->getCircle());
//
//    if (!canCollide) {
//        // in legal boundaries
////        entity->setPosition(entityX - entitySpeed/2, entityY + entitySpeed/2);
//        entity->setPosition(gameMapsPoints[entityY + entitySpeed / 2][entityX - entitySpeed / 2]);
//    }
//
//    delete leftRect;
//    delete leftCircle;
//    delete downRect;
//    delete downCircle;
//
//    return !canCollide;
//}

bool GameEntityMovement::moveRandomly(int randomDirection) {
    // DOWN, RIGHT, LEFT, UP
    switch (randomDirection) {
        case 0:
            return move(MoveDirection::DOWN) == Constants::MoveSuccessValues::SUCCESS;
        case 1:
            return move(MoveDirection::RIGHT) == Constants::MoveSuccessValues::SUCCESS;
        case 2:
            return move(MoveDirection::LEFT) == Constants::MoveSuccessValues::SUCCESS;
        case 3:
            return move(MoveDirection::UP) == Constants::MoveSuccessValues::SUCCESS;
        default:
            // TODO: add error handling
            break;
    }
    return false;
}

void GameEntityMovement::setEntity(GameEntity &e) {
    this->entity = &e;
}

GameEntity *GameEntityMovement::getEntity() {
    return entity;
}

void GameEntityMovement::setCurrentMap(GameMap *map) {
    currentMap = map;
}

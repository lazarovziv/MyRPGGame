#include "../include/GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, GameMap *map) {
    this->entity = entity;
    currentMap = map;
    isPlayer = player;
    screenWidth = Constants::SCREEN_WIDTH;
    screenHeight = Constants::SCREEN_HEIGHT;
    tileSize = Constants::TILE_SIZE;
}

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, GameMap *map, Point ***points) {
    this->entity = entity;
    currentMap = map;
    isPlayer = player;
    screenWidth = Constants::SCREEN_WIDTH;
    screenHeight = Constants::SCREEN_HEIGHT;
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
        if (point->getY() > entity->getCircle()->getCenter()->getY()) return move(MoveDirection::DOWN);
        // go up
        return move(MoveDirection::UP);
    } else {
        // same col, can go right or left
        // go right
        if (point->getX() > entity->getCircle()->getCenter()->getX()) return move(MoveDirection::RIGHT);
        // go left
        return move(MoveDirection::LEFT);
    }
}

bool GameEntityMovement::move(MoveDirection direction) {
    GameMap *map = Game::getInstance()->getCurrentGameMap();

    int entitySpeed = entity->getSpeed();
    IntRect entityRect = entity->getRectangle();

    int entityX = entity->getPosition().x;
    int entityY = entity->getPosition().y;

    if (direction == MoveDirection::UP) {
        // try and move up, if false, collided with something
        if (moveUp(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::UP);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    } else if (direction == MoveDirection::DOWN) {
        // try and move down, if false, collided with something
        if (moveDown(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::DOWN);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    } else if (direction == MoveDirection::RIGHT) {
        // try and move right, if false, collided with something
        if (moveRight(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::RIGHT);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    } else if (direction == MoveDirection::LEFT) {
        // try and move left, if false, collided with something
        if (moveLeft(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::LEFT);
            entity->incrementStep();
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[direction] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
        // TODO: add other diagonal functions
    } else if (direction == MoveDirection::UP_RIGHT) {
        if (moveUpRight(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::UP_RIGHT);
            entity->incrementStep();
            // TODO: add diagonal sprites
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[MoveDirection::UP] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    } else if (direction == MoveDirection::UP_LEFT) {
        if (moveUpLeft(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::UP_LEFT);
            entity->incrementStep();
            // TODO: add diagonal sprites
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[MoveDirection::UP] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    } else if (direction == MoveDirection::DOWN_RIGHT) {
        if (moveDownRight(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::DOWN_RIGHT);
            entity->incrementStep();
            // TODO: add diagonal sprites
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[MoveDirection::DOWN] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    } else if (direction == MoveDirection::DOWN_LEFT) {
        if (moveDownLeft(map, entityX, entityY, entitySpeed, entityRect)) {
            entity->setMoveDirection(MoveDirection::DOWN_LEFT);
            entity->incrementStep();
            // TODO: add diagonal sprites
            entity->getSprite()->setTextureRect(sf::IntRect(
                    entity->getStep() * Constants::TILE_SIZE,
                    entity->getMoveDirectionsSpritesMap()[MoveDirection::DOWN] * Constants::TILE_SIZE,
                    Constants::TILE_SIZE, Constants::TILE_SIZE));
            return true;
        } else return false;
    }
    return false;
}

bool GameEntityMovement::moveUp(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached top of screen
    if (entityY - entitySpeed <= tileSize / 2) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize / 2 <= map->getTopExitMaxX() && entityX - tileSize / 2 >= map->getTopExitMinX()) {
                cout << "Reached Top Exit" << endl;
                // set current map to map above
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() - 1, map->getWorldMapCol());
                currentMap = Game::getInstance()->getCurrentGameMap();
                // set entity position to down enter point (changed current map)
//                entity->setPosition(entityX, screenHeight - tileSize/2);
                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
                return true;
            }
        }
//        entity->setPosition(entityX, tileSize/2);
        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
        return false;
    }
    // temp rectangle for moving up
    auto *rect = new IntRect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
    // temp circle for moving up
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX(),
                              entity->getCircle()->getCenter()->getY() - entitySpeed,
                              entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*rect) ||
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
    } else canCollide = circle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX, entityY - entitySpeed);
        entity->setPosition(gameMapsPoints[entityY - entitySpeed][entityX]);
    }

    delete rect;
    delete circle;

    return !canCollide;
}

bool GameEntityMovement::moveDown(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached bottom of screen
    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize / 2 <= map->getBottomExitMaxX() &&
                entityX - tileSize / 2 >= map->getBottomExitMinX()) {
                cout << "Reached Bottom Exit" << endl;
                // set current map to map below
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() + 1, map->getWorldMapCol());
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
                // set entity position to top enter point (changed current map)
//                entity->setPosition(entityX, tileSize/2);
                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
                return true;
            }
        }
//        entity->setPosition(entityX, screenHeight - tileSize/2);
        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
        return false;
    }
    // temp rectangle for moving down
    auto *rect = new IntRect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
    // temp circle for moving down
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX(),
                              entity->getCircle()->getCenter()->getY() + entitySpeed, entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*rect) ||
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
    } else canCollide = circle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX, entityY + entitySpeed);
        entity->setPosition(gameMapsPoints[entityY + entitySpeed][entityX]);
    }

    delete rect;
    delete circle;

    return !canCollide;
}

bool GameEntityMovement::moveRight(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight()) {
            // check if we're at current map's right exit point
            if (entityY + tileSize / 2 <= map->getRightExitMinY() &&
                entityY - tileSize / 2 >= map->getRightExitMaxY()) {
                cout << "Reached Right Exit" << endl;
                // set current map to the map to the right
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() + 1);
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
                // set entity position to right enter point (changed current map)
//                entity->setPosition(tileSize/2, entityY);
                entity->setPosition(gameMapsPoints[entityY][tileSize]);
                return true;
            }
        }
//        entity->setPosition(screenWidth - tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
        return false;
    }
    // temp rectangle for moving right
    auto *rect = new IntRect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp circle for moving right
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX() + entitySpeed,
                              entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*rect) ||
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
    } else canCollide = circle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX + entitySpeed, entityY);
        entity->setPosition(gameMapsPoints[entityY][entityX + entitySpeed]);
    }

    delete rect;
    delete circle;

    return !canCollide;
}

bool GameEntityMovement::moveLeft(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached left of screen
    if (entityX - entitySpeed <= tileSize / 2) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft()) {
            // check if we're at current map's left exit point
            if (entityY + tileSize / 2 <= map->getLeftExitMinY() && entityY - tileSize / 2 >= map->getLeftExitMaxY()) {
                cout << "Reached Left Exit" << endl;
                // set current map to the map to the left
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() - 1);
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
                // set entity position to left enter point (changed current map)
//                entity->setPosition(screenWidth - tileSize/2, entityY);
                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
                return true;
            }
        }
//        entity->setPosition(tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
        return false;
    }
    // temp rectangle for moving left
    auto *rect = new IntRect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp circle for moving left
    auto *circle = new Circle(entity->getCircle()->getCenter()->getX() - entitySpeed,
                              entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*rect) ||
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
    } else canCollide = circle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX - entitySpeed, entityY);
        entity->setPosition(gameMapsPoints[entityY][entityX - entitySpeed]);
    }

    delete rect;
    delete circle;

    return !canCollide;
}

bool GameEntityMovement::moveUpRight(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight()) {
            // check if we're at current map's right exit point
            if (entityY + tileSize / 2 <= map->getRightExitMinY() &&
                entityY - tileSize / 2 >= map->getRightExitMaxY()) {
                cout << "Reached Right Exit" << endl;
                // set current map to the map to the right
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() + 1);
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
                // set entity position to right enter point (changed current map)
//                entity->setPosition(tileSize/2, entityY);
                entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
                return true;
            }
        }
//        entity->setPosition(screenWidth - tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
        return false;
    }

    // reached top of the screen
    if (entityY - entitySpeed <= tileSize / 2) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize / 2 <= map->getTopExitMaxX() && entityX - tileSize / 2 >= map->getTopExitMinX()) {
                cout << "Reached Top Exit" << endl;
                // set current map to map above
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() - 1, map->getWorldMapCol());
                // set entity position to down enter point (changed current map)
//                entity->setPosition(entityX, screenHeight - tileSize/2);
                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
                return true;
            }
        }
//        entity->setPosition(entityX, tileSize/2);
        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
        return false;
    }

    // temp rectangle for moving right
    auto *rightRect = new IntRect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp rightCircle for moving right
    auto *rightCircle = new Circle(entity->getCircle()->getCenter()->getX() + entitySpeed,
                                   entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // temp rectangle for moving up
    auto *upRect = new IntRect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
    // temp upCircle for moving up
    auto *upCircle = new Circle(entity->getCircle()->getCenter()->getX(),
                                entity->getCircle()->getCenter()->getY() - entitySpeed,
                                entity->getCircle()->getRadius());

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*rightRect) ||
            map->getLandscapes()[i]->getRectangle().intersects(*upRect) ||
            map->getLandscapes()[i]->getCircle()->intersects(rightCircle) ||
            map->getLandscapes()[i]->getCircle()->intersects(upCircle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(rightCircle) ||
                map->getEnemies()[i]->getCircle()->intersects(upCircle)) {
                canCollide = true;
                break;
            }
        }
    } else
        canCollide = rightCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
                     upCircle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX + entitySpeed/2, entityY - entitySpeed/2);
        entity->setPosition(gameMapsPoints[entityY - entitySpeed / 2][entityX + entitySpeed / 2]);
    }

    delete rightRect;
    delete rightCircle;
    delete upRect;
    delete upCircle;

    return !canCollide;
}

bool GameEntityMovement::moveUpLeft(GameMap *map, int entityX, int entityY, int entitySpeed, IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached top of screen
    if (entityY - entitySpeed <= tileSize / 2) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize / 2 <= map->getTopExitMaxX() && entityX - tileSize / 2 >= map->getTopExitMinX()) {
                cout << "Reached Top Exit" << endl;
                // set current map to map above
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() - 1, map->getWorldMapCol());
                currentMap = Game::getInstance()->getCurrentGameMap();
                // set entity position to down enter point (changed current map)
//                entity->setPosition(entityX, screenHeight - tileSize/2);
                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
                return true;
            }
        }
//        entity->setPosition(entityX, tileSize/2);
        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
        return false;
    }

    // reached left of screen
    if (entityX - entitySpeed <= tileSize / 2) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft()) {
            // check if we're at current map's left exit point
            if (entityY + tileSize / 2 <= map->getLeftExitMinY() && entityY - tileSize / 2 >= map->getLeftExitMaxY()) {
                cout << "Reached Left Exit" << endl;
                // set current map to the map to the left
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() - 1);
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
                // set entity position to left enter point (changed current map)
//                entity->setPosition(screenWidth - tileSize/2, entityY);
                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
                return true;
            }
        }
//        entity->setPosition(tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
        return false;
    }
    // temp rectangle for moving left
    auto *leftRect = new IntRect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp leftCircle for moving left
    auto *leftCircle = new Circle(entity->getCircle()->getCenter()->getX() - entitySpeed,
                                  entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());
    // temp rectangle for moving up
    auto *upRect = new IntRect(entityRect.left, entityRect.top - entitySpeed, entityRect.width, entityRect.height);
    // temp upCircle for moving up
    auto *upCircle = new Circle(entity->getCircle()->getCenter()->getX(),
                                entity->getCircle()->getCenter()->getY() - entitySpeed,
                                entity->getCircle()->getRadius());

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*leftRect) ||
            map->getLandscapes()[i]->getRectangle().intersects(*upRect) ||
            map->getLandscapes()[i]->getCircle()->intersects(leftCircle) ||
            map->getLandscapes()[i]->getCircle()->intersects(upCircle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(leftCircle) ||
                map->getEnemies()[i]->getCircle()->intersects(upCircle)) {
                canCollide = true;
                break;
            }
        }
    } else
        canCollide = leftCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
                     upCircle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX - entitySpeed/2, entityY - entitySpeed/2);
        entity->setPosition(gameMapsPoints[entityY - entitySpeed / 2][entityX - entitySpeed / 2]);
    }

    delete leftRect;
    delete leftCircle;
    delete upRect;
    delete upCircle;

    return !canCollide;
}

bool GameEntityMovement::moveDownRight(GameMap *map, int entityX, int entityY, int entitySpeed,
                                       IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached bottom of screen
    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize / 2 <= map->getBottomExitMaxX() &&
                entityX - tileSize / 2 >= map->getBottomExitMinX()) {
                cout << "Reached Bottom Exit" << endl;
                // set current map to map below
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() + 1, map->getWorldMapCol());
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
                // set entity position to top enter point (changed current map)
//                entity->setPosition(entityX, tileSize/2);
                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
                return true;
            }
        }
//        entity->setPosition(entityX, screenHeight - tileSize/2);
        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
        return false;
    }

    // reached right of screen
    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight()) {
            // check if we're at current map's right exit point
            if (entityY + tileSize / 2 <= map->getRightExitMinY() &&
                entityY - tileSize / 2 >= map->getRightExitMaxY()) {
                cout << "Reached Right Exit" << endl;
                // set current map to the map to the right
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() + 1);
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() + 1);
                // set entity position to right enter point (changed current map)
//                entity->setPosition(tileSize/2, entityY);
                entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
                return true;
            }
        }
//        entity->setPosition(screenWidth - tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
        return false;
    }

    // temp rectangle for moving down
    auto *downRect = new IntRect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
    // temp circle for moving down
    auto *downCircle = new Circle(entity->getCircle()->getCenter()->getX(),
                                  entity->getCircle()->getCenter()->getY() + entitySpeed,
                                  entity->getCircle()->getRadius());
    // temp rectangle for moving right
    auto *rightRect = new IntRect(entityRect.left + entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp rightCircle for moving right
    auto *rightCircle = new Circle(entity->getCircle()->getCenter()->getX() + entitySpeed,
                                   entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*downRect) ||
            map->getLandscapes()[i]->getRectangle().intersects(*rightRect) ||
            map->getLandscapes()[i]->getCircle()->intersects(rightCircle) ||
            map->getLandscapes()[i]->getCircle()->intersects(downCircle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(downCircle) ||
                map->getEnemies()[i]->getCircle()->intersects(rightCircle)) {
                canCollide = true;
                break;
            }
        }
    } else
        canCollide = downCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
                     rightCircle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX + entitySpeed/2, entityY + entitySpeed/2);
        entity->setPosition(gameMapsPoints[entityY + entitySpeed / 2][entityX + entitySpeed / 2]);
    }

    delete rightRect;
    delete rightCircle;
    delete downRect;
    delete downCircle;

    return !canCollide;
}

bool GameEntityMovement::moveDownLeft(GameMap *map, int entityX, int entityY, int entitySpeed,
                                      IntRect &entityRect) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached bottom of screen
    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom()) {
            // check if we're at current map's top exit point
            if (entityX + tileSize / 2 <= map->getBottomExitMaxX() &&
                entityX - tileSize / 2 >= map->getBottomExitMinX()) {
                cout << "Reached Bottom Exit" << endl;
                // set current map to map below
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow() + 1, map->getWorldMapCol());
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapRow() + 1);
                // set entity position to top enter point (changed current map)
//                entity->setPosition(entityX, tileSize/2);
                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
                return true;
            }
        }
//        entity->setPosition(entityX, screenHeight - tileSize/2);
        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
        return false;
    }

    // reached left of screen
    if (entityX - entitySpeed <= tileSize / 2) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft()) {
            // check if we're at current map's left exit point
            if (entityY + tileSize / 2 <= map->getLeftExitMinY() && entityY - tileSize / 2 >= map->getLeftExitMaxY()) {
                cout << "Reached Left Exit" << endl;
                // set current map to the map to the left
                Game::getInstance()->changeCurrentMap(map->getWorldMapRow(), map->getWorldMapCol() - 1);
                currentMap = Game::getInstance()->getCurrentGameMap();
//                Game::getInstance()->setCurrentWorldMapRow(map->getWorldMapCol() - 1);
                // set entity position to left enter point (changed current map)
//                entity->setPosition(screenWidth - tileSize/2, entityY);
                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
                return true;
            }
        }
//        entity->setPosition(tileSize/2, entityY);
        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
        return false;
    }

    // temp rectangle for moving down
    auto *downRect = new IntRect(entityRect.left, entityRect.top + entitySpeed, entityRect.width, entityRect.height);
    // temp circle for moving down
    auto *downCircle = new Circle(entity->getCircle()->getCenter()->getX(),
                                  entity->getCircle()->getCenter()->getY() + entitySpeed,
                                  entity->getCircle()->getRadius());
    // temp rectangle for moving left
    auto *leftRect = new IntRect(entityRect.left - entitySpeed, entityRect.top, entityRect.width, entityRect.height);
    // temp leftCircle for moving left
    auto *leftCircle = new Circle(entity->getCircle()->getCenter()->getX() - entitySpeed,
                                  entity->getCircle()->getCenter()->getY(), entity->getCircle()->getRadius());

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getRectangle().intersects(*downRect) ||
            map->getLandscapes()[i]->getRectangle().intersects(*leftRect) ||
            map->getLandscapes()[i]->getCircle()->intersects(downCircle) ||
            map->getLandscapes()[i]->getCircle()->intersects(leftCircle)) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(downCircle) ||
                map->getEnemies()[i]->getCircle()->intersects(leftCircle)) {
                canCollide = true;
                break;
            }
        }
        dynamic_cast<Player *>(entity)->notifyAll();
//        ((Player *) entity)->notifyAll();
    } else
        canCollide = downCircle->intersects(Game::getInstance()->getPlayer()->getCircle()) ||
                     leftCircle->intersects(Game::getInstance()->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
//        entity->setPosition(entityX - entitySpeed/2, entityY + entitySpeed/2);
        entity->setPosition(gameMapsPoints[entityY + entitySpeed / 2][entityX - entitySpeed / 2]);
    }

    delete leftRect;
    delete leftCircle;
    delete downRect;
    delete downCircle;

    return !canCollide;
}

bool GameEntityMovement::moveRandomly(int randomDirection) {
    // TODO: handle errors where NPC can go out of map
    // DOWN, RIGHT, LEFT, UP
    switch (randomDirection) {
        case 0:
            return move(MoveDirection::DOWN);
            break;
        case 1:
            return move(MoveDirection::RIGHT);
            break;
        case 2:
            return move(MoveDirection::LEFT);
            break;
        case 3:
            return move(MoveDirection::UP);
            break;
            /*
        case 4:
            enemiesMovement->move(MoveDirection::DOWN_RIGHT);
            break;
        case 5:
            enemiesMovement->move(MoveDirection::DOWN_LEFT);
            break;
        case 6:
            enemiesMovement->move(MoveDirection::UP_RIGHT);
            break;
        case 7:
            enemiesMovement->move(MoveDirection::UP_LEFT);
            break;
             */
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

#include "../include/GameEntityMovement.hpp"

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, GameMap *map) {
    this->entity = entity;
    currentMap = map;
    isPlayer = player;
    screenWidth = Constants::FULL_SCREEN_WIDTH;
    screenHeight = Constants::FULL_SCREEN_HEIGHT;
    tileSize = Constants::TILE_SIZE;

    animationManager = new AnimationManager(this->entity);
}

GameEntityMovement::GameEntityMovement(GameEntity *entity, bool player, GameMap *map, Point ***points) {
    this->entity = entity;
    currentMap = map;
    isPlayer = player;
    screenWidth = Constants::FULL_SCREEN_WIDTH;
    screenHeight = Constants::FULL_SCREEN_HEIGHT;
    tileSize = Constants::TILE_SIZE;
    gameMapsPoints = points;

    animationManager = new AnimationManager(this->entity);
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
            return move(MoveDirection::DOWN, EntityMovementState::RUN) == Constants::MoveSuccessValues::SUCCESS;
        // go up
        return move(MoveDirection::UP, EntityMovementState::RUN) == Constants::MoveSuccessValues::SUCCESS;
    } else {
        // same col, can go right or left
        // go right
        if (point->getX() > entity->getCircle()->getCenter()->getX())
            return move(MoveDirection::RIGHT, EntityMovementState::RUN) == Constants::MoveSuccessValues::SUCCESS;
        // go left
        return move(MoveDirection::LEFT, EntityMovementState::RUN) == Constants::MoveSuccessValues::SUCCESS;
    }
}

Constants::MoveSuccessValues GameEntityMovement::move(MoveDirection direction, EntityMovementState movementState) {
    GameMap *map = currentMap;

    float entitySpeed = entity->getSpeed();

    float entityX = entity->getPosition().x;
    float entityY = entity->getPosition().y;

    int directionRow = entity->getMoveDirectionsSpritesMap()[direction];

    Constants::MoveSuccessValues moveSuccessValue;
    if (movementState == EntityMovementState::IDLE) {
        animationManager->incrementCount(movementState);
        entity->setIntRectPosition(animationManager->getMovementStateCount(movementState) * Constants::TILE_SIZE,
                                   (entity->getMovementStateRowMap()[movementState] + directionRow-1) * Constants::TILE_SIZE,
                                   Constants::TILE_SIZE, Constants::TILE_SIZE);
        return Constants::MoveSuccessValues::NOT_MOVED;
    }
    if (direction == MoveDirection::UP) {
        moveSuccessValue = moveUp(map, entityX, entityY, entitySpeed);
        // try and move up, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
        moveSuccessValue == Constants::MoveSuccessValues::CHANGE_UP) {
            entity->setMoveDirection(MoveDirection::UP);
//            animationManager->incrementCount(EntityMovementState::WALK);
            animationManager->incrementCount(movementState);
//            animationManager->incrementWalkCount();
//            entity->incrementStep();
            //                    entity->getStep() * Constants::TILE_SIZE,
            entity->setIntRectPosition(animationManager->getMovementStateCount(movementState) * Constants::TILE_SIZE,
                                       (entity->getMovementStateRowMap()[movementState] + directionRow-1) * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE);
//            animationManager->animate(EntityMovementState::WALK);
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::DOWN) {
        moveSuccessValue = moveDown(map, entityX, entityY, entitySpeed);
        // try and move down, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
        moveSuccessValue == Constants::MoveSuccessValues::CHANGE_DOWN) {
            entity->setMoveDirection(MoveDirection::DOWN);
            //            animationManager->incrementCount(EntityMovementState::WALK);
            animationManager->incrementCount(movementState);
//            animationManager->incrementWalkCount();
//            entity->incrementStep();
            //                    entity->getStep() * Constants::TILE_SIZE,
            entity->setIntRectPosition(animationManager->getMovementStateCount(movementState) * Constants::TILE_SIZE,
                                       (entity->getMovementStateRowMap()[movementState] + directionRow-1) * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE);
//            animationManager->animate(EntityMovementState::WALK);
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::RIGHT) {
        moveSuccessValue = moveRight(map, entityX, entityY, entitySpeed);
        // try and move right, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_RIGHT) {
            entity->setMoveDirection(MoveDirection::RIGHT);
            //            animationManager->incrementCount(EntityMovementState::WALK);
            animationManager->incrementCount(movementState);
//            animationManager->incrementWalkCount();
//            entity->incrementStep();
            //                    entity->getStep() * Constants::TILE_SIZE,
            entity->setIntRectPosition(animationManager->getMovementStateCount(movementState) * Constants::TILE_SIZE,
                                       (entity->getMovementStateRowMap()[movementState] + directionRow-1) * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE);
//            animationManager->animate(EntityMovementState::WALK);
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::LEFT) {
        moveSuccessValue = moveLeft(map, entityX, entityY, entitySpeed);
        // try and move left, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_LEFT) {
            entity->setMoveDirection(MoveDirection::LEFT);
            //            animationManager->incrementCount(EntityMovementState::WALK);
            animationManager->incrementCount(movementState);
//            animationManager->incrementWalkCount();
//            entity->incrementStep();
            //                    entity->getStep() * Constants::TILE_SIZE,
            entity->setIntRectPosition(animationManager->getMovementStateCount(movementState) * Constants::TILE_SIZE,
                                       (entity->getMovementStateRowMap()[movementState] + directionRow-1) * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE);
//            animationManager->animate(EntityMovementState::WALK);
            return moveSuccessValue;
        } else return Constants::MoveSuccessValues::FAILURE;
    }
    return Constants::MoveSuccessValues::FAILURE;
}

Constants::MoveSuccessValues GameEntityMovement::moveUp(GameMap *map, float entityX, float entityY, float entitySpeed) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached top of screen
    if (entityY - entitySpeed <= tileSize / 2) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop() && map->getTopExitCircle() != nullptr) {
            // if reached top exit circle of the map
            if (entity->getCircle()->intersects(map->getTopExitCircle())) {
                cout << "Reached Top Exit" << endl;
                entity->setPosition(entityX, (screenHeight - tileSize / 2));
//                entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
                return Constants::MoveSuccessValues::CHANGE_UP;
            }
        }
        entity->setPosition(entityX, tileSize/2);
//        entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
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
        entity->setPosition(entityX, (entityY - entitySpeed));
//        entity->setPosition(gameMapsPoints[entityY - entitySpeed][entityX]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveDown(GameMap *map, float entityX, float entityY, float entitySpeed) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached bottom of screen
    if (entityY + entitySpeed >= screenHeight - tileSize / 2) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom() && map->getBottomExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getBottomExitCircle())) {
                cout << "Reached Bottom Exit" << endl;
                entity->setPosition(entityX, (tileSize) / 2);
//                entity->setPosition(gameMapsPoints[tileSize / 2][entityX]);
                return Constants::MoveSuccessValues::CHANGE_DOWN;
            }
        }
        entity->setPosition(entityX, (screenHeight - tileSize/2));
//        entity->setPosition(gameMapsPoints[screenHeight - tileSize / 2][entityX]);
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
        entity->setPosition(entityX, (entityY + entitySpeed));
//        entity->setPosition(gameMapsPoints[entityY + entitySpeed][entityX]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveRight(GameMap *map, float entityX, float entityY, float entitySpeed) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (entityX + entitySpeed >= screenWidth - tileSize / 2) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight() && map->getRightExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getRightExitCircle())) {
                cout << "Reached Right Exit" << endl;
                entity->setPosition(tileSize, entityY);
//                entity->setPosition(gameMapsPoints[entityY][tileSize]);
                return Constants::MoveSuccessValues::CHANGE_RIGHT;
            }
        }
        entity->setPosition((screenWidth - tileSize/2), entityY);
//        entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
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
        entity->setPosition((entityX + entitySpeed), entityY);
//        entity->setPosition(gameMapsPoints[entityY][entityX + entitySpeed]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveLeft(GameMap *map, float entityX, float entityY, float entitySpeed) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached left of screen
    if (entityX - entitySpeed <= tileSize / 2) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft() && map->getLeftExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getLeftExitCircle())) {
                cout << "Reached Left Exit" << endl;
                entity->setPosition((screenWidth - tileSize / 2), entityY);
//                entity->setPosition(gameMapsPoints[entityY][screenWidth - tileSize / 2]);
                return Constants::MoveSuccessValues::CHANGE_LEFT;
            }
        }
        entity->setPosition(tileSize/2, entityY);
//        entity->setPosition(gameMapsPoints[entityY][tileSize / 2]);
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
        entity->setPosition((entityX - entitySpeed), entityY);
//        entity->setPosition(gameMapsPoints[entityY][entityX - entitySpeed]);
    }

    delete circle;

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

bool GameEntityMovement::moveRandomly(int randomDirection) {
    // DOWN, RIGHT, LEFT, UP
    switch (randomDirection) {
        case 0:
            return move(MoveDirection::DOWN, EntityMovementState::WALK) == Constants::MoveSuccessValues::SUCCESS;
        case 1:
            return move(MoveDirection::RIGHT, EntityMovementState::WALK) == Constants::MoveSuccessValues::SUCCESS;
        case 2:
            return move(MoveDirection::LEFT, EntityMovementState::WALK) == Constants::MoveSuccessValues::SUCCESS;
        case 3:
            return move(MoveDirection::UP, EntityMovementState::WALK) == Constants::MoveSuccessValues::SUCCESS;
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

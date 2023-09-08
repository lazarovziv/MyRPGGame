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

bool GameEntityMovement::moveBasedOnPoint(Point *point, float dt) {
    if (point == nullptr) return false;
    // same row, can go up or down
    if (point->getX() == entity->getCircle()->getCenter()->getX()) {
        // go down
        if (point->getY() > entity->getCircle()->getCenter()->getY())
            return move(MoveDirection::DOWN, EntityMovementState::RUN, dt) == Constants::MoveSuccessValues::SUCCESS;
        // go up
        return move(MoveDirection::UP, EntityMovementState::RUN, dt) == Constants::MoveSuccessValues::SUCCESS;
    } else {
        // same col, can go right or left
        // go right
        if (point->getX() > entity->getCircle()->getCenter()->getX())
            return move(MoveDirection::RIGHT, EntityMovementState::RUN, dt) == Constants::MoveSuccessValues::SUCCESS;
        // go left
        return move(MoveDirection::LEFT, EntityMovementState::RUN, dt) == Constants::MoveSuccessValues::SUCCESS;
    }
}

void GameEntityMovement::calculatePathTo(GameEntity *target, float dt) {
    // if slope doesn't exist
    if (entity->getPosition().x == target->getPosition().x) return;
    // calculate linear interpolation
    float slope = (entity->getPosition().y - target->getPosition().y) / (entity->getPosition().x - target->getPosition().x);
    int b = entity->getPosition().y - slope * entity->getPosition().x;
    // TODO: understand slope and its positions
    // positive slope = go down iff
    if (slope > 0) {
        // go down
        if (entity->getPosition().y > target->getPosition().y) move(MoveDirection::DOWN, EntityMovementState::RUN, dt);
    }
}

Constants::MoveSuccessValues GameEntityMovement::move(MoveDirection direction, EntityMovementState movementState, float dt) {
    // no need to create extra variables if not using them
    if (movementState == EntityMovementState::IDLE) {
        entity->setMovementState(movementState);
        animationManager->animate(movementState, dt);
        // since we're idle, resetting the travel we covered until now
        entity->resetDistanceTraveledSinceIdle();
        entity->incrementIdleAnimationInterval(dt);
        return Constants::MoveSuccessValues::NOT_MOVED;
    }

    GameMap *map = currentMap;

    float entitySpeed = entity->getSpeed();
    float entityX = entity->getPosition().x;
    float entityY = entity->getPosition().y;

    Constants::MoveSuccessValues moveSuccessValue;

    if (direction == MoveDirection::UP) {
        moveSuccessValue = moveUp(map, entityX, entityY, entitySpeed, dt);
        // try and move up, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
        moveSuccessValue == Constants::MoveSuccessValues::CHANGE_UP) {
            entity->setMoveDirection(MoveDirection::UP);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::DOWN) {
        moveSuccessValue = moveDown(map, entityX, entityY, entitySpeed, dt);
        // try and move down, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
        moveSuccessValue == Constants::MoveSuccessValues::CHANGE_DOWN) {
            entity->setMoveDirection(MoveDirection::DOWN);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::RIGHT) {
        moveSuccessValue = moveRight(map, entityX, entityY, entitySpeed, dt);
        // try and move right, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_RIGHT) {
            entity->setMoveDirection(MoveDirection::RIGHT);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::LEFT) {
        moveSuccessValue = moveLeft(map, entityX, entityY, entitySpeed, dt);
        // try and move left, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_LEFT) {
            entity->setMoveDirection(MoveDirection::LEFT);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::UP_RIGHT) {
        moveSuccessValue = moveRightUp(map, entityX, entityY, entitySpeed, dt);
        // try and move right/up, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_RIGHT ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_UP) {
            entity->setMoveDirection(MoveDirection::RIGHT);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::UP_LEFT) {
        moveSuccessValue = moveLeftUp(map, entityX, entityY, entitySpeed, dt);
        // try and move left/up, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_LEFT ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_UP) {
            entity->setMoveDirection(MoveDirection::LEFT);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::DOWN_RIGHT) {
        moveSuccessValue = moveRightDown(map, entityX, entityY, entitySpeed, dt);
        // try and move right/down, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_RIGHT ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_DOWN) {
            entity->setMoveDirection(MoveDirection::RIGHT);
        } else return Constants::MoveSuccessValues::FAILURE;
    } else if (direction == MoveDirection::DOWN_LEFT) {
        moveSuccessValue = moveLeftDown(map, entityX, entityY, entitySpeed, dt);
        // try and move left/down, if false, collided with something
        if (moveSuccessValue == Constants::MoveSuccessValues::SUCCESS ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_LEFT ||
            moveSuccessValue == Constants::MoveSuccessValues::CHANGE_DOWN) {
            entity->setMoveDirection(MoveDirection::LEFT);
        } else return Constants::MoveSuccessValues::FAILURE;
    }

    entity->setMovementState(movementState);
    animationManager->animate(entity->getMovementState(), dt);
    entity->resetMoveInterval();
    entity->incrementDistanceTraveledSinceIdle(entitySpeed * dt);
    return moveSuccessValue;
}

Constants::MoveSuccessValues GameEntityMovement::moveUp(GameMap *map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached top of screen
    if (reachedTopEndOfScreen(entityY, entitySpeed, dt)) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop() && map->getTopExitCircle() != nullptr) {
            // if reached top exit circle of the map
            if (entity->getCircle()->intersects(map->getTopExitCircle())) {
                cout << "Reached Top Exit" << endl;
                entity->setPosition(entityX, (screenHeight - tileSize / 2));
                return Constants::MoveSuccessValues::CHANGE_UP;
            }
        }
        entity->setPosition(entityX, tileSize/2);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getTopCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getTopCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getTopCircle()->intersects(map->getPlayer()->getCircle());
    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(Vector2f(0, -1), dt);
    }


    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveDown(GameMap *map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached bottom of screen
    if (reachedBottomEndOfScreen(entityY, entitySpeed, dt)) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom() && map->getBottomExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getBottomExitCircle())) {
                cout << "Reached Bottom Exit" << endl;
                entity->setPosition(entityX, (tileSize) / 2);
                return Constants::MoveSuccessValues::CHANGE_DOWN;
            }
        }
        entity->setPosition(entityX, (screenHeight - tileSize/2));
        return Constants::MoveSuccessValues::FAILURE;
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getBottomCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getBottomCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getBottomCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(Vector2f(0, 1), dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveRight(GameMap *map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (reachedRightEndOfScreen(entityX, entitySpeed, dt)) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight() && map->getRightExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getRightExitCircle())) {
                cout << "Reached Right Exit" << endl;
                entity->setPosition(tileSize, entityY);
                return Constants::MoveSuccessValues::CHANGE_RIGHT;
            }
        }
        entity->setPosition((screenWidth - tileSize/2), entityY);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getRightCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getRightCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getRightCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(Vector2f(1, 0), dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveLeft(GameMap *map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached left of screen
    if (reachedLeftEndOfScreen(entityX, entitySpeed, dt)) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft() && map->getLeftExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getLeftExitCircle())) {
                cout << "Reached Left Exit" << endl;
                entity->setPosition((screenWidth - tileSize / 2), entityY);
                return Constants::MoveSuccessValues::CHANGE_LEFT;
            }
        }
        entity->setPosition(tileSize/2, entityY);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getLeftCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getLeftCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getLeftCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(Vector2f(-1, 0), dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveLeftUp(GameMap* map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached left of screen
    if (reachedLeftEndOfScreen(entityX, entitySpeed, dt)) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft() && map->getLeftExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getLeftExitCircle())) {
                cout << "Reached Left Exit" << endl;
                entity->setPosition((screenWidth - tileSize / 2), entityY);
                return Constants::MoveSuccessValues::CHANGE_LEFT;
            }
        }
        entity->setPosition(tileSize/2, entityY);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // reached top of screen
    if (reachedTopEndOfScreen(entityY, entitySpeed, dt)) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop() && map->getTopExitCircle() != nullptr) {
            // if reached top exit circle of the map
            if (entity->getCircle()->intersects(map->getTopExitCircle())) {
                cout << "Reached Top Exit" << endl;
                entity->setPosition(entityX, (screenHeight - tileSize / 2));
                return Constants::MoveSuccessValues::CHANGE_UP;
            }
        }
        entity->setPosition(entityX, tileSize/2);
        return Constants::MoveSuccessValues::FAILURE;
    }

    // direction vector
    Vector2f directionVector = Vector2f(-1, -1);
    // normalize
    normalizeVector(&directionVector);

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getLeftCircle())) {
            canCollide = true;
            break;
        }
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getTopCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getLeftCircle()) ||
                    map->getEnemies()[i]->getCircle()->intersects(entity->getTopCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getLeftCircle()->intersects(map->getPlayer()->getCircle()) ||
                entity->getTopCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(directionVector, dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveLeftDown(GameMap* map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached left of screen
    if (reachedLeftEndOfScreen(entityX, entitySpeed, dt)) {
        // check if left of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromLeft() && map->getLeftExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getLeftExitCircle())) {
                cout << "Reached Left Exit" << endl;
                entity->setPosition((screenWidth - tileSize / 2), entityY);
                return Constants::MoveSuccessValues::CHANGE_LEFT;
            }
        }
        entity->setPosition(tileSize/2, entityY);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // reached bottom of screen
    if (reachedBottomEndOfScreen(entityY, entitySpeed, dt)) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom() && map->getBottomExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getBottomExitCircle())) {
                cout << "Reached Bottom Exit" << endl;
                entity->setPosition(entityX, (tileSize) / 2);
                return Constants::MoveSuccessValues::CHANGE_DOWN;
            }
        }
        entity->setPosition(entityX, (screenHeight - tileSize/2));
        return Constants::MoveSuccessValues::FAILURE;
    }

    // direction vector
    Vector2f directionVector = Vector2f(-1, 1);
    // normalize
    normalizeVector(&directionVector);

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getLeftCircle())) {
            canCollide = true;
            break;
        }
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getBottomCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getLeftCircle()) ||
                map->getEnemies()[i]->getCircle()->intersects(entity->getBottomCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getLeftCircle()->intersects(map->getPlayer()->getCircle()) ||
                entity->getBottomCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(directionVector, dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveRightUp(GameMap* map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (reachedRightEndOfScreen(entityX, entitySpeed, dt)) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight() && map->getRightExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getRightExitCircle())) {
                cout << "Reached Right Exit" << endl;
                entity->setPosition(tileSize, entityY);
                return Constants::MoveSuccessValues::CHANGE_RIGHT;
            }
        }
        entity->setPosition((screenWidth - tileSize/2), entityY);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // reached top of screen
    if (reachedTopEndOfScreen(entityY, entitySpeed, dt)) {
        // check if top of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromTop() && map->getTopExitCircle() != nullptr) {
            // if reached top exit circle of the map
            if (entity->getCircle()->intersects(map->getTopExitCircle())) {
                cout << "Reached Top Exit" << endl;
                entity->setPosition(entityX, (screenHeight - tileSize / 2));
                return Constants::MoveSuccessValues::CHANGE_UP;
            }
        }
        entity->setPosition(entityX, tileSize/2);
        return Constants::MoveSuccessValues::FAILURE;
    }

    // direction vector
    Vector2f directionVector = Vector2f(1, -1);
    // normalize
    normalizeVector(&directionVector);

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getRightCircle())) {
            canCollide = true;
            break;
        }
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getTopCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getRightCircle()) ||
                map->getEnemies()[i]->getCircle()->intersects(entity->getTopCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getRightCircle()->intersects(map->getPlayer()->getCircle()) ||
                entity->getTopCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(directionVector, dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

Constants::MoveSuccessValues GameEntityMovement::moveRightDown(GameMap* map, float entityX, float entityY, float entitySpeed, float dt) {
    int unreachableAreasSize = map->getLandscapes().size();
    bool canCollide = false;
    // reached right of screen
    if (reachedRightEndOfScreen(entityX, entitySpeed, dt)) {
        // check if right of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromRight() && map->getRightExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getRightExitCircle())) {
                cout << "Reached Right Exit" << endl;
                entity->setPosition(tileSize, entityY);
                return Constants::MoveSuccessValues::CHANGE_RIGHT;
            }
        }
        entity->setPosition((screenWidth - tileSize/2), entityY);
        return Constants::MoveSuccessValues::FAILURE;
    }
    // reached bottom of screen
    if (reachedBottomEndOfScreen(entityY, entitySpeed, dt)) {
        // check if bottom of screen is an exit point for current game map
        if (isPlayer && map->isExitableFromBottom() && map->getBottomExitCircle() != nullptr) {
            if (entity->getCircle()->intersects(map->getBottomExitCircle())) {
                cout << "Reached Bottom Exit" << endl;
                entity->setPosition(entityX, (tileSize) / 2);
                return Constants::MoveSuccessValues::CHANGE_DOWN;
            }
        }
        entity->setPosition(entityX, (screenHeight - tileSize/2));
        return Constants::MoveSuccessValues::FAILURE;
    }

    // direction vector
    Vector2f directionVector = Vector2f(1, 1);
    // normalize
    normalizeVector(&directionVector);

    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (map->getLandscapes()[i]->getCircle()->intersects(entity->getRightCircle())) {
            canCollide = true;
            break;
        }
    }
    // check if the move can overlap with any of the unreachable areas
    for (int i = 0; i < unreachableAreasSize; i++) {
        if (/*map->getLandscapes()[i]->getRectangle().intersects(*rect) ||*/
                map->getLandscapes()[i]->getCircle()->intersects(entity->getBottomCircle())) {
            canCollide = true;
            break;
        }
    }

    if (isPlayer) {
        // check for collisions with enemies
        for (int i = 0; i < map->getEnemies().size(); i++) {
            if (map->getEnemies()[i]->getCircle()->intersects(entity->getRightCircle()) ||
                map->getEnemies()[i]->getCircle()->intersects(entity->getBottomCircle())) {
                canCollide = true;
                break;
            }
        }
    } else canCollide = entity->getRightCircle()->intersects(map->getPlayer()->getCircle()) ||
                entity->getBottomCircle()->intersects(map->getPlayer()->getCircle());

    if (!canCollide) {
        // in legal boundaries
        entity->setPosition(directionVector, dt);
    }

    if (canCollide) return Constants::MoveSuccessValues::FAILURE;
    return Constants::MoveSuccessValues::SUCCESS;
}

bool GameEntityMovement::reachedLeftEndOfScreen(float entityX, float entitySpeed, float dt) const {
    return entityX - entitySpeed <= tileSize / 2;
}

bool GameEntityMovement::reachedRightEndOfScreen(float entityX, float entitySpeed, float dt) const {
    return entityX + entitySpeed >= screenWidth - tileSize / 2;
}

bool GameEntityMovement::reachedTopEndOfScreen(float entityY, float entitySpeed, float dt) const {
    return entityY - entitySpeed <= tileSize / 2;
}

bool GameEntityMovement::reachedBottomEndOfScreen(float entityY, float entitySpeed, float dt) const {
    return entityY + entitySpeed >= screenHeight - tileSize / 2;
}

void GameEntityMovement::normalizeVector(sf::Vector2f *vector) {
    float normSquared = vector->x * vector->x + vector->y * vector->y;
    float norm = sqrt(normSquared);
    vector->x /= norm;
    vector->y /= norm;
}

bool GameEntityMovement::moveRandomly(int randomDirection, float dt) {
    // DOWN, RIGHT, LEFT, UP
    switch (randomDirection) {
        case 0:
            return move(MoveDirection::DOWN, EntityMovementState::WALK, dt) == Constants::MoveSuccessValues::SUCCESS;
        case 1:
            return move(MoveDirection::RIGHT, EntityMovementState::WALK, dt) == Constants::MoveSuccessValues::SUCCESS;
        case 2:
            return move(MoveDirection::LEFT, EntityMovementState::WALK, dt) == Constants::MoveSuccessValues::SUCCESS;
        case 3:
            return move(MoveDirection::UP, EntityMovementState::WALK, dt) == Constants::MoveSuccessValues::SUCCESS;
        default:
            // TODO: add error handling
            break;
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

void GameEntityMovement::setCurrentMap(GameMap *map) {
    currentMap = map;
}

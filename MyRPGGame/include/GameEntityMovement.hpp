//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include "GameMap.hpp"
#include "Constants.h"
#include "AnimationManager.hpp"

class GameEntityMovement {
private:
    GameEntity *entity;
    std::shared_ptr<GameMap> currentMap;
    Point ***gameMapsPoints;
    AnimationManager *animationManager;
    bool isPlayer;
    int screenWidth, screenHeight;
    int tileSize;

    // TODO: add delta time multiplication with *entitySpeed* for every movement which is not screen borders
    Constants::MoveSuccessValues moveUp(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveDown(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveRight(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveLeft(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveLeftUp(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveLeftDown(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveRightUp(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);
    Constants::MoveSuccessValues moveRightDown(GameMap* map, real entityX, real entityY, real entitySpeed, real dt);

    bool reachedLeftEndOfScreen(real entityX, real entitySpeed, real dt) const;
    bool reachedRightEndOfScreen(real entityX, real entitySpeed, real dt) const;
    bool reachedTopEndOfScreen(real entityY, real entitySpeed, real dt) const;
    bool reachedBottomEndOfScreen(real entityY, real entitySpeed, real dt) const;

    void normalizeVector(sf::Vector2f *vector);

public:
    GameEntityMovement(GameEntity* entity, bool player, std::shared_ptr<GameMap> map);
    GameEntityMovement(GameEntity* entity, bool player, std::shared_ptr<GameMap> map, Point ***points);
    ~GameEntityMovement() = default;
    Constants::MoveSuccessValues move(MoveDirection direction, EntityMovementState movementState, real dt);
    // TODO: add run method (use acceleration, speed etc.)

    bool moveRandomly(int randomDirection, real dt);
    bool moveTowardsEntity(GameEntity *gameEntity, Graph<Point *> *graph);
    bool moveTowardsPoint(Point *point, Graph<Point *> *graph);
    bool moveBasedOnPoint(Point *point, real dt);

    void calculatePathTo(GameEntity *target, real dt);

    void setEntity(GameEntity &entity);
    GameEntity* getEntity();
    void setCurrentMap(std::shared_ptr<GameMap> map);
};

#endif /* GameEntityMovement_hpp */

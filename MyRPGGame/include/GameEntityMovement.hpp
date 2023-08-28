//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include "GameMap.hpp"
#include "AnimationManager.hpp"

class GameEntityMovement {
private:
    GameEntity *entity;
    GameMap *currentMap;
    Point ***gameMapsPoints;
    AnimationManager *animationManager;
    bool isPlayer;
    int screenWidth, screenHeight;
    int tileSize;

    Constants::MoveSuccessValues moveUp(GameMap* currentMap, float entityX, float entityY, float entitySpeed);
    Constants::MoveSuccessValues moveDown(GameMap* currentMap, float entityX, float entityY, float entitySpeed);
    Constants::MoveSuccessValues moveRight(GameMap* currentMap, float entityX, float entityY, float entitySpeed);
    Constants::MoveSuccessValues moveLeft(GameMap* currentMap, float entityX, float entityY, float entitySpeed);

public:
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map);
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map, Point ***points);
    ~GameEntityMovement() = default;
    Constants::MoveSuccessValues move(MoveDirection direction, EntityMovementState movementState);
    // TODO: add run method (use acceleration, speed etc.)

    bool moveRandomly(int randomDirection);
    bool moveTowardsEntity(GameEntity *gameEntity, Graph<Point *> *graph);
    bool moveTowardsPoint(Point *point, Graph<Point *> *graph);
    bool moveBasedOnPoint(Point *point);

    void setEntity(GameEntity &entity);
    GameEntity* getEntity();
    void setCurrentMap(GameMap *map);
};

#endif /* GameEntityMovement_hpp */

//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include "GameEntity.hpp"
#include "GameMap.hpp"
#include "Constants.h"

class GameEntityMovement {
private:
    GameEntity *entity;
    GameMap *currentMap;
    Point ***gameMapsPoints;
    bool isPlayer;
    int screenWidth, screenHeight;
    int tileSize;

    Constants::MoveSuccessValues moveUp(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    Constants::MoveSuccessValues moveDown(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    Constants::MoveSuccessValues moveRight(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    Constants::MoveSuccessValues moveLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    // diagonal moves
//    Constants::MoveSuccessValues moveUpRight(GameMap  *currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
//    Constants::MoveSuccessValues moveUpLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
//    Constants::MoveSuccessValues moveDownRight(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
//    Constants::MoveSuccessValues moveDownLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
public:
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map);
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map, Point ***points);
    ~GameEntityMovement() = default;
    Constants::MoveSuccessValues move(MoveDirection direction);

    bool moveRandomly(int randomDirection);
    bool moveTowardsEntity(GameEntity *gameEntity, Graph<Point *> *graph);
    bool moveTowardsPoint(Point *point, Graph<Point *> *graph);
    bool moveBasedOnPoint(Point *point);

    void setEntity(GameEntity &entity);
    GameEntity* getEntity();
    void setCurrentMap(GameMap *map);
};

#endif /* GameEntityMovement_hpp */

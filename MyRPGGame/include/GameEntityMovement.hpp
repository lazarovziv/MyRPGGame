//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include "GameEntity.hpp"
#include "GameMap.hpp"
#include "../include/Game.hpp"
#include "../include/Constants.h"

class GameEntityMovement {
private:
    GameEntity *entity;
    GameMap *currentMap;
    Point ***gameMapsPoints;
    bool isPlayer;
    int screenWidth, screenHeight;
    int tileSize;
    
    bool moveUp(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveDown(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveRight(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    // diagonal moves
    bool moveUpRight(GameMap  *currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveUpLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveDownRight(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveDownLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
public:
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map);
    GameEntityMovement(GameEntity* entity, bool player, GameMap *map, Point ***points);
    ~GameEntityMovement() = default;
    bool move(MoveDirection direction);

    bool moveRandomly(int randomDirection);
    bool moveTowardsEntity(GameEntity* gameEntity, Graph<Point *> *graph);
    bool moveBasedOnPoint(Point *point);

    void setEntity(GameEntity &entity);
    GameEntity* getEntity();
};

#endif /* GameEntityMovement_hpp */

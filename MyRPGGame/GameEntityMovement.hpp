//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include <stdio.h>
#include "GameEntity.hpp"
#include "GameMap.hpp"
//class GameEntity;
//class MoveDirection;
//class GameMap;

class GameEntityMovement {
private:
    GameEntity* entity;
    int screenWidth, screenHeight;
    int tileSize;
    
    bool moveUp(GameMap* currentMap, float entityX, float entityY, float entitySpeed, FloatRect &entityRect);
    bool moveDown(GameMap* currentMap, float entityX, float entityY, float entitySpeed, FloatRect &entityRect);
    bool moveRight(GameMap* currentMap, float entityX, float entityY, float entitySpeed, FloatRect &entityRect);
    bool moveLeft(GameMap* currentMap, float entityX, float entityY, float entitySpeed, FloatRect &entityRect);
public:
    GameEntityMovement(GameEntity* entity);
    ~GameEntityMovement() = default;
    bool move(MoveDirection direction);
};

#endif /* GameEntityMovement_hpp */

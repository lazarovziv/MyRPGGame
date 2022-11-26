//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include <stdio.h>
#include "GameEntity.hpp"
#include "GameMap.hpp"
#include "../include/Game.hpp"
#include "../include/Constants.h"

class GameEntityMovement {
private:
    GameEntity* entity;
    bool isPlayer;
    int screenWidth, screenHeight;
    int tileSize;
    
    bool moveUp(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveDown(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveRight(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
    bool moveLeft(GameMap* currentMap, int entityX, int entityY, int entitySpeed, IntRect &entityRect);
public:
    GameEntityMovement(GameEntity* entity, bool player);
    ~GameEntityMovement() = default;
    bool move(MoveDirection direction);

    void setEntity(GameEntity &entity);
    GameEntity* getEntity();
};

#endif /* GameEntityMovement_hpp */

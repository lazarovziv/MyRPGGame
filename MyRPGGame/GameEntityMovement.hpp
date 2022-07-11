#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include <stdio.h>
#include "GameEntity.hpp"
#include "GameMap.hpp"

class GameEntityMovement {
private:
    GameEntity* entity;
    int screenWidth, screenHeight;
    int tileSize;
public:
    GameEntityMovement(GameEntity* entity);
    ~GameEntityMovement() = default;
    bool move(MoveDirection direction);
};

#endif /* GameEntityMovement_hpp */

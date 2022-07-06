#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include <stdio.h>
#include "GameEntity.hpp"
#include "GameMap.hpp"

class GameEntityMovement {
private:
    GameEntity* entity;
    int screenWidth, screenHeight;
public:
    GameEntityMovement(GameEntity* entity);
    ~GameEntityMovement() = default;
    bool move(MoveDirection direction);
    void setScreenWidth(int width);
    void setScreenHeight(int height);
};

#endif /* GameEntityMovement_hpp */

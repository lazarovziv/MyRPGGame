#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include <stdio.h>
#include "GameEntity.hpp"
#include "Game.hpp"
#include "GameMap.hpp"

class GameEntityMovement {
private:
    GameEntity* entity;
    Game* game;
    GameMap* map;
public:
    GameEntityMovement(Game* game, GameEntity* entity, GameMap* map);
    ~GameEntityMovement() = default;
    bool move(MoveDirection direction);
};

#endif /* GameEntityMovement_hpp */

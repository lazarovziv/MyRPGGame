#ifndef GameEntityBattle_hpp
#define GameEntityBattle_hpp

#include <iostream>
#include "GameMap.hpp"
#include "Player.hpp"
#include "AnimationManager.hpp"

class GameEntityBattle {
private:
    GameEntity *entity;
    AnimationManager *animationManager;
    
public:
    explicit GameEntityBattle(GameEntity* entity);
    ~GameEntityBattle() = default;
    GameEntity *getEntity();
    void setEntity(GameEntity *gameEntity);
    bool attack(GameEntity &enemy, real dt);
};

#endif /* GameEntityBattle_hpp */

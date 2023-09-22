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
    GameEntityBattle(GameEntity* entity);
    ~GameEntityBattle() = default;
    GameEntity *getEntity();
    void setEntity(GameEntity *gameEntity);
    bool attack(GameEntity &enemy, real dt);
    bool isInAttackRange(GameEntity &enemy);
    bool animate(real dt);
};

#endif /* GameEntityBattle_hpp */

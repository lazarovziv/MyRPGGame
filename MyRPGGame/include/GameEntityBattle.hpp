#ifndef GameEntityBattle_hpp
#define GameEntityBattle_hpp

#include <iostream>
#include "GameMap.hpp"
#include "Player.hpp"
#include "AnimationManager.hpp"

using namespace std;

class GameEntityBattle {
private:
    GameEntity *entity;
    AnimationManager *animationManager;
    
public:
    GameEntityBattle(GameEntity* entity);
    ~GameEntityBattle() = default;
    GameEntity *getEntity();
    void setEntity(GameEntity *gameEntity);
    bool attack(GameEntity &enemy, float dt);
    bool isInAttackRange(GameEntity &enemy);
    bool animate(float dt);
};

#endif /* GameEntityBattle_hpp */

#ifndef GameEntityBattle_hpp
#define GameEntityBattle_hpp

#include <iostream>
#include "GameMap.hpp"
#include "Player.hpp"

using namespace std;

class GameEntityBattle {
private:
    GameEntity *entity;
    
public:
    GameEntityBattle(GameEntity* entity);
    ~GameEntityBattle() = default;
    GameEntity *getEntity();
    void setEntity(GameEntity *gameEntity);
    bool attack(GameEntity &enemy);
    bool isInAttackRange(GameEntity &enemy);
};

#endif /* GameEntityBattle_hpp */

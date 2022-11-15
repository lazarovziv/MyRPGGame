#ifndef GameEntityBattle_hpp
#define GameEntityBattle_hpp

#include <stdio.h>
#include <iostream>
#include "GameEntity.hpp"
#include "GameMap.hpp"

using namespace std;

class GameEntityBattle {
private:
    GameEntity* entity;
    
public:
    GameEntityBattle(GameEntity* entity);
    ~GameEntityBattle() = default;
    bool attack(GameEntity &enemy);
    bool isInAttackRange(GameEntity &enemy);
};

#endif /* GameEntityBattle_hpp */

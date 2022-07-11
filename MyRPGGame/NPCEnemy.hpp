#pragma once

#ifndef NPCEnemy_hpp
#define NPCEnemy_hpp

#include <stdio.h>
#include "GameEntity.hpp"

using namespace sf;

enum class EnemyType { WORM, SNAKE, BIRD, ETC }; // add more

class NPCEnemy : public GameEntity {
private:
    // duration of "chasing" player after engaged close
    int battleTimeout;
    // radius for area to wander when not engaged with player
    float wanderAreaRadius;
    // radius for area to battle player after engaging (needs to be bigger than wander)
    float battleAreaRadius;
    // type of enemy
    EnemyType type;
    
public:
    NPCEnemy();
    NPCEnemy(EnemyType type, float x, float y);
    ~NPCEnemy() = default;
    int getBattleTimeout();
    float getWanderAreaRadius();
    float getBattleAreaRadius();
    EnemyType getType();
    
    void spawn(float x, float y);
};

#endif /* NPCEnemy_hpp */

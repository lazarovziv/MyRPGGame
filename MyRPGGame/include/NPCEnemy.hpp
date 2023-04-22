//#pragma once

#ifndef NPCEnemy_hpp
#define NPCEnemy_hpp

#include <iostream>
#include <ctime>
#include "GameEntity.hpp"
#include "Circle.hpp"
#include "Constants.h"
#include "MovementObserver.hpp"

using namespace sf;

enum class EnemyType { WORM, SNAKE, BIRD, ETC }; // add more

class NPCEnemy : public GameEntity, public MovementObserver {
private:
    // duration of "chasing" player after engaged close
    int battleTimeout;
    // radius for area to wander when not engaged with player
    float wanderAreaRadius;
    // radius for area to battle player after engaging (needs to be bigger than wander)
    float battleAreaRadius;
    // type of enemy
    int type;
    // movement intervals
    std::clock_t lastTimeMoved;
    float moveInterval = 0.25;
    
    // movement handler
//    GameEntityMovement* movement;
    
    int expPointsWorth;
    
    Circle *spawnArea = nullptr;
    
public:
    static const int WORM = 1;
    static const int SNAKE = 2;
    static const int BIRD = 3;
    static const int ETC = 4;
    
    NPCEnemy();
    NPCEnemy(int type, int x, int y);
    ~NPCEnemy();
    int getBattleTimeout();
    float getWanderAreaRadius();
    float getBattleAreaRadius();
    int getExpPointsWorth();
    int getType();
    
    bool canMove();
    
    Circle* getSpawnArea();
    void setSpawnArea(int centerX, int centerY, float radius);
    
    void spawn(int x, int y);

    void notify() override;
};

#endif /* NPCEnemy_hpp */

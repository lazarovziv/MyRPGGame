//#pragma once

#ifndef NPCEnemy_hpp
#define NPCEnemy_hpp

#include <stdio.h>
#include "GameEntity.hpp"
#include "Circle.hpp"
#include "Constants.h"

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
    int type;
    // movement intervals
//    float lastTimeMoved;
//    float moveInterval;
    
    // movement handler
//    GameEntityMovement* movement;
    
    int expPointsWorth;
    
    Circle* spawnArea = nullptr;
    
public:
    static const int WORM = 1;
    static const int SNAKE = 2;
    static const int BIRD = 3;
    static const int ETC = 4;
    
    NPCEnemy();
    NPCEnemy(int type, float x, float y);
    ~NPCEnemy() = default;
    int getBattleTimeout();
    float getWanderAreaRadius();
    float getBattleAreaRadius();
    int getExpPointsWorth();
    int getType();
    
//    float getLastTimeMoved() {
//        return lastTimeMoved;
//    }
//    
//    float getMoveInterval() {
//        return moveInterval;
//    }
//    
//    void setLastTimeMoved(float time) {
//        lastTimeMoved = time;
//    }
//    
//    void setMoveInterval(float time) {
//        moveInterval = time;
//    }
    
//    bool move();
    
    Circle* getSpawnArea();
    void setSpawnArea(float centerX, float centerY, float radius);
    
    void spawn(float x, float y);
};

#endif /* NPCEnemy_hpp */

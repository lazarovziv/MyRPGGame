//#pragma once

#ifndef NPCEnemy_hpp
#define NPCEnemy_hpp

#include <iostream>
#include <ctime>
#include <stack>
#include "GameEntity.hpp"
#include "Circle.hpp"
#include "Constants.h"
#include "MovementObserver.hpp"

using namespace sf;

enum class EnemyType { WORM, SNAKE, BIRD, ETC }; // add more

class NPCEnemy : public GameEntity, public MovementObserver {
private:
    // radius for area to wander when not engaged with player
    float wanderAreaRadius;
    Circle *wanderAreaCircle = nullptr;
    std::clock_t lastTimeWandered;
    // radius for area to battle player after engaging (needs to be bigger than wander)
    float battleAreaRadius;
    Circle *battleAreaCircle = nullptr;
    std::clock_t lastTimeBattled;
    // duration of "chasing" player after engaged close
    int battleTimeout = 5;
    // type of enemy
    int type;
    // movement intervals
    std::clock_t lastTimeMoved;
    float moveInterval = MOVE_INTERVAL_DEFAULT;
    // duration of regenerating path to wander area
    int wanderTimeout = 7;
    
    // movement handler
//    GameEntityMovement* movement;
    
    int expPointsWorth;
    
    Circle *spawnArea = nullptr;
    
public:
    static const int WORM = 1;
    static const int SNAKE = 2;
    static const int BIRD = 3;
    static const int ETC = 4;
    constexpr static const float MOVE_INTERVAL_DEFAULT = 0.25;
    
    NPCEnemy();
    NPCEnemy(int type, int x, int y);
    NPCEnemy(int type, Point *center);
    ~NPCEnemy();
    int getBattleTimeout();
    float getWanderAreaRadius();
    float getBattleAreaRadius();
    int getExpPointsWorth();
    int getType();
    
    bool canMove();
    bool canGoToBattle();
    bool canGoToWanderArea();
    bool isInBattleArea();
    bool isInWanderArea();
    
    Circle* getSpawnArea();
    Circle *getWanderAreaCircle();
    Circle *getBattleAreaCircle();
    void setSpawnArea(int centerX, int centerY, float radius);
    
    void spawn(int x, int y);
    void setMoveInterval(float interval);

    void notify() override;
};

#endif /* NPCEnemy_hpp */

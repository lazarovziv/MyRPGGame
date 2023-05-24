//#pragma once

#ifndef NPCEnemy_hpp
#define NPCEnemy_hpp

#include <iostream>
#include <ctime>
#include <stack>
#include "GameEntity.hpp"
#include "Circle.hpp"
#include "Constants.h"
#include "Observer.hpp"

using namespace sf;

enum class EnemyType { WORM, SNAKE, BIRD, ETC }; // add more

class NPCEnemy : public GameEntity, public Observer {
private:
    // radius for area to wander when not engaged with player
    float wanderAreaRadius;
    Circle *wanderAreaCircle = nullptr;
    std::clock_t lastTimeWandered;
    // radius for area to battle player after engaging (needs to be bigger than wander)
    float battleAreaRadius;
    Circle *battleAreaCircle = nullptr;
    // type of enemy
    int type;
    // movement intervals
    std::clock_t lastTimeMoved;
    float moveInterval = MOVE_INTERVAL_DEFAULT;
    // duration of regenerating path to wander area
    int wanderTimeout = 7;
    bool onWayToWanderArea = false;
    
    // movement handler
//    GameEntityMovement* movement;
    
    int expPointsWorth;
    
public:
    static const int WORM = 1;
    static const int SNAKE = 2;
    static const int BIRD = 3;
    static const int ETC = 4;
    constexpr static const float MOVE_INTERVAL_DEFAULT = 0.25;
    
    NPCEnemy() = default;
    explicit NPCEnemy(int type, Point *center);
    ~NPCEnemy();
    int getBattleTimeout();
    float getWanderAreaRadius();
    float getBattleAreaRadius();
    int getExpPointsWorth();
    int getType();
    
    bool canMove();
    bool canGoToWanderArea();
    bool isInBattleArea();
    bool isInWanderArea();

    Circle *getWanderAreaCircle();
    Circle *getBattleAreaCircle();

    void setMoveInterval(float interval);

    void update(Point ***points) override;
    void notify() override;
};

#endif /* NPCEnemy_hpp */

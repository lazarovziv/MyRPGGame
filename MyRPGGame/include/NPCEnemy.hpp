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
    constexpr static const float WANDER_AREA_INTERVAL_DEFAULT = 168.0f;
    // duration of regenerating path to wander area
    float wanderAreaInterval = 0;
    bool onWayToWanderArea = false;
    Circle *wanderAreaCircle = nullptr;
    // radius for area to battle player after engaging (needs to be bigger than wander)
    float battleAreaRadius;
    Circle *battleAreaCircle = nullptr;
    // type of enemy
    int type;
    // movement interval
    constexpr static const float MOVE_INTERVAL_DEFAULT = 42.0f; // TODO: tweak a little bit more
    
    // movement handler
//    GameEntityMovement* movement;
    
    int expPointsWorth;
    
public:
    static const int WORM = 1;
    static const int SNAKE = 2;
    static const int BIRD = 3;
    static const int ETC = 4;
    
    NPCEnemy() = default;
    explicit NPCEnemy(int type, Point *center);
    ~NPCEnemy();
    int getBattleTimeout();
    float getWanderAreaRadius();
    float getBattleAreaRadius();
    int getExpPointsWorth();
    int getType();

    bool canMove() const;
    bool canGoToWanderArea();
    bool isInBattleArea();
    bool isInWanderArea();

    Circle *getWanderAreaCircle();
    Circle *getBattleAreaCircle();

    void setMoveInterval(float interval);

    void update(Point ***points, float dt) override;
    void notify() override;
};

#endif /* NPCEnemy_hpp */

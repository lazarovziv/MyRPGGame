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

enum class EnemyType { WORM, SNAKE, BIRD, ETC }; // add more

class NPCEnemy : public GameEntity, public Observer {
private:
    // radius for area to wander when not engaged with player
    real wander_area_radius;
    constexpr static const real WANDER_AREA_INTERVAL_DEFAULT = 168.0f;
    // duration of regenerating path to wander area
    real wander_area_interval = 0;
    bool on_way_to_wander_area = false;
    Circle *wander_area_circle = nullptr;
    // radius for area to battle player after engaging (needs to be bigger than wander)
    real battle_area_radius;
    Circle *battle_area_circle = nullptr;
    // type of enemy
    int type;
    // movement interval
    constexpr static const real MOVE_INTERVAL_DEFAULT = 42.0f; // TODO: tweak a little bit more
    
    // movement handler
//    GameEntityMovement* movement;
    
    int exp_points_worth;
    
public:
    static const int WORM = 1;
    static const int SNAKE = 2;
    static const int BIRD = 3;
    static const int ETC = 4;
    
    NPCEnemy() = default;
    explicit NPCEnemy(int type, physics::Vector initial_position);
    ~NPCEnemy() override;
    int get_battle_timeout();
    real get_wander_area_radius();
    real get_battle_area_radius();
    int get_exp_points_worth();
    int get_type();

    bool can_move() const;
    bool can_go_to_wander_area();
    bool is_in_battle_area();
    bool is_in_wander_area();

    Circle *get_wander_area_circle();
    Circle *get_battle_area_circle();

    void set_move_interval(real interval);

    void notify() override;
};

#endif /* NPCEnemy_hpp */

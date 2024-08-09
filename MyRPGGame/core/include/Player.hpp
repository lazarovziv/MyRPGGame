//#pragma once

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include "GameEntity.hpp"
#include "Constants.h"
#include "Subject.hpp"
#include "TextureLoader.hpp"

enum class PlayerType { KNIGHT, WIZARD, DUAL_WIELDER };

class Player : public GameEntity, public Subject {
private:
    // for knight
    int strength_points;
    // for wizard
    int intelligence_points;
    // for dual wielder
    int critical_hist_points;
    int exp_points;
    // Weapon currentWeapon
    // Helmet helmet
    // Braces braces
    // ...
    PlayerType type;
    
public:
    Player();
    explicit Player(PlayerType type, physics::Vector initial_position);
    ~Player() override;
    int get_strength_points();
    int get_intelligence_points();
    int get_critical_hist_points();
    int get_exp_points();
    // called when leveling up attributes
    void level_up_strength_points();
    void level_up_intelligence_points();
    void level_up_critical_hits_points();
    void increment_exp_points(int amount);
    void set_player_type(PlayerType type);

};

#endif /* Player_hpp */

//#pragma once

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <iostream>
#include "GameEntity.hpp"
#include "Constants.h"

enum class PlayerType { KNIGHT, WIZARD, DUAL_WIELDER };

class Player : public GameEntity {
private:
    // for knight
    int* strengthPoints = nullptr;
    // for wizard
    int* intelligencePoints = nullptr;
    // for dual wielder
    int* criticalHitsPoints = nullptr;
    int* expPoints = nullptr;
    // Weapon currentWeapon
    // Helmet helmet
    // Braces braces
    // ...
    PlayerType type;
    
public:
    Player();
    Player(PlayerType type);
    ~Player();
    int getStrengthPoints();
    int getIntelligencePoints();
    int getCriticalHitPoints();
    int getExpPoints();
    // called when leveling up attributes
    void levelUpStrengthPoints();
    void levelUpIntelligencePoints();
    void levelUpCriticalHitsPoints();
    void incrementExpPoints(int amount);
    void setPlayerType(PlayerType type);
};

#endif /* Player_hpp */

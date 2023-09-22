//#pragma once

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include "GameEntity.hpp"
#include "Constants.h"
#include "Subject.hpp"
#include "../include/TextureLoader.hpp"

enum class PlayerType { KNIGHT, WIZARD, DUAL_WIELDER };

class Player : public GameEntity, public Subject {
private:
    // for knight
    int strengthPoints;
    // for wizard
    int intelligencePoints;
    // for dual wielder
    int criticalHitsPoints;
    int expPoints;
    // Weapon currentWeapon
    // Helmet helmet
    // Braces braces
    // ...
    PlayerType type;
    
public:
    Player();
    Player(PlayerType type);
    explicit Player(PlayerType type, Point *center);
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

    void update(Point ***points, real dt) override;
};

#endif /* Player_hpp */

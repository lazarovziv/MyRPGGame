#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "GameEntity.hpp"

enum class PlayerType { KNIGHT, WIZARD, DUAL_WIELDER };

class Player : public GameEntity {
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
    ~Player() = default;
    int getStrengthPoints();
    int getIntelligencePoints();
    int getCriticalHitPoints();
    int getExpPoints();
    // called when leveling up attributes
    void levelUpStrengthPoints();
    void levelUpIntelligencePoints();
    void levelUpCriticalHitsPoints();
    void setPlayerType(PlayerType type);
};

#endif /* Player_hpp */

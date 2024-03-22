#ifndef GameEntityBattle_hpp
#define GameEntityBattle_hpp

#include <iostream>
#include "../../maps/include/GameMap.hpp"
#include "../../entities/include/Player.hpp"
#include "../../animation/include/AnimationManager.hpp"

class GameEntityBattle {
private:
    GameEntity *entity;
    std::unique_ptr<AnimationManager> animationManager;
    
public:
    explicit GameEntityBattle(GameEntity *entity);
    ~GameEntityBattle() = default;
    GameEntity *getEntity();
    void setEntity(GameEntity *gameEntity);
    bool attack(GameEntity &enemy, real dt);
};

#endif /* GameEntityBattle_hpp */

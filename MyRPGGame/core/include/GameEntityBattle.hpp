#ifndef GameEntityBattle_hpp
#define GameEntityBattle_hpp

#include <iostream>
#include "GameMap.hpp"
#include "Player.hpp"
#include "AnimationManager.hpp"

class GameEntityBattle {
private:
    GameEntity *entity;
    std::unique_ptr<AnimationManager> animationManager;
    
public:
    explicit GameEntityBattle(GameEntity *entity);
    ~GameEntityBattle() = default;
    GameEntity *get_entity();
    void set_entity(GameEntity *game_entity);
    bool attack(GameEntity &enemy, real dt);
};

#endif /* GameEntityBattle_hpp */

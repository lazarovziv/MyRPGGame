//#pragma once

#ifndef GameEntityMovement_hpp
#define GameEntityMovement_hpp

#include "GameMap.hpp"
#include "Constants.h"
#include "AnimationManager.hpp"
#include "Collision.hpp"

class GameEntityMovement {
private:
    GameEntity *entity;
    std::shared_ptr<GameMap> current_map;
    std::unique_ptr<AnimationManager> animation_manager;

public:
    GameEntityMovement(GameEntity *entity, bool player, std::shared_ptr<GameMap> map);
    ~GameEntityMovement() = default;

    Constants::MoveSuccessValues move(const physics::Vector &direction, real dt);
    // TODO: add run method (use acceleration, speed etc.)
    bool jump(const physics::Vector &direction, real dt);

    bool move_randomly(int random_direction, real dt);

    void set_entity(GameEntity &entity);
    GameEntity *get_entity();
    void set_current_map(std::shared_ptr<GameMap> map);
};

#endif /* GameEntityMovement_hpp */

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
    std::shared_ptr<GameMap> currentMap;
    std::unique_ptr<AnimationManager> animationManager;

public:
    GameEntityMovement(GameEntity *entity, bool player, std::shared_ptr<GameMap> map);
    ~GameEntityMovement() = default;

    Constants::MoveSuccessValues move(const physics::Vector &direction, real dt);
    // TODO: add run method (use acceleration, speed etc.)
    bool jump(const physics::Vector &direction, real dt);

    bool moveRandomly(int randomDirection, real dt);

    void setEntity(GameEntity &entity);
    GameEntity *getEntity();
    void setCurrentMap(std::shared_ptr<GameMap> map);
};

#endif /* GameEntityMovement_hpp */

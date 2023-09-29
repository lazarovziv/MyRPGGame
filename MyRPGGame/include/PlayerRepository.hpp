#ifndef MYRPGGAME_PLAYERREPOSITORY_HPP
#define MYRPGGAME_PLAYERREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class PlayerRepository {
private:
    std::shared_ptr<Player> player;
    GameEntityMovement *movementHandler = nullptr;
    GameEntityBattle *battleHandler = nullptr;
    std::unique_ptr<physics::RigidBodyGravity> forceGenerator;
    std::shared_ptr<GameMap> map;

    AnimationManager *animationManager = nullptr;

public:
    explicit PlayerRepository(const std::shared_ptr<Player> player, GameEntityMovement *movement,
                              GameEntityBattle *battle, const std::shared_ptr<GameMap> gameMap);
    ~PlayerRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

    void setGameMap(std::shared_ptr<GameMap> gameMap);
    void setLastTimeMoved(std::clock_t time);
    bool move(physics::Vector direction, bool run, real dt);
//    Constants::MoveSuccessValues move(MoveDirection direction, EntityMovementState movementState, real dt);
    bool attack(real dt);
    void update(Constants::MoveSuccessValues moveSuccessValue, real dt);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

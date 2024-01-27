#ifndef MYRPGGAME_PLAYERREPOSITORY_HPP
#define MYRPGGAME_PLAYERREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class PlayerRepository {
private:
    std::shared_ptr<Player> player;
    GameEntityMovement &movementHandler;
    GameEntityBattle &battleHandler;
    std::shared_ptr<GameMap> map;
    AnimationManager *animationManager = nullptr;

public:
    explicit PlayerRepository(const std::shared_ptr<Player> &player, GameEntityMovement &movement,
                              GameEntityBattle &battle, const std::shared_ptr<GameMap> &gameMap);
    ~PlayerRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

    void setGameMap(std::shared_ptr<GameMap> gameMap);
    bool move(physics::Vector &direction, bool run, real dt);
    bool jump(physics::Vector &direction, real dt);
    bool attack(EntityMovementState state, real dt);
    void update(real dt);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

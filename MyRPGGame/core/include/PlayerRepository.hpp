#ifndef MYRPGGAME_PLAYERREPOSITORY_HPP
#define MYRPGGAME_PLAYERREPOSITORY_HPP

#include "GameEntityRepository.hpp"

class PlayerRepository : public GameEntityRepository {
private:
    std::shared_ptr<Player> player;

public:
    explicit PlayerRepository(const std::shared_ptr<Player> &player, std::unique_ptr<GameEntityMovement> movement,
                              std::unique_ptr<GameEntityBattle> battle, const std::shared_ptr<GameMap> &gameMap);
    ~PlayerRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

    void setGameMap(std::shared_ptr<GameMap> gameMap);
    bool move(physics::Vector &direction, const bool run, const real dt);
    bool attack(const EntityMovementState state, const real dt);
    void update(const real dt);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

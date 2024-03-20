#ifndef MYRPGGAME_GAMEENTITYREPOSITORY_HPP
#define MYRPGGAME_GAMEENTITYREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class GameEntityRepository {
protected:
    std::unique_ptr<GameEntityMovement> movementHandler;
    std::unique_ptr<GameEntityBattle> battleHandler;
    std::shared_ptr<GameMap> map;
    AnimationManager *animationManager = nullptr;

public:
    explicit GameEntityRepository(std::unique_ptr<GameEntityMovement> movement,
                              std::unique_ptr<GameEntityBattle> battle);
    ~GameEntityRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

public:
    void setGameMap(std::shared_ptr<GameMap> gameMap);
    bool move(physics::Vector &direction, const bool run, const real dt);
    // can set the direction vector as const as we don't normalize it like in move()
    bool jump(const physics::Vector &direction, const real dt);
    bool attack(const EntityMovementState state, const real dt);
    void update(const real dt);
};

#endif //MYRPGGAME_GAMEENTITYREPOSITORY_HPP

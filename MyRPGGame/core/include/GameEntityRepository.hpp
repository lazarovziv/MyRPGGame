#ifndef MYRPGGAME_GAMEENTITYREPOSITORY_HPP
#define MYRPGGAME_GAMEENTITYREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class GameEntityRepository {
protected:
    std::unique_ptr<GameEntityMovement> movement_handler;
    std::unique_ptr<GameEntityBattle> battle_handler;
    std::shared_ptr<GameMap> map;
    AnimationManager *animation_manager = nullptr;

public:
    explicit GameEntityRepository(std::unique_ptr<GameEntityMovement> movement,
                              std::unique_ptr<GameEntityBattle> battle);
    ~GameEntityRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

public:
    void set_game_map(std::shared_ptr<GameMap> game_map);
    bool move(physics::Vector &direction, const bool run, const real dt);
    // can set the direction vector as const as we don't normalize it like in move()
    bool jump(const physics::Vector &direction, const real dt);
    bool attack(const EntityMovementState state, const real dt);
    void update(const real dt);
};

#endif //MYRPGGAME_GAMEENTITYREPOSITORY_HPP

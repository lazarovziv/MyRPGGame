#ifndef MYRPGGAME_PLAYERREPOSITORY_HPP
#define MYRPGGAME_PLAYERREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class PlayerRepository {
private:
    Player *player;
    GameEntityMovement *movementHandler = nullptr;
    GameEntityBattle *battleHandler = nullptr;
    GameMap *map = nullptr;

    AnimationManager *animationManager = nullptr;

public:
    explicit PlayerRepository(Player *player, GameEntityMovement *movement,
                              GameEntityBattle *battle, GameMap *gameMap);
    ~PlayerRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

    void setGameMap(GameMap *gameMap);
    void setLastTimeMoved(std::clock_t time);
    Constants::MoveSuccessValues move(MoveDirection direction, EntityMovementState movementState, real dt);
    bool attack(real dt);
    void update(Point ***points, Constants::MoveSuccessValues moveSuccessValue, real dt);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

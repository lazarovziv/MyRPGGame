#ifndef MYRPGGAME_PLAYERREPOSITORY_HPP
#define MYRPGGAME_PLAYERREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class PlayerRepository {
private:
    Player *player;
    GameEntityMovement *movementHandler;
    GameEntityBattle *battleHandler;
    GameMap *map;

    AnimationManager *animationManager;

public:
    explicit PlayerRepository(Player *player, GameEntityMovement *movement,
                              GameEntityBattle *battle, GameMap *gameMap);
    ~PlayerRepository() = default; // TODO: delete all handlers and player (same for enemy repository)

    void setGameMap(GameMap *gameMap);
    void setLastTimeMoved(std::clock_t time);
    Constants::MoveSuccessValues move(MoveDirection direction, EntityMovementState movementState, float dt, bool diagonal);
    bool attack(float dt);
    void update(Point ***points, Constants::MoveSuccessValues moveSuccessValue, float dt);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

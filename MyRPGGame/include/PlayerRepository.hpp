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

public:
    explicit PlayerRepository(Player *player, GameEntityMovement *movement,
                              GameEntityBattle *battle, GameMap *gameMap);
    ~PlayerRepository() = default;

    void setGameMap(GameMap *gameMap);

    Constants::MoveSuccessValues move(MoveDirection direction);
    bool attack();
    void update(Point ***points, Constants::MoveSuccessValues moveSuccessValue);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

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
    explicit PlayerRepository(Player *player, GameMap *gameMap);
    ~PlayerRepository() = default;

    void setGameMap(GameMap *gameMap);

    bool move(MoveDirection direction);
    bool attack(GameEntity &enemy);
};

#endif //MYRPGGAME_PLAYERREPOSITORY_HPP

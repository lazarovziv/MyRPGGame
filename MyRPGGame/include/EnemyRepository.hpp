#ifndef MYRPGGAME_ENEMYREPOSITORY_HPP
#define MYRPGGAME_ENEMYREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class EnemyRepository {
private:
    GameMap *map;
    GameEntityMovement *movementHandler;
    GameEntityBattle *battleHandler;
    Player *player;

public:
    explicit EnemyRepository(GameEntityMovement *movement, GameEntityBattle *battle,
                    Player *player, GameMap *gameMap);
    ~EnemyRepository();

    void setGameMap(GameMap *gameMap);

    void move();
    bool attack(GameEntity *entity);
    void update();
};

#endif //MYRPGGAME_ENEMYREPOSITORY_HPP

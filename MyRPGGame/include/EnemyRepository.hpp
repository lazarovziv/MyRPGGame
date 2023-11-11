#ifndef MYRPGGAME_ENEMYREPOSITORY_HPP
#define MYRPGGAME_ENEMYREPOSITORY_HPP

#include "GameEntityMovement.hpp"
#include "GameEntityBattle.hpp"

class EnemyRepository {
private:
    std::shared_ptr<GameMap> map;
    // GameMap *map;
    // std::unique_ptr<GameEntityMovement> movementHandler;
    GameEntityMovement &movementHandler;
    // std::unique_ptr<GameEntityBattle> battleHandler;
    GameEntityBattle &battleHandler;
    std::shared_ptr<Player> player;
    // Player *player;
    AnimationManager *animationManager = nullptr;

public:
    explicit EnemyRepository(GameEntityMovement &movement, GameEntityBattle &battle,
                    std::shared_ptr<Player> player, std::shared_ptr<GameMap> gameMap);
    ~EnemyRepository() = default;

    void setGameMap(std::shared_ptr<GameMap> gameMap);

    void move(real dt);
    bool attack(GameEntity &entity, real dt);
    void update(real dt);
};

#endif //MYRPGGAME_ENEMYREPOSITORY_HPP

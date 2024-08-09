#ifndef MYRPGGAME_ENEMYREPOSITORY_HPP
#define MYRPGGAME_ENEMYREPOSITORY_HPP

#include "GameEntityRepository.hpp"

class EnemyRepository : public GameEntityRepository {
private:

public:
    explicit EnemyRepository(std::unique_ptr<GameEntityMovement> movement,
                             std::unique_ptr<GameEntityBattle> battle,
                             const std::shared_ptr<GameMap> &game_map);
    ~EnemyRepository() = default;

    bool move(physics::Vector &direction, const bool run, const real dt);
    bool attack(GameEntity &entity, real dt);
};

#endif //MYRPGGAME_ENEMYREPOSITORY_HPP

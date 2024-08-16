#include "../include/EnemyRepository.hpp"

EnemyRepository::EnemyRepository(std::unique_ptr<GameEntityMovement> movement,
                                 std::unique_ptr<GameEntityBattle> battle,
                                 const std::shared_ptr<GameMap> &gameMap)
                                 : GameEntityRepository(std::move(movement), std::move(battle)) {
    setGameMap(gameMap);
    animationManager = new AnimationManager(nullptr);
}

// TODO: create move method for attacking other enemies (some skill for controlling enemies)
bool EnemyRepository::move(physics::Vector &direction, bool run, real dt) {
    for (auto &enemy : map->getEnemies()) {
        if (!enemy->isDead() && enemy->canMove()) {
            // set enemy if not already set
            movementHandler->setEntity(*enemy);
            battleHandler->setEntity(enemy);
            animationManager->setEntity(enemy);
            physics::Vector enemyToPlayerVector = direction - enemy->getPosition();
            // movement handler move method expects a normalized vector
            enemyToPlayerVector.normalize();
            movementHandler->move(enemyToPlayerVector, dt);
            animationManager->animate(EntityMovementState::WALK, dt); // TODO: change to only if in battle area
            /*
            // move randomly
            if (enemy->canChangeDirection()) {
                enemy->resetChangeDirectionInterval();
                int randomDirection = ((int) rand()) % 4; // change to 5 for idle state and fix the animation for NPCEnemy only
                auto direction = (MoveDirection) randomDirection;
                while (direction == enemy->getMoveDirection()) {
                    randomDirection = ((int) rand()) % 4;
                    direction = (MoveDirection) randomDirection;
                }
                enemy->setMoveDirection(direction);
                movementHandler->moveRandomly(randomDirection, dt);
            }
             */
        }
    }
    return true;
}

bool EnemyRepository::attack(GameEntity &entity, const real dt) {
    return battleHandler->attack(entity, dt);
}

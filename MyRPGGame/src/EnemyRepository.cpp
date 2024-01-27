#include "../include/EnemyRepository.hpp"

EnemyRepository::EnemyRepository(GameEntityMovement &movement, GameEntityBattle &battle,
                                 std::shared_ptr<Player> player, std::shared_ptr<GameMap> gameMap) :
                                 movementHandler(movement), battleHandler(battle) {
    this->player = player;
    setGameMap(std::move(gameMap));
    animationManager = new AnimationManager(nullptr);
}

void EnemyRepository::setGameMap(std::shared_ptr<GameMap> gameMap) {
    map = gameMap;
    movementHandler.setCurrentMap(map);
    // referring player to map is taken care of in player repository
}

// TODO: create move method for attacking other enemies (some skill for controlling enemies)
void EnemyRepository::move(real dt) {
    for (auto &enemy : map->getEnemies()) {
        if (!enemy->isDead() && enemy->canMove()) {
            // set enemy if not already set
            movementHandler.setEntity(*enemy);
            battleHandler.setEntity(enemy);
            animationManager->setEntity(enemy);
            physics::Vector enemyToPlayerVector = player->getPosition() - enemy->getPosition();
            movementHandler.move(enemyToPlayerVector, dt);
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
}

bool EnemyRepository::attack(GameEntity &entity, const real dt) {
    return battleHandler.attack(entity, dt);
}

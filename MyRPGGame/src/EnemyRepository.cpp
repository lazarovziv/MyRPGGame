#include "../include/EnemyRepository.hpp"

EnemyRepository::EnemyRepository(GameEntityMovement *movement, GameEntityBattle *battle,
                                 std::shared_ptr<Player> player, std::shared_ptr<GameMap> gameMap) {
    this->player = player;
    movementHandler = movement;
    battleHandler = battle;
    setGameMap(std::move(gameMap));
}

EnemyRepository::~EnemyRepository() {
    // delete movementHandler;
    // delete battleHandler;
}

void EnemyRepository::setGameMap(std::shared_ptr<GameMap> gameMap) {
    map = gameMap;
    movementHandler->setCurrentMap(map);
    // referring player to map is taken care of in player repository
}

// TODO: create move method for attacking other enemies (some skill for controlling enemies)
void EnemyRepository::move(real dt) {
    for (auto &enemy : map->getEnemies()) {
        if (!enemy->isDead() && enemy->canMove()) {
            // set enemy if not already set
            if (movementHandler->getEntity() != enemy) movementHandler->setEntity(*enemy);
            if (battleHandler->getEntity() != enemy) battleHandler->setEntity(enemy);
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
            }
            switch (enemy->getMoveDirection()) {
                case MoveDirection::RIGHT: {
                    movementHandler->moveRandomly(0, dt);
                    break;
                }
                case MoveDirection::LEFT: {
                    movementHandler->moveRandomly(1, dt);
                    break;
                }
                case MoveDirection::UP: {
                    movementHandler->moveRandomly(2, dt);
                    break;
                }
                case MoveDirection::DOWN: {
                    movementHandler->moveRandomly(3, dt);
                    break;
                }
                default: {
                    movementHandler->moveRandomly(4, dt); // don't move
                    break;
                }
            }
        }
    }
}

bool EnemyRepository::attack(GameEntity *entity, real dt) {
    return battleHandler->attack(*entity, dt);
}

void EnemyRepository::update(real dt) {
    map->update(dt);
}
#include "../include/EnemyRepository.hpp"

EnemyRepository::EnemyRepository(GameEntityMovement *movement, GameEntityBattle *battle,
                                 Player *player, GameMap *gameMap) {
    this->player = player;
    movementHandler = movement;
    battleHandler = battle;
    map = gameMap;
    setGameMap(gameMap);
}

EnemyRepository::~EnemyRepository() {
    delete movementHandler;
    delete battleHandler;
}

void EnemyRepository::setGameMap(GameMap *gameMap) {
    map = gameMap;
    movementHandler->setCurrentMap(map);
    // referring player to map is taken care of in player repository
}

// TODO: create move method for attacking other enemies (some skill for controlling enemies)
void EnemyRepository::move(float dt) {
    for (auto &enemy : map->getEnemies()) {
        if (!enemy->isDead() && enemy->canMove()) {
            // set enemy if not already set
            if (movementHandler->getEntity() != enemy) movementHandler->setEntity(*enemy);
            if (battleHandler->getEntity() != enemy) battleHandler->setEntity(enemy);
            // if enemy is in battle (and in battle circle), chase the player
            // TODO: add checks for chasing other entities
            if (enemy->isInBattle() && enemy->isInBattleArea()) {
//                if (player->didJustMove()) movementHandler->moveTowardsEntity(player, map->getMapGraph());
                // calculate path to player
                if (enemy->areAvailableMoves()) {
                    movementHandler->moveBasedOnPoint(enemy->popMove(), dt);
                } else {
                    // attack player when reached him
                    if (attack(player, dt)) {
                        cout << "Attacked player..." << endl;
                        // resetting battle interval for the enemy
                        enemy->resetBattleInterval();
                    }
                    // TODO: trying without regenerating path to player right after attacking it
//                    movementHandler->moveTowardsEntity(player, map->getMapGraph());
                }
            } else {
                // go to wander area and move random in there
                if (enemy->isInWanderArea()) {
                    // keep on going to center of wander area if haven't reached it
                    if (enemy->areAvailableMoves()) {
                        movementHandler->moveBasedOnPoint(enemy->popMove(), dt);
                    } else {
                        // TODO: make enemy move in one direction for a period of time and then change it to another direction randomly
                        // move randomly
                        int randomDirection = ((int) random()) % 4;
                        movementHandler->moveRandomly(randomDirection, dt);
                    }
                } else {
                    // go back to wander area
                    if (enemy->areAvailableMoves()) {
                        movementHandler->moveBasedOnPoint(enemy->popMove(), dt);
                    } /* else movementHandler->moveTowardsPoint(enemy->getWanderAreaCircle()->getCenter(),
                                                             map->getMapGraph()); */
                }
            }
        }
    }
}

bool EnemyRepository::attack(GameEntity *entity, float dt) {
    return battleHandler->attack(*entity, dt);
}

void EnemyRepository::update(float dt) {
    map->update(dt);
}
#include "../include/PlayerRepository.hpp"

PlayerRepository::PlayerRepository(const std::shared_ptr<Player> &player, std::unique_ptr<GameEntityMovement> movement,
                                   std::unique_ptr<GameEntityBattle> battle, const std::shared_ptr<GameMap> &gameMap)
                                   : GameEntityRepository(std::move(movement), std::move(battle)) {
    this->player = player;
    animationManager = new AnimationManager(this->player.get());
    setGameMap(gameMap);
}

void PlayerRepository::setGameMap(std::shared_ptr<GameMap> gameMap) {
    // setting to-be-replaced map's player to null
    if (map != nullptr) map->removePlayer();
    GameEntityRepository::setGameMap(std::move(gameMap));
    map->setPlayer(player);
}

bool PlayerRepository::move(physics::Vector &direction, const bool run, const real dt) {
    // movement handler expects a normalized vector
    direction.normalize();
    if (movementHandler->move(direction, dt) != Constants::MoveSuccessValues::SUCCESS) return false;
    player->setIsRunning(run);
    animationManager->animate(run ? EntityMovementState::RUN : EntityMovementState::WALK, dt);
    return true;
}

// TODO: add listeners invocation
bool PlayerRepository::attack(const EntityMovementState state, const real dt) {
    if (!player->canAttack()) return false;
    // succeeded in attacking any of the enemies
    bool singleSuccess = false;
    // TODO: instead of traversing all enemies, maybe keep a min heap which is ordered by the distance from the player
    for (int i = 0; i < map->getEnemies().size(); i++) {
        if (!map->getEnemies().at(i)->isDead()) {
             singleSuccess = player->isAttacking() || battleHandler->attack(*(map->getEnemies()[i]), dt);
        }
    }
    // resetting move interval because player isn't idle
    if (singleSuccess) {
        player->setIsAttacking(true);
        animationManager->animate(state, dt);
        player->resetMoveInterval();
        // player->resetBattleInterval();
    }

    return singleSuccess;
}

void PlayerRepository::update(const real dt) {
    player->update(dt);
    // player->getWeapon()->update(dt);
    player->notifyAll();
}